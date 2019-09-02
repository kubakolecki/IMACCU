#pragma once

//class for calculating the reverse distortion from
//OpcnCV distortion coefficients

class RevDist
{
public:
	RevDist();
	~RevDist();

	void model2corrections(const vector<double>& DistortionCoefficientsModel,
		cv::Size Size,
		const cv::Mat& K,
		std::array<int,5>& ModelDescription );

	vector<double> OutputCoefficients;
	vector<cv::Point2f> Residuals;
	float rmse = 0.0;


private:
	short _grid_dim = 4;

};

