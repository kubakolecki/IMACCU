#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Input.H>

class ChessboardDeffineWindow : public Fl_Window
{
public:
	ChessboardDeffineWindow(int w, int h, const char* title);
	~ChessboardDeffineWindow();

	Fl_Int_Input* Input_Rows;
	Fl_Int_Input* Input_Cols;
	Fl_Float_Input* Input_Dim;
	Fl_Input* Input_Prefix;
	Fl_Button* Button_Generate;

	void *win;

private:
	static void cb_Button_Generate(Fl_Widget *w, void *v);
	inline void cb_Button_Generate_i();
};

