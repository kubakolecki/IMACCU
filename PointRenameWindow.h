#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>

class PointRenameWindow : public Fl_Window
{
public:

	PointRenameWindow(int w, int h, const char* title);
	~PointRenameWindow();
	
	Fl_Output* Output_Point_Name_Old;
	Fl_Output* Output_Point_Type_Old;
	Fl_Input* Input_Point_Name_New;
	Fl_Input* Output_Point_Type_New;
	Fl_Button* Button_Ok;
	Fl_Button* Button_Cancel;

	void * win; //Pointer to parent window

	int Idx_Point_To_Rename = 0; //index punktu ktorego nazwe trzeba zmienic
	bool IsGCP = false;


private:


	static void cb_Button_Cancel(Fl_Widget *w, void *v);
	inline void cb_Button_Cancel_i();

	static void cb_Button_Ok(Fl_Widget *w, void *v);
	inline void cb_Button_Ok_i();

	static void cb_Button_Input_Point_Name_New(Fl_Widget *w, void *v);
	inline void cb_Button_Input_Point_Name_New_i();

	int handle(int e);

};

