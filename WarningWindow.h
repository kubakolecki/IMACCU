#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

class WarningWindow : public Fl_Window
{
public:
	Fl_Button * Button_Ok;
	Fl_Box * Box_Message;
	WarningWindow(int w, int h, const char * title, const char * message);
	~WarningWindow(void);
private:
	static void cb_Button_Ok(Fl_Widget *w, void *v);
	inline void cb_Button_Ok_i();
};
