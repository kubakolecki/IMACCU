#pragma once
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Check_Button.H>
#include "Transf3DData.h"

class Transf3DWindow : public Fl_Window
{
public:
	Transf3DWindow(int w, int h, const char* title);
	~Transf3DWindow(void);

	Fl_File_Chooser* Chooser_Source;
	Fl_File_Chooser* Chooser_Target;
	Fl_File_Chooser* Chooser_Points;

	Fl_Output* Output_Source;
	Fl_Output* Output_Target;
	Fl_Output* Output_Points;

	Fl_Button* Button_Source;
	Fl_Button* Button_Target;
	Fl_Button* Button_Points;

	Fl_Input_Choice* Input_Priority;
	Fl_Check_Button* Button_Scale;

	Fl_Button* Button_Solve;
	Fl_Input* Input_Raport;

	Transf3DData TransfData; // Obiekt z danymi transformacji

private:
	static void cb_Button_Source(Fl_Widget *w, void *v);
	inline void cb_Button_Source_i();

	static void cb_Button_Target(Fl_Widget *w, void *v);
	inline void cb_Button_Target_i();

	static void cb_Button_Points(Fl_Widget *w, void *v);
	inline void cb_Button_Points_i();

	static void cb_Chooser_Source(Fl_File_Chooser *w, void *v);
	inline void cb_Chooser_Source_i();

	static void cb_Chooser_Target(Fl_File_Chooser *w, void *v);
	inline void cb_Chooser_Target_i();

	static void cb_Chooser_Points(Fl_File_Chooser *w, void *v);
	inline void cb_Chooser_Points_i();

	static void cb_Button_Solve(Fl_Widget *w, void *v);
	inline void cb_Button_Solve_i();

	static void cb_Button_Scale(Fl_Widget *w, void *v);
	inline void cb_Button_Scale_i();

	static void cb_Input_Priority(Fl_Widget *w, void *v);
	inline void cb_Input_Priority_i();


};

