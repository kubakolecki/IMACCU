#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Progress.H>
#include "WarningWindow.h"
class ChessboardDetectWindow : public Fl_Window
{
public:
	ChessboardDetectWindow(const char* title);
	~ChessboardDetectWindow();
	Fl_Int_Input *Input_Rows;
	Fl_Int_Input *Input_Cols;
	Fl_Button * Button_Run;
	Fl_Multi_Browser * Browser_Images;
	Fl_Multi_Browser * Browser_Control_Points;
	WarningWindow *Window_Warning_GCP;
	Fl_Progress* Progress_Detection;

	void * win;

private:
	int Rows{ 8 };
	int Cols{ 8 };

	static void cb_Button_Run(Fl_Widget *w, void *v);
	inline void cb_Button_Run_i();
};

