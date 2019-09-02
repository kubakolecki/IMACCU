#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Box.H>
#include "CameraSaveWindow.h"

class OpenCVCalibWindow : public Fl_Window
{
public:
	OpenCVCalibWindow(int w, int h, const char* title);
	~OpenCVCalibWindow();
	Fl_Button* Button_Run;
	Fl_Button* Button_SaveEO;
	Fl_Button* Button_SaveCamera;
	Fl_Check_Button * Check_Estimate_k1;
	Fl_Check_Button * Check_Estimate_k2;
	Fl_Check_Button * Check_Estimate_k3;
	Fl_Check_Button * Check_Estimate_tangential;
	Fl_Check_Button * Check_Estimate_principalpoint;
	Fl_Multi_Browser * Browser_Images;
	Fl_Multi_Browser * Browser_Control_Points;
	Fl_Multi_Browser * Browser_Results;
	Fl_Input * Input_Raport_File_Name;
	Fl_Box * Box_Save_EO;
	Fl_Check_Button * Check_EO_Approximated;
	Fl_Check_Button * Check_EO_Observed;
	Fl_Check_Button * Check_EO_Adjusted;

	WarningWindow *Window_Warnign_Not_Enough_Images;
	WarningWindow *Window_Warnign_Multiple_Cameras;
	CameraSaveWindow *Window_Save_Camera;

	Camera CalibratedCamera;
	void *win;

private:
	
	//buffers for external orientation parameters:
	std::vector<cv::Mat> CvRotationMatrices;
	std::vector<cv::Mat> CvProjectionCenters;
	//Bundlab (Euler) angles:
	std::vector<std::array<double,3> > Angles;

	//Image data:
	vector<Image> IdaData;

	int MinNumOfGCPsPerImage = 4;

	static void cb_Button_Run(Fl_Widget *w, void *v);
	inline void cb_Button_Run_i();

	static void cb_Button_SaveEO(Fl_Widget *w, void *v);
	inline void cb_Button_SaveEO_i();

	static void cb_Button_SaveCamera(Fl_Widget *w, void *v);
	inline void cb_Button_SaveCamera_i();
};