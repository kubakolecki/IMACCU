#include "pch.h"
#include "ImageCameraWindow.h"
#include "MainWindow.h"

using namespace std;

ImageCameraWindow::ImageCameraWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		Browser_Images = new Fl_Multi_Browser(50,30,150,400,"Images");
		Input_Camera = new Fl_Input_Choice(300,30,150,25,"Camera");
		Button_Change = new Fl_Button(300,397,60,33,"Change");
		Button_Cancel = new Fl_Button(390,397,60,33,"Cancel");
	end();
	Input_Camera->callback(cb_Input_Camera,this);
	Button_Cancel->callback(cb_Button_Cancel,this);
	Button_Change->callback(cb_Button_Change,this);
}

ImageCameraWindow::~ImageCameraWindow(void)
{

}

void ImageCameraWindow::cb_Button_Cancel (Fl_Widget *w, void *v)
{
	((ImageCameraWindow*)v)->cb_Button_Cancel_i();
}

void ImageCameraWindow::cb_Button_Cancel_i()
{
	this->hide();
}

void ImageCameraWindow::cb_Input_Camera(Fl_Widget *w, void *v)
{
	((ImageCameraWindow*)v)->cb_Input_Camera_i();
}

void ImageCameraWindow::cb_Input_Camera_i()
{
	string s1(Input_Camera->value());
	string s2("");
	if (s1 == s2)
	{
		Button_Change->deactivate();
	}
	else
	{
		Button_Change->activate();
	}
}


void ImageCameraWindow::cb_Button_Change (Fl_Widget *w, void *v)
{
	((ImageCameraWindow*)v)->cb_Button_Change_i();
}

void ImageCameraWindow::cb_Button_Change_i()
{
	string s1("CAM\\");
	string s2(Input_Camera->value());
	string s3 = s1 + s2;
	Camera cam(s3.c_str());
	if (cam.Valid)
	{
		for (int i=1; i<= Browser_Images->size(); i++)
		{
			if (Browser_Images->selected(i))
			{
				string s1 (Browser_Images->text(i));
				string s2 = ((MainWindow*)win)->ProjectFolder + s1;
				Image I;
				I.ReadFromFile(s2.c_str());
				I.setCamera(cam);
				I.CamFileName = s3;
				I.WriteToFile(s2.c_str());
			}		
		}
	}
	else
	{				
		//cout << "Brak pliku kamery: " <<s3 <<endl;
	}
}
