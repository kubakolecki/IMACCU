#include "pch.h"
#include "OpenCVCalibWindow.h"
#include "RevDist.h"
#include <map>
#include <numeric>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp> //for reverese distortion estimation
#include <Eigen/Dense> //for reverse distortion estimation


OpenCVCalibWindow::OpenCVCalibWindow(int w, int h, const char* title) :Fl_Window(w, h, title)
{
	begin();
			
		Browser_Images = new Fl_Multi_Browser(20, 20, 220, 500, "Images");
		Browser_Control_Points = new Fl_Multi_Browser(250, 20, 60, 500, "#GCP");
		Browser_Results = new Fl_Multi_Browser(320, 20, 80, 500, "RMS [px]");

		Check_Estimate_k1 = new Fl_Check_Button(420, 20, 20, 20, "Estimate K1");
		Check_Estimate_k2 = new Fl_Check_Button(420, 50, 20, 20, "Estimate K2");
		Check_Estimate_k3 = new Fl_Check_Button(420, 80, 20, 20, "Estimate K3");
		Check_Estimate_tangential = new Fl_Check_Button(420, 110, 20, 20, "Estimate P1 and P2");
		Check_Estimate_principalpoint = new Fl_Check_Button(420, 140, 20, 20, "Estimate x0 and y0");
		Input_Raport_File_Name = new Fl_Input(510, 190, 140, 30, "Raport File:");
		Button_Run = new Fl_Button(420, 240, 100, 30, "Run");
		Box_Save_EO = new Fl_Box(420, 290, 120, 30, "Save External Orientation Options:");

		Check_EO_Approximated = new Fl_Check_Button(420, 330, 20, 20, "Approximated");
		Check_EO_Observed = new Fl_Check_Button(420, 360, 20, 20, "Observed");
		Check_EO_Adjusted = new Fl_Check_Button(420, 390, 20, 20, "Adjusted");

		Button_SaveEO = new Fl_Button(420, 430, 100, 30,"Save EO");

		Button_SaveCamera = new Fl_Button(420, 480, 100, 30, "Save Camera");

		Window_Warnign_Not_Enough_Images = new WarningWindow(240, 180, "Error!", "Less than 2 images!");
		Window_Warnign_Multiple_Cameras = new WarningWindow(240, 180, "Error!", "Multiple cameras!");
		Window_Save_Camera = new CameraSaveWindow(380, 170, "Save Camera");
	end();

	//setting callbacks:
	Button_Run->callback(cb_Button_Run, this);
	Button_SaveEO->callback(cb_Button_SaveEO, this);
	Button_SaveCamera->callback(cb_Button_SaveCamera, this);

	//setting properties:
	Button_SaveCamera->deactivate();
	Input_Raport_File_Name->value("calib_raport.txt");
	Check_Estimate_k1->value(1);
	Check_Estimate_k2->value(1);
	Check_Estimate_k3->value(1);
	Check_Estimate_tangential->value(1);
	Check_Estimate_principalpoint->value(1);
	Check_EO_Approximated->value(1);
	Box_Save_EO->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

	Window_Save_Camera->win = (void*)this;

}

OpenCVCalibWindow::~OpenCVCalibWindow()
{
	delete Button_Run;
	delete Browser_Images;
	delete Browser_Control_Points;
	delete Browser_Results;
}

void OpenCVCalibWindow::cb_Button_Run(Fl_Widget * w, void * v)
{
	((OpenCVCalibWindow*)v)->cb_Button_Run_i();
}

