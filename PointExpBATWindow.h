#pragma once

#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Float_Input.H>
#include "Image.h"
#include "ImagePoint.h"

class PointExpBATWindow : public Fl_Window
{
public:
	PointExpBATWindow(int w, int h, const char* title);
	~PointExpBATWindow(void);
	void * win; // Wskaznik do okna glownego
	Fl_Multi_Browser *Browser_Images;
	Fl_Button *Button_Export;
	Fl_Round_Button *RB_Points;
	Fl_Round_Button *RB_Lines;
	Fl_Input *Input_File;
	Fl_Float_Input *Input_Biasx;
	Fl_Float_Input *Input_Biasy;

private:
	static void cb_Button_Export(Fl_Widget *w, void *v);
	inline void cb_Button_Export_i();

	static void cb_RB_Points(Fl_Widget *w, void *v);
	inline void cb_RB_Points_i();

	static void cb_RB_Lines(Fl_Widget *w, void *v);
	inline void cb_RB_Lines_i();
};