#pragma once
#include <FL/Fl_Output.H>

class HelpWindow : public Fl_Window
{
public:
	HelpWindow(int w, int h, const char* title);
		Fl_Output * Output_Info;
	~HelpWindow(void);
};