inline void OpenCVCalibWindow::cb_Button_Run_i()
{
	//clearing browser
	Browser_Results->clear();
	
	//clearing eo containers
	CvRotationMatrices.clear();
	CvProjectionCenters.clear();
	Angles.clear();
	IdaData.clear();

	//checking some prerequesities:
	int ImListSize = Browser_Images->size();
	vector<bool> IsUsable(ImListSize, true); //checks which images to use

	if (ImListSize < 2)
	{
		Window_Warnign_Not_Enough_Images->show();
		Window_Warnign_Not_Enough_Images->set_modal();
		return;
	}

	//checking if all images have the same size and camera
	std::string idafilename = ((MainWindow*)win)->ProjectFolder + std::string(Browser_Images->text(1));
	Image Obraz;
	Obraz.ReadFromFile(idafilename.c_str());
	int W0 = Obraz.W;
	int H0 = Obraz.H;
	std::string Cam0 = Obraz.CamFileName;

	for (int im = 2; im <= ImListSize; im++)
	{
		idafilename = ((MainWindow*)win)->ProjectFolder + std::string(Browser_Images->text(im));
		Obraz.ReadFromFile(idafilename.c_str());
		if (Obraz.W != W0 || Obraz.H != H0 || Obraz.CamFileName != Cam0)
		{
			Window_Warnign_Multiple_Cameras->show();
			Window_Warnign_Multiple_Cameras->set_modal();
			return;
		}
	}


	//transfomring GPC vector into map to handl it easily:
	std::map<string, TerrainPoint> GCPMAP;

	for (auto &gc : ((MainWindow*)win)->GC)
	{
		GCPMAP[gc.Name] = gc;
	}
	

	vector<vector<cv::Point3f> > CalibrationInputGCP; //terrain points
	vector<vector<cv::Point2f> > CalibrationInputIMG; //image points
	vector<vector<SimplePoint<string> > > ImagePoints; //image points but with their names
	vector<vector<SimplePoint<string> > > Residuals;

	CalibrationInputGCP.reserve(ImListSize);
	CalibrationInputIMG.reserve(ImListSize);

	ImagePoints.reserve(ImListSize);
	Residuals.reserve(ImListSize);

	Camera Cam; //camera data for opencv camera matrix initialization
	
	
	//colecting image points and ground control points
	for (int im = 1; im <= ImListSize; im++)
	{
		int NumOfGCPInImage = atoi(Browser_Control_Points->text(im));
		if (NumOfGCPInImage < MinNumOfGCPsPerImage)
		{
			IsUsable.at(im-1) = false;
			continue; //insuficient number of controll points in this image, go to next
		}

		vector<cv::Point3f> punkty_teren;
		vector<cv::Point2f> punkty_obraz;
		vector<SimplePoint<string> > punkty_z_id_obraz;

		idafilename = ((MainWindow*)win)->ProjectFolder + std::string(Browser_Images->text(im));
		std::string pixfilename = idafilename.substr(0, idafilename.length() - 4) + ".pix";

		Obraz.ReadFromFile(idafilename.c_str());
		IdaData.push_back(Obraz);

		if (im == 1) //get camera data from the fist image
		{
			Cam.ck = Obraz.Interior[0];
			Cam.x0 = Obraz.Interior[1];
			Cam.y0 = Obraz.Interior[2];
			Cam.W = Obraz.W;
			Cam.H = Obraz.H;
		}

		punkty_teren.reserve(NumOfGCPInImage);
		punkty_obraz.reserve(NumOfGCPInImage);
		punkty_z_id_obraz.reserve(NumOfGCPInImage);

		ifstream STR;
		STR.open(pixfilename);

		string pointname, imagename, linename;
		float x, y;
		float mx, my;
		int type, usage;
		while (STR) //read point by point from the pix file
		{
			STR >> pointname >> imagename >> linename >> x >> y >> mx >> my >> type >> usage;
			if (STR && type == 3) //picked point is gcp
			{
				auto gcpit = GCPMAP.find(pointname);
				if (gcpit != GCPMAP.end()) //if gcp is on the list of gcp, ie has terrain coordinates
				{
					punkty_obraz.emplace_back(x + Obraz.W / 2.0 - 0.5, Obraz.H/2.0 - y - 0.5);
					punkty_teren.emplace_back(gcpit->second.X, gcpit->second.Y, gcpit->second.Z);
					punkty_z_id_obraz.emplace_back(x, y, pointname);
				}
			}
		}

		punkty_teren.shrink_to_fit();
		punkty_obraz.shrink_to_fit();
		punkty_z_id_obraz.shrink_to_fit();

		CalibrationInputGCP.push_back(punkty_teren);
		CalibrationInputIMG.push_back(punkty_obraz);
		ImagePoints.push_back(punkty_z_id_obraz);
		STR.close();

	}
	CalibrationInputGCP.shrink_to_fit();
	CalibrationInputIMG.shrink_to_fit();
	

	//now image and terrain data are ready, let's prepare camera matrix:
	cv::Mat K(3, 3, CV_64FC1);
	K.at<double>(0, 0) = Cam.ck;
	K.at<double>(0, 1) = 0.0;
	K.at<double>(0, 2) = Cam.x0 + Obraz.W / 2.0 - 0.5;
	K.at<double>(1, 0) = 0.0;
	K.at<double>(1, 1) = Cam.ck;
	K.at<double>(1, 2) = Obraz.H / 2.0 - Cam.y0 - 0.5;
	K.at<double>(2, 0) = 0.0;
	K.at<double>(2, 1) = 0.0;
	K.at<double>(2, 2) = 1.0;

	vector<double>  Distortion = { 0.0, 0.0, 0.0, 0.0, 0.0, 0,0 };
	vector<cv::Mat> Rotations; //Opencv Rotations (their are not rotation matrices)
	vector<cv::Mat> Translations; //Opencv Translations (thier are not projection centers)
	vector<double> stdDeviationsIntrinsics;
	vector<double> stdDeviationsExtrinsics;
	vector<double> perViewErrors;


	//preparing flags:
	int flags = cv::CALIB_USE_INTRINSIC_GUESS | cv::CALIB_FIX_ASPECT_RATIO;

	if (Check_Estimate_k1->value() == 0) flags |= cv::CALIB_FIX_K1;
	if (Check_Estimate_k2->value() == 0) flags |= cv::CALIB_FIX_K2;
	if (Check_Estimate_k3->value() == 0) flags |= cv::CALIB_FIX_K3;
	if (Check_Estimate_tangential->value() == 0) flags |= cv::CALIB_ZERO_TANGENT_DIST;
	if (Check_Estimate_principalpoint->value() == 0) flags |= cv::CALIB_FIX_PRINCIPAL_POINT;


	cv::calibrateCamera(	CalibrationInputGCP, CalibrationInputIMG,
							cv::Size(Cam.W,Cam.H), K, Distortion, Rotations, Translations,
							stdDeviationsIntrinsics, stdDeviationsExtrinsics, perViewErrors,
							flags);

	//Principial point coordinates in image-centerd frame;
	double x0 = K.at<double>(0, 2) - W0 / 2.0 + 0.5;
	double y0 = H0 / 2.0 - K.at<double>(1, 2) - 0.5;
	double ck = K.at<double>(0, 0);

	for (int im = 0, veid = 0 ; im< ImListSize; im++)
	{
		if (IsUsable.at(im)) Browser_Results->add(num2str_d(perViewErrors.at(veid++), 3).c_str());
		else Browser_Results->add("----");
	}


	//Writing report:
	std::string strrep = ((MainWindow*)win)->ProjectFolder + "REP\\" + std::string(Input_Raport_File_Name->value());

	ofstream Repstream;
	Repstream.open(strrep);
	Repstream << "Camera calibration - OpenCV library - report" << std::endl;
	Repstream << std::endl;
	Repstream << "This camera calibration approach involves using mulitple views of calibration pattern\n";
	Repstream << "of ground control points. Tie points are not used for the estimation.\n";
	Repstream << "Control points are assumed to be 'ideal', ie have no apriori errors.\n";
	Repstream << "Applied projection model is the same as in the OpenCV library.\n";
	Repstream << "For more detailes see: https://docs.opencv.org/3.4.6/d9/d0c/group__calib3d.html#ga3207604e4b1a1758aa66acb6ed5aa65d (visit 31.05.2019)\n";
	Repstream << "The crucial difference is that in the OpenCV model the distortion is applied to\n";
	Repstream << "projected points (according to the ideal pinhole projection model) not to measurements (ie points\n";
	Repstream << "affected by distortion)\n";
	Repstream << "Distortion coefficients are normalized ie. they apply to the x', y' or r' (see equations at the web page),\n";
	Repstream << "not to the [u, v] coordinates.\n";
	Repstream << "Distortion coefficients that serve for the correction calculation are esitmated using the model fitting approach and\n";
	Repstream << "are provided at the end of this report\n";

	Repstream << "Number of images: " << CalibrationInputGCP.size() << "\n";
	Repstream << "Estimated parameters with standard deviations:\n";
	Repstream << "Principal distance (ck) [px]: "<<fixed<<setprecision(4) << K.at<double>(0,0) << " " << stdDeviationsIntrinsics.at(1) << "\n";
	Repstream << "Principal point coord. x0 [px]: "<<fixed<<setprecision(4) <<K.at<double>(0,2) << " " << stdDeviationsIntrinsics.at(2) << "\n";
	Repstream << "Principal point coord. y0 [px]: " << fixed << setprecision(4) << K.at<double>(1, 2) << " " << stdDeviationsIntrinsics.at(3) << "\n";
	Repstream << "k1: " << fixed << setprecision(6) << Distortion.at(0) << " " << stdDeviationsIntrinsics.at(4) << "\n";
	Repstream << "k2: " << fixed << setprecision(6) << Distortion.at(1) << " " << stdDeviationsIntrinsics.at(5) << "\n";
	Repstream << "k3: " << fixed << setprecision(6) << Distortion.at(4) << " " << stdDeviationsIntrinsics.at(8) << "\n";
	Repstream << "p1: " << fixed << setprecision(6) << Distortion.at(2) << " " << stdDeviationsIntrinsics.at(6) << "\n";
	Repstream << "p2: " << fixed << setprecision(6) << Distortion.at(3) << " " << stdDeviationsIntrinsics.at(7) << "\n\n";

	Repstream << "image-centered principal point [px], photogrammetric convension (y axis up, x axis right):\n";
	Repstream << "[" << K.at<double>(0, 2) - W0 / 2.0 + 0.5 << " " << H0 / 2.0 - K.at<double>(1, 2) - 0.5 << "]\n\n";

	Repstream << "camera matrix:\n";
	Repstream << K;
	Repstream << "\n\n";

	Repstream << "2D reprojection errors for images:\n";
	for (int i=0, S = perViewErrors.size(); i<S; i++)
	{
		Repstream << setw(14) << Browser_Images->text(i+1) << " " <<fixed<<setprecision(4)<< perViewErrors.at(i) << "\n";
	}



	//we have to do some transformations before printing
	for (int i = 0, S = Rotations.size(); i < S; i++)
	{
		cv::Mat R(3, 3, CV_64FC1);
		cv::Mat RT(3, 3, CV_64FC1);
		cv::Rodrigues(Rotations.at(i), R);
		//we need to calculate rotation matrix transpose as R represents terrain to camera rotation
		cv::transpose(R, RT);	
		CvRotationMatrices.push_back(RT);

		//its important to know that translations resulting from cvcalib are not projection centers!
		cv::Mat PC(3, 1, CV_64FC1);
		cv::gemm(R, Translations.at(i), -1.0, cv::Mat(3, 3, CV_64FC1, cv::Scalar(0.0)), 0, PC, CV_GEMM_A_T);
		CvProjectionCenters.push_back(PC);
	}

	Repstream << "\nExterior orientation paramters as a transformation (4 X 4) matrices: \n";
	Repstream << "OpenCV camera frame (z axis forwards, y axis down, x axis right):\n";
	for (int i=0, S = CvRotationMatrices.size(); i<S; i++)
	{
		Repstream << IdaData.at(i).Name << "\n";
		for (int j = 0; j < 3; j++)
		{
			Repstream << setw(14) << fixed << setprecision(10) << CvRotationMatrices.at(i).at<double>(j, 0) << " ";
			Repstream << setw(14) << fixed << setprecision(10) << CvRotationMatrices.at(i).at<double>(j, 1) << " ";
			Repstream << setw(14) << fixed << setprecision(10) << CvRotationMatrices.at(i).at<double>(j, 2) << " ";
			Repstream << setw(8)  << fixed << setprecision(4) << CvProjectionCenters.at(i).at<double>(j, 0) << "\n";
		}
		Repstream << setw(14) << fixed << setprecision(10) << 0.0 << " ";
		Repstream << setw(14) << fixed << setprecision(10) << 0.0 << " ";
		Repstream << setw(14) << fixed << setprecision(10) << 0.0 << " ";
		Repstream << setw(8)  << fixed <<  setprecision(4) << 1.0 << "\n";
	}

	Repstream << "\nExterior orientation paramters as a transformation (4 X 4) matrices: \n";
	Repstream << "Bundlab camera frame - photogrammetric convention (z axis backwards, y axis up, x axis right):\n";

	double R_CV_B_data[9] = { 1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0 };
	cv::Mat R_CV_B = cv::Mat(3, 3, CV_64FC1, R_CV_B_data);

	for (int i = 0, S = CvRotationMatrices.size(); i < S; i++)
	{
		cv::Mat R_BUNDLAB = cv::Mat(3, 3, CV_64FC1);
		cv::gemm(CvRotationMatrices.at(i), R_CV_B, 1.0, cv::Mat(3,3,CV_64FC1,cv::Scalar(0.0)), 0, R_BUNDLAB);
		
		Repstream << IdaData.at(i).Name << "\n";
		for (int j = 0; j < 3; j++)
		{
			Repstream << setw(14) << fixed << setprecision(10) << R_BUNDLAB.at<double>(j, 0) << " ";
			Repstream << setw(14) << fixed << setprecision(10) << R_BUNDLAB.at<double>(j, 1) << " ";
			Repstream << setw(14) << fixed << setprecision(10) << R_BUNDLAB.at<double>(j, 2) << " ";
			Repstream << setw(8)  << fixed << setprecision(4) << CvProjectionCenters.at(i).at<double>(j, 0) << "\n";
		}
		Repstream << setw(14) << fixed << setprecision(10) << 0.0 << " ";
		Repstream << setw(14) << fixed << setprecision(10) << 0.0 << " ";
		Repstream << setw(14) << fixed << setprecision(10) << 0.0 << " ";
		Repstream << setw(8)  << fixed << setprecision(4) << 1.0 << "\n";

	}

	Repstream << "\nExterior orientation paramters as projection centers and angles [deg].\n";
	Repstream << "Bundlab camera frame - photogrammetric convention (z axis backwards, y axis up, x axis right).\n";
	Repstream << "Calculation of angles accoridng to sequence from .ida file.\n";
	Repstream << "al-ni-ka <=> z->x->z; om-fi-ka <=> x->y->z; roll-pitch-yaw <=> z->y->x\n";

	for (int i = 0, S = CvRotationMatrices.size(); i < S; i++)
	{
		cv::Mat R_BUNDLAB = cv::Mat(3, 3, CV_64FC1);
		cv::gemm(CvRotationMatrices.at(i), R_CV_B, 1.0, cv::Mat(3, 3, CV_64FC1, cv::Scalar(0.0)), 0, R_BUNDLAB);
		
		double R_BUNDLAB_data[9];
		int l = 0;
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				R_BUNDLAB_data[l] = R_BUNDLAB.at<double>(j, k);
				l++;
			}
		}
		double ANGLES[3];
		fT_rot2angles(R_BUNDLAB_data, ANGLES, IdaData.at(i).AngularSystem);
		Angles.push_back(std::array<double,3>({ ANGLES[0], ANGLES[1], ANGLES[2] }) );

		Repstream <<setw(20) << IdaData.at(i).Name << " ";
		Repstream << setw(15) << fixed << setprecision(4) << CvProjectionCenters.at(i).at<double>(0, 0) << " ";
		Repstream << setw(15) << CvProjectionCenters.at(i).at<double>(1, 0) << " ";
		Repstream << setw(15) << CvProjectionCenters.at(i).at<double>(2, 0) << " ";
		Repstream << setw(15) << fixed << setprecision(4) << setw(15) <<ANGLES[0] << " " << setw(15) << ANGLES[1] << " " << setw(15) << ANGLES[2] << " ";
		Repstream << IdaData.at(i).AngularSystem << "\n";
	}

	//caluclating projections:

	vector<vector<cv::Point2f> > Projections(Translations.size());

	for (int i = 0, S = Translations.size(); i < S; i++)
	{
		cv::projectPoints(CalibrationInputGCP.at(i),
			Rotations.at(i),
			Translations.at(i),
			K,
			Distortion,
			Projections.at(i));
	}

	//calculating residuals

	for (int i = 0, S = Projections.size(); i < S; i++)
	{
		vector<SimplePoint<string> > v;
		v.reserve(Projections.at(i).size());
		for (int j = 0, SS = Projections.at(i).size(); j < SS; j++)
		{
			double x = Projections.at(i).at(j).x - 0.5 * IdaData.at(i).W + 0.5;
			double y = 0.5 * IdaData.at(i).H - Projections.at(i).at(j).y - 0.5;

			double vx = x - ImagePoints.at(i).at(j).x;
			double vy = y - ImagePoints.at(i).at(j).y;
			v.emplace_back(vx, vy, ImagePoints.at(i).at(j).Nr);
		}
		v.shrink_to_fit();
		Residuals.push_back(v);
	}
	Residuals.shrink_to_fit();

	Repstream << "\nResiduals[px]: for each image, sorted starting from the largest\n";
	Repstream << setw(12) << "POINT_ID" <<setw(10) << "vx[px]" <<setw(10) << "vy[px]" <<setw(10) <<  "vxy[px]" <<" \n";

	for (int i = 0, S = IdaData.size(); i < S; i++)
	{
		Repstream << IdaData.at(i).Name << "\n";

		std::sort(Residuals.at(i).begin(), Residuals.at(i).end(),
			[](SimplePoint<string>& p1, SimplePoint<string>& p2)
			{
			return p1.x*p1.x + p1.y*p1.y > p2.x*p2.x + p2.y*p2.y;
			}
		);

		for (auto &v : Residuals.at(i))
		{
			Repstream << setw(12) << v.Nr << " "<<fixed<<setprecision(3) <<setw(9) << v.x << " " <<setw(9) << v.y <<" "<<setw(9) << sqrt(v.x*v.x + v.y*v.y) << "\n";
		}
		Repstream << "\n";
	}

	//copying io parameters
	CalibratedCamera.ck = ck;
	CalibratedCamera.x0 = x0;
	CalibratedCamera.y0 = y0;
	//copying size
	CalibratedCamera.W = W0;
	CalibratedCamera.H = H0;

	//calculation of reverse distortion model using cv::undistort (to be put to separate class)

	//only if distortion has been estimated:
	
	std::array<int,5> RevDistCalcCode =
	{
		static_cast<int>(Check_Estimate_k1->value()),
		static_cast<int>(Check_Estimate_k2->value()),
		static_cast<int>(Check_Estimate_k3->value()),
		static_cast<int>(Check_Estimate_tangential->value()),
		static_cast<int>(Check_Estimate_tangential->value()),
	};

	if (std::accumulate(RevDistCalcCode.begin(), RevDistCalcCode.end(), 0) > 0)
	{
		RevDist RevDistEstimator;
		RevDistEstimator.model2corrections(Distortion, cv::Size(W0, H0), K, RevDistCalcCode);
		double c = ck * ck;
		CalibratedCamera.k1 = RevDistEstimator.OutputCoefficients[0] / c;
		CalibratedCamera.k2 = RevDistEstimator.OutputCoefficients[1] / (c*c);
		CalibratedCamera.k3 = RevDistEstimator.OutputCoefficients[2] / (c*c*c);
		CalibratedCamera.p1 = RevDistEstimator.OutputCoefficients[3] / ck;
		CalibratedCamera.p2 = RevDistEstimator.OutputCoefficients[4] / ck;

		Repstream << "Bundlab distortion corection coefficients estimation: " <<"\n";
		Repstream << "The coefficeints allow to correct the measured coordinates for distortion." << "\n";
		Repstream << "The corrections are calculated according to the following formula:" << "\n";
		Repstream << "DX = -(x*k1*r^2 + x*k2*r^4 + x*k3*r^6 + p1*(r^2 + 2*x^2) + 2*p2*x*y)" << "\n";
		Repstream << "DY = -(y*k1*r^2 + y*k2*r^4 + y*k3*r^6 + 2*p1*x*y + p2*(r^2 + 2*y^2))" << "\n";
		Repstream << "where:" << "\n";
		Repstream << "x, y are the measured fiducial coordiantes (x axis left, y axis up)," << "\n";
		Repstream << "r^2 = x^2 + y^2" << "\n";
		Repstream << "and the corrected coordinates are:\n";
		Repstream << "x_corrected = x + DX\n";
		Repstream << "y_corrected = y + DY\n";

		Repstream << "\n";
		Repstream << "qaulity of model fitting: RMSE = " << scientific <<setprecision(6) <<RevDistEstimator.rmse<<" [px]" << "\n";
		Repstream << "output parameters (normalized): " << "\n";
		Repstream << "k1: "<< fixed << setprecision(8)<<setw(12) << RevDistEstimator.OutputCoefficients[0] << "\n";
		Repstream << "k2: "<< fixed << setprecision(8)<<setw(12) << RevDistEstimator.OutputCoefficients[1] << "\n";
		Repstream << "k3: "<< fixed << setprecision(8)<<setw(12) << RevDistEstimator.OutputCoefficients[2] << "\n";
		Repstream << "p1: "<< fixed << setprecision(8)<<setw(12) << RevDistEstimator.OutputCoefficients[3] << "\n";
		Repstream << "p2: "<< fixed << setprecision(8)<<setw(12) << RevDistEstimator.OutputCoefficients[4] << "\n";
		Repstream << "output parameters: " << "\n";
		Repstream << "k1: " << scientific << setprecision(6)<<setw(15) << CalibratedCamera.k1<<" [px^-2]"<< "\n";
		Repstream << "k2: " << scientific << setprecision(6)<<setw(15) << CalibratedCamera.k2<<" [px^-4]"<< "\n";
		Repstream << "k3: " << scientific << setprecision(6)<<setw(15) << CalibratedCamera.k3<<" [px^-6]"<< "\n";
		Repstream << "p1: " << scientific << setprecision(6)<<setw(15) << CalibratedCamera.p1<<" [px^-1]"<< "\n";
		Repstream << "p2: " << scientific << setprecision(6)<<setw(15) << CalibratedCamera.p2<<" [px^-1]"<< "\n";
			
		

	}

	Repstream.close();

	Button_SaveCamera->activate();
}

