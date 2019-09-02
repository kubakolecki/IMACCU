#include "pch.h"
#include "HelpWindow.h"


HelpWindow::HelpWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		Output_Info = new Fl_Output(15,15,250,200);
	end();
	Output_Info->value("© Copyright Jakub Kolecki 2012");
}

HelpWindow::~HelpWindow(void)
{
}
