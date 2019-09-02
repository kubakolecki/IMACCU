#include "pch.h"
#include "StereoModeWindow.h"

StereoModeWindow::StereoModeWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		RB_Two_Views = new Fl_Round_Button(15,15,50,22,"Two Views");
		RB_Anaglyph = new Fl_Round_Button(15,45,50,22,"Anaglyph");
		RB_3D_Glasses = new Fl_Round_Button(15,75,50,22,"3D Glasses");
	end();
	RB_Two_Views->callback(cb_RB_Two_Views,this);
	RB_Anaglyph->callback(cb_RB_Anaglyph,this);
	RB_3D_Glasses->callback(cb_RB_3D_Glasses,this);
	RB_Two_Views->value(1);
	RB_Anaglyph->value(0);
	RB_3D_Glasses->value(0);
}

StereoModeWindow::~StereoModeWindow(void)
{
}

void StereoModeWindow::cb_RB_Two_Views (Fl_Widget *w, void *v)
{
	((StereoModeWindow*)v)->cb_RB_Two_Views_i();
}

void StereoModeWindow::cb_RB_Two_Views_i()
{
	if (RB_Two_Views->value() == 1)
	{
		RB_Anaglyph->value(0);
		RB_3D_Glasses->value(0);
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->StereoMode = 1;
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->StereoMode = 1;
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->Mono = false;
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->Mono = false;
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->redraw();
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->redraw();
	}
	else
	{
		RB_Two_Views->value(1);
	}
}

void StereoModeWindow::cb_RB_Anaglyph (Fl_Widget *w, void *v)
{
	((StereoModeWindow*)v)->cb_RB_Anaglyph_i();
}

void StereoModeWindow::cb_RB_Anaglyph_i()
{
	if (RB_Anaglyph->value() == 1)
	{
		RB_3D_Glasses->value(0);
		RB_Two_Views->value(0);
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->StereoMode = 2;
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->StereoMode = 2;
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->Mono = true;
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->Mono = true;
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->redraw();
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->redraw();
	}
	else
	{
		RB_Anaglyph->value(1);
	}

}


void StereoModeWindow::cb_RB_3D_Glasses(Fl_Widget *w, void *v)
{
	((StereoModeWindow*)v)->cb_RB_3D_Glasses_i();
}

void StereoModeWindow::cb_RB_3D_Glasses_i()
{
	if (RB_3D_Glasses->value() == 1)
	{
		RB_Anaglyph->value(0);
		RB_Two_Views->value(0);
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->StereoMode = 4;
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->StereoMode = 4;
	}
	else
	{
		RB_3D_Glasses->value(1);
	}
}
