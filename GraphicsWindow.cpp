#include "pch.h"
#include "GraphicsWindow.h"
#include "Graphics.h"

GraphicsWindow::GraphicsWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		ChB_Circle = new Fl_Check_Button(15,15,200,22,"Circular cursor");
		ChB_ELine = new Fl_Check_Button(260,15,200,22,"Draw epipolar lines");	
		Input_Radius = new Fl_Int_Input(210,15,22,22,"Radius");
		Input_Cross = new Fl_Int_Input(210,45,22,22,"Cross size");
		Color_Cursor = new Fl_Color_Chooser(15,90,220,100,"Cursor color");
		Output_Color_Cursor = new Fl_Output(15,200,30,22);
		Color_Epipolar = new Fl_Color_Chooser(260,90,220,100,"Epipolar line color");
		Output_Color_Epipolar = new Fl_Output(260,200,30,22);
		Input_Post_Size = new Fl_Float_Input(80,250,26,22,"Post size");
		ChB_DrawPoints = new Fl_Check_Button(80,290,22,22,"Display points");
		Button_Apply = new Fl_Button(260,300,100,30,"Apply");
	end();
	Input_Radius->value("6");
	Input_Cross->value("4");
	Input_Post_Size->value("3");
	ChB_Circle->value(0);
	ChB_Circle->callback(cb_ChB_Circle,this);
	ChB_DrawPoints->value(1);
	Color_Cursor->callback(cb_Color_Cursor,this);
	Color_Epipolar->callback(cb_Color_Epipolar,this);
	Button_Apply->callback(cb_Button_Apply,this);
	Input_Radius->deactivate();
	Color_Cursor->rgb(1.0,0.0,0.0);
	Color_Epipolar->rgb(1.0,1.0,0.0);
	Output_Color_Cursor->color(fl_rgb_color(255,0.0,0.0));
	Output_Color_Epipolar->color(fl_rgb_color(255,255,0.0));

}

GraphicsWindow::~GraphicsWindow(void)
{
}

void GraphicsWindow::cb_ChB_Circle(Fl_Widget *w, void *v)
{	
	((GraphicsWindow*)v)->cb_ChB_Circle_i();
}
void GraphicsWindow::cb_ChB_Circle_i()
{
	if (ChB_Circle->value() == 0)
			Input_Radius->deactivate();
	else
			Input_Radius->activate();
}

void GraphicsWindow::cb_Color_Cursor(Fl_Widget *w, void *v)
{	
	((GraphicsWindow*)v)->cb_Color_Cursor_i();
}
void GraphicsWindow::cb_Color_Cursor_i()
{
	
	uchar r = Color_Cursor->r()*255;
	uchar g = Color_Cursor->g()*255;
	uchar b = Color_Cursor->b()*255;
	Output_Color_Cursor->color(fl_rgb_color(r,g,b));
	Output_Color_Cursor->redraw();
}

void GraphicsWindow::cb_Color_Epipolar(Fl_Widget *w, void *v)
{	
	((GraphicsWindow*)v)->cb_Color_Epipolar_i();
}
void GraphicsWindow::cb_Color_Epipolar_i()
{
	
	uchar r = Color_Epipolar->r()*255;
	uchar g = Color_Epipolar->g()*255;
	uchar b = Color_Epipolar->b()*255;
	Output_Color_Epipolar->color(fl_rgb_color(r,g,b));
	Output_Color_Epipolar->redraw();
}	

void GraphicsWindow::cb_Button_Apply(Fl_Widget *w, void *v)
{	
	((GraphicsWindow*)v)->cb_Button_Apply_i();
}
void GraphicsWindow::cb_Button_Apply_i()
{
	stringstream ss;
	string temp;
	int iLiczba;
	float fLiczba;
	Graphics G(((MainWindow*)win)->ProjectFolder);
	if (ChB_Circle->value() == 1) G.Circular_Cursor = true; else G.Circular_Cursor = false;
	if (ChB_ELine->value() == 1) G.Draw_Epipolar = true; else G.Draw_Epipolar = false;
	
	//Wczytanie wartosci pormienia okregu
	temp = Input_Radius->value();
	ss << temp;
	ss >> iLiczba;
	G.Circle_Radius = iLiczba;
	ss.clear();

	//Wczytanie wartosci wymiaru krzyzyka
	temp = Input_Cross->value();
	ss << temp;
	ss >> iLiczba;
	G.Cross_Size = iLiczba;
	ss.clear();

	//Wczytanie koloru kursora
	G.Cursor_Color[0] = Color_Cursor->r();
	G.Cursor_Color[1] = Color_Cursor->g();
	G.Cursor_Color[2] = Color_Cursor->b();

	//Wczytanie koloru linii epipolarnej
	G.Epipolar_Color[0] = Color_Epipolar->r();
	G.Epipolar_Color[1] = Color_Epipolar->g();
	G.Epipolar_Color[2] = Color_Epipolar->b();

	//Wczytanie wartosci wymiaru punktu
	temp = Input_Post_Size->value();
	ss << temp;
	ss >> fLiczba;
	G.Post_Size = fLiczba;
	ss.clear();

	G.write();
	((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->Grafika.read();
	((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->Grafika.read();
	((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->redraw();
	((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->redraw();
	G.wypisz();

	if (ChB_DrawPoints->value() == 1)
	{
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->DrawPoints = true;
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->DrawPoints = true;
	}
	else
	{
		((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->DrawPoints = false;
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->DrawPoints = false;
	}

}	