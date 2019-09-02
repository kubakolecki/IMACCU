#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Button.H>


class ProjectOpenWindow :
	public Fl_Window
{
public:
	
	Fl_Hold_Browser *Browser_Projects;
	Fl_Button * Button_Ok;
	Fl_Button * Button_Cancel;
	void *win;
	ProjectOpenWindow(int w, int h, const char* title);
	~ProjectOpenWindow(void);
private:
	static void cb_Button_Cancel (Fl_Widget *w, void *v);
	inline void cb_Button_Cancel_i();

	static void cb_Button_Ok (Fl_Widget *w, void *v);
	inline void cb_Button_Ok_i();

};
