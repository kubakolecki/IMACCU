#include "pch.h"
#include "WarningWindow.h"

WarningWindow::WarningWindow(int w, int h, const char* title, const char * message):Fl_Window(w,h,title)
{
	begin();
		Box_Message = new Fl_Box(15,15,140,50,message);
		Button_Ok = new Fl_Button(50,75,80,25,"OK");
	end();
	Button_Ok->callback(cb_Button_Ok,this);
}

WarningWindow::~WarningWindow(void)
{
}

void WarningWindow::cb_Button_Ok(Fl_Widget *w, void *v)
{
	((WarningWindow*)v)->cb_Button_Ok_i();
}

void WarningWindow::cb_Button_Ok_i()
{
	this->hide();
}
