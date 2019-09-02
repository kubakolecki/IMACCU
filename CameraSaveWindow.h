#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Native_File_Chooser.H>
#include "Camera.h"
class CameraSaveWindow : public Fl_Window
{
public:
	CameraSaveWindow(int w, int h, const char* title);
	~CameraSaveWindow();
	Fl_Button* Button_Save;
	Fl_Button* Button_Cancel;
	Fl_Input* Input_Name;
	Fl_Input* Input_Description;
	Fl_Native_File_Chooser* Chooser_File;
	void *win;
	
private:
	static void cb_Button_Save(Fl_Widget *w, void *v);
	inline void cb_Button_Save_i();

	static void cb_Button_Cancel(Fl_Widget *w, void *v);
	inline void cb_Button_Cancel_i();

};

