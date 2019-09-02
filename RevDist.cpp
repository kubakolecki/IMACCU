#include "pch.h"
#include "RevDist.h"
#include <numeric>
#include <opencv2/imgproc.hpp>
#include <Eigen/Core> //for placeholders
#include <Eigen/Dense>
#include <Eigen/Eigen>

RevDist::RevDist()
{
}


RevDist::~RevDist()
{
}

void RevDist::model2corrections(const vector<double>& DistortionCoefficientsModel,
	cv::Size Size,
	const cv::Mat & K,
	std::array<int, 5>& ModelDescription)
{
	//this function converts the opencv distortion coefficients to
	//the coeffictients that model the corrections to the measured image coordinates
	//DistortionCoefficientsModel - input 5-element vector of opencv distotrtion coeffs.
	//Size - image size, needed only to generate the adjustment grid
	//K - camera matrix - can be the output from standard opencv calibration
	//ModelDescritpion - it is the 5-element array of ints indicating which dist coeffs are to be estimated
	//where 0 excludes from estimation and 1 enables the estimation
	//the order of coeffs is as follows:  [k1 k2 k3 p1 p2] - different as the order of input coeffs!
	
	//1. Preparation
	OutputCoefficients.clear();
	Residuals.clear();
	//interior orientation parameters in the image cantered frame
	double x0 = K.at<double>(0, 2) - Size.width / 2.0 + 0.5;
	double y0 = Size.height / 2.0 - K.at<double>(1, 2) - 0.5;
	double ck = K.at<double>(0, 0);

	//1. determie the nodes of transformation to be used in to calculate undistortion:
	//here we use 10 x 10 grid of points:
	int grid_size_x = _grid_dim;
	int grid_size_y = _grid_dim;
	double grid_separation_x = static_cast<float>(Size.width) / (grid_size_x - 1);
	double grid_separation_y = static_cast<float>(Size.height) / (grid_size_y - 1);
	int grid_size_total = grid_size_x * grid_size_y;

	cv::Mat input_grid_points(grid_size_total, 1, CV_32FC2);
	cv::Mat output_undistorted_points;

	for (int row = 0; row < grid_size_y; row++)
	{
		for (int col = 0; col < grid_size_x; col++)
		{
			input_grid_points.at<cv::Vec2f>(row*grid_size_y + col, 0)[0] = col * grid_separation_x;
			input_grid_points.at<cv::Vec2f>(row*grid_size_y + col, 0)[1] = row * grid_separation_y;
		}
	}

	for (int row = 0; row < input_grid_points.rows; row++)
	{
		float x = input_grid_points.at<cv::Vec2f>(row, 0)[0];
		float y = input_grid_points.at<cv::Vec2f>(row, 0)[1];
	}

	//2. undistort pooints
	//thats the only line whera DistortionCoefficientsModel is needed
	cv::undistortPoints(input_grid_points, output_undistorted_points, K, DistortionCoefficientsModel, cv::noArray(), K);

	for (int row = 0; row < output_undistorted_points.rows; row++)
	{
		float x = output_undistorted_points.at<cv::Vec2f>(row, 0)[0];
		float y = output_undistorted_points.at<cv::Vec2f>(row, 0)[1];
	}

	//3. calculate fiducial coordinates of both grid points and undistorted points
	vector<ImagePoint> GridPointsCorner;
	vector<ImagePoint> GridPointsPixel;
	vector<ImagePoint> GridPointsFiducial;
	vector<ImagePoint> UndistortedPointsCorner;
	vector<ImagePoint> UndistortedPointsPixel;
	vector<ImagePoint> UndistortedPointsFiducial;
	vector<SimplePoint<int> > Corrections;

	for (int row = 0; row < input_grid_points.rows; row++)
	{
		GridPointsCorner.emplace_back(
			input_grid_points.at<cv::Vec2f>(row, 0)[0],
			input_grid_points.at<cv::Vec2f>(row, 0)[1]);
	}

	for (int row = 0; row < output_undistorted_points.rows; row++)
	{
		UndistortedPointsCorner.emplace_back(
			output_undistorted_points.at<cv::Vec2f>(row, 0)[0],
			output_undistorted_points.at<cv::Vec2f>(row, 0)[1]);
	}

	fT_cor2pix(GridPointsCorner, GridPointsPixel, Size.width, Size.height);
	fT_cor2pix(UndistortedPointsCorner, UndistortedPointsPixel, Size.width, Size.height);

	GridPointsFiducial.resize(GridPointsPixel.size());
	UndistortedPointsFiducial.resize(UndistortedPointsPixel.size());

	std::transform(GridPointsPixel.begin(), GridPointsPixel.end(), GridPointsFiducial.begin(),
		[&](ImagePoint &P1) {return ImagePoint(P1.X - x0, P1.Y - y0); });

	std::transform(UndistortedPointsPixel.begin(), UndistortedPointsPixel.end(), UndistortedPointsFiducial.begin(),
		[&](ImagePoint &P1) {return ImagePoint(P1.X - x0, P1.Y - y0); });

	for (int i = 0, S = GridPointsFiducial.size(); i < S; i++)
	{
		double correctionx = UndistortedPointsFiducial.at(i).X - GridPointsFiducial.at(i).X;
		double correctiony = UndistortedPointsFiducial.at(i).Y - GridPointsFiducial.at(i).Y;
		Corrections.emplace_back(correctionx, correctiony, i);
	}

	//4. Preparing adjustment problem
	//coordinate normalization for better conditioning:
	std::transform(GridPointsFiducial.begin(), GridPointsFiducial.end(), GridPointsFiducial.begin(),
		[&](ImagePoint &P1) { return ImagePoint(P1.X / ck, P1.Y / ck); });

	std::transform(Corrections.begin(), Corrections.end(), Corrections.begin(),
		[&](SimplePoint<int> &P1) { return SimplePoint<int>(P1.x / ck, P1.y / ck, 0); });


	//matrix preparation
	int NumOfUnknowns = std::accumulate(ModelDescription.begin(), ModelDescription.end(), 0);

	Eigen::MatrixXd A(2 * GridPointsFiducial.size(), 5);
	Eigen::MatrixXd ADeleted(2 * GridPointsFiducial.size(), NumOfUnknowns);
	Eigen::VectorXd L(2 * Corrections.size());
	Eigen::VectorXd DistortionSolution(NumOfUnknowns, 1);
	Eigen::VectorXd V(2 * GridPointsFiducial.size());

	//filling matrix
	for (int i = 0, S = GridPointsFiducial.size(); i < S; i++)
	{
		double x = GridPointsFiducial.at(i).X;
		double y = GridPointsFiducial.at(i).Y;
		double x2 = x * x;
		double y2 = y * y;
		double r2 = x2 + y2;

		A(2 * i, 0) = x * r2;
		A(2 * i, 1) = x * r2 * r2;
		A(2 * i, 2) = x * r2 * r2 * r2;
		A(2 * i, 3) = r2 + 2 * x2;
		A(2 * i, 4) = 2 * x *y;

		A(2 * i + 1, 0) = y * r2;
		A(2 * i + 1, 1) = y * r2 * r2;
		A(2 * i + 1, 2) = y * r2 * r2 * r2;
		A(2 * i + 1, 3) = 2 * x *y;
		A(2 * i + 1, 4) = r2 + 2 * y2;
	}

	//deletion:
	int ADeletedColumn = 0;
	for (int i = 0; i < ModelDescription.size(); i++ )
	{
		if (ModelDescription[i] == 1)
		{
			for (int j = 0; j < A.rows(); j++)
			{
				ADeleted(j, ADeletedColumn) = A(j, i);
			}
			
			ADeletedColumn++;
		}
	}

	//filling L vector
	for (int i = 0, S = Corrections.size(); i < S; i++)
	{
		L(2 * i) = Corrections.at(i).x;
		L(2 * i + 1) = Corrections.at(i).y;
	}

	//solving the system of linear equations
	DistortionSolution = ADeleted.colPivHouseholderQr().solve(L);

	//calculating residuals:
	V = L - ADeleted * DistortionSolution;

	//filling the residual vector:
	Residuals.reserve(Corrections.size());
	for (int i = 0, S = Corrections.size(); i < S; i++)
	{
		Residuals.push_back(cv::Point2f(V(2 * i), V(2 * i + 1)));		
	}
	Residuals.shrink_to_fit();
	
	//calculating rmse of the reverse distortion fitting:
	Eigen::VectorXd VTV = V.transpose()*V;
	rmse = std::sqrt(VTV(0) / (V.rows() - NumOfUnknowns));

	OutputCoefficients.resize(5, 0.0);
	int CoeffIdx = 0;
	for (int i = 0; i < ModelDescription.size(); i++)
	{
		if (ModelDescription[i] == 1)
		{
			OutputCoefficients.at(CoeffIdx) = -DistortionSolution(i); //we take the "-" signed values
			CoeffIdx++;
		}
	}



}
