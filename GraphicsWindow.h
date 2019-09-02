#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Output.H>

class GraphicsWindow: public Fl_Window
{
public:
	Fl_Check_Button* ChB_Circle;
	Fl_Int_Input* Input_Radius;
	Fl_Int_Input* Input_Cross;
	Fl_Float_Input* Input_Post_Size;
	Fl_Check_Button* ChB_DrawPoints; //Czy fotopunkty i punkty wiazace maja byc wyswietlane, nie nalezy do graphics
	Fl_Color_Chooser* Color_Cursor;
	Fl_Color_Chooser* Color_Epipolar;
	Fl_Check_Button* ChB_ELine;
	Fl_Output* Output_Color_Cursor;
	Fl_Output* Output_Color_Epipolar;
	Fl_Button* Button_Apply;
	void *win;
	GraphicsWindow(int w, int h, const char* title);
	~GraphicsWindow(void);

private:
	static void cb_ChB_Circle(Fl_Widget *w, void *v);
	inline void cb_ChB_Circle_i();

	static void cb_Color_Cursor(Fl_Widget *w, void *v);
	inline void cb_Color_Cursor_i();

	static void cb_Color_Epipolar(Fl_Widget *w, void *v);
	inline void cb_Color_Epipolar_i();

	static void cb_Button_Apply(Fl_Widget *w, void *v);
	inline void cb_Button_Apply_i();
};
