
#include "pch.h"
#include "PointDeleteWindow.h"


PointDeleteWindow::PointDeleteWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		Browser_Images = new Fl_Multi_Browser(20,15,150,400,"Images");
		Button_Delete = new Fl_Button(270,390,80,25,"Delete");
	end();
	Button_Delete->callback(cb_Button_Delete,this);

}

PointDeleteWindow::~PointDeleteWindow()
{
	// TODO Auto-generated destructor stub
}

void PointDeleteWindow::cb_Button_Delete(Fl_Widget *w, void *v)
{
	((PointDeleteWindow*)v)->cb_Button_Delete_i();
}

void PointDeleteWindow::cb_Button_Delete_i()
{
	string FolderName = ((MainWindow*)win)->ProjectFolder;
	for (int i=1; i<=Browser_Images->size(); i++)
	{
		if (Browser_Images->selected(i))
		{
			string IDA = FolderName + Browser_Images->text(i);
			string PIX = IDA.substr(0, IDA.length() - 4) + ".pix";
			Image Obraz;
			Obraz.ReadFromFile(IDA.c_str());
			Obraz.NPoints = 0;
			Obraz.WriteToFile(IDA.c_str());

			//Czyscimy plik pix
			ofstream PIX_STR;
			PIX_STR.open(PIX.c_str());
			PIX_STR.close();

		}

	}


	//przeladowanie obrazow:
	if (((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->LoadedL)
	{
		((MainWindow*)win)->OknoWyswietlaniaObrazow->InputObrazLewy->do_callback();
	}
	if (((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->LoadedR)
	{
		((MainWindow*)win)->OknoWyswietlaniaObrazow->InputObrazPrawy->do_callback();
	}
	if (((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->LoadedL)
	{
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->InputObrazLewy->do_callback();
	}
	if (((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->LoadedR)
	{
		((MainWindow*)win)->OknoWyswietlaniaObrazow_2->InputObrazPrawy->do_callback();
	}


}
