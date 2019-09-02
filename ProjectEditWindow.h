#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
class ProjectEditWindow: public Fl_Window
{
public:
	Fl_Input * Input_Folder;
	Fl_Button * Button_Ok;
	void *win;
	ProjectEditWindow(int w, int h, const char* title);
	~ProjectEditWindow(void);

private:
	static void cb_Button_Ok (Fl_Widget *w, void *v);
	inline void cb_Button_Ok_i();
};