void OpenCVCalibWindow::cb_Button_SaveEO(Fl_Widget * w, void * v)
{
	((OpenCVCalibWindow*)v)->cb_Button_SaveEO_i();
}

inline void OpenCVCalibWindow::cb_Button_SaveEO_i()
{
	for (int i = 0, S = IdaData.size(); i < S; i++)
	{
		//cout << "saving EO for: " << IdaData.at(i).Name << std::endl;

		//preparing arrays:
		double ANG[3] = { Angles.at(i).at(0), Angles.at(i).at(1), Angles.at(i).at(2) };
		double COR[3] = { CvProjectionCenters.at(i).at<double>(0, 0),
			CvProjectionCenters.at(i).at<double>(1, 0),
			CvProjectionCenters.at(i).at<double>(2, 0) };


		if (Check_EO_Approximated->value() == 1)
		{
			IdaData.at(i).setAnglesApr(ANG);
			IdaData.at(i).setCoordsApr(COR);
		}

		if (Check_EO_Observed->value() == 1)
		{
			IdaData.at(i).setAnglesObs(ANG);
			IdaData.at(i).setCoordsObs(COR);
		}

		if (Check_EO_Adjusted->value() == 1)
		{
			IdaData.at(i).setAnglesAdj(ANG);
			IdaData.at(i).setCoordsAdj(COR);
		}

		string IdaFileName = ((MainWindow*)win)->ProjectFolder + IdaData.at(i).Name + ".ida";
		IdaData.at(i).WriteToFile(IdaFileName.c_str());
		
	}

}

void OpenCVCalibWindow::cb_Button_SaveCamera(Fl_Widget * w, void * v)
{
	((OpenCVCalibWindow*)v)->cb_Button_SaveCamera_i();
}

inline void OpenCVCalibWindow::cb_Button_SaveCamera_i()
{
	Window_Save_Camera->show();
}





