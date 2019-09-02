#include "pch.h"
#include "PointExpBATWindow.h"
#include "MainWindow.h"
using namespace std;

PointExpBATWindow::PointExpBATWindow(int w, int h, const char* title) : Fl_Window(w,h,title)
{
	begin();
		Browser_Images = new Fl_Multi_Browser(20,15,150,400,"Images");
		Input_File = new Fl_Input(270,15,120,22,"File Name");
		Button_Export = new Fl_Button(270,390,80,25,"Export");
		RB_Points = new Fl_Round_Button(270,45,120,22,"Points");
		RB_Lines = new Fl_Round_Button(270,70,120,22,"Lines");
		Input_Biasx = new Fl_Float_Input(270,105,62,22,"x bias:");
		Input_Biasy = new Fl_Float_Input(270,130,62,22,"y bias:");
	end();
	Button_Export->callback(cb_Button_Export,this);
	RB_Points->value(1);
	RB_Lines->value(0);
	RB_Points->callback(cb_RB_Points,this);
	RB_Lines->callback(cb_RB_Lines,this);
	Input_Biasx->value("0.0");
	Input_Biasy->value("0.0");
}

PointExpBATWindow::~PointExpBATWindow(void)
{

}

void PointExpBATWindow::cb_Button_Export (Fl_Widget *w, void *v)
{
	((PointExpBATWindow*)v)->cb_Button_Export_i();
}

void PointExpBATWindow::cb_Button_Export_i()
{
	string FileName = Input_File->value();
	string FullFileName = ((MainWindow*)win)->ProjectFolder  + FileName;
	cout <<"FullFileName: " <<FullFileName <<endl;
	string Biasx = Input_Biasx->value();
	string Biasy = Input_Biasy->value();
	istringstream StrBiasx(Biasx);
	istringstream StrBiasy(Biasy);
	float Bx, By;
	StrBiasx >> Bx;
	StrBiasy >> By;
	cout <<"Bx: " << Bx <<" By: " <<By <<endl;
	if (FileName != "") //Jezeli jest wpisana jakas nazwa pliku
	{
		ofstream Str2File;
		Str2File.open(FullFileName.c_str());
		for (int i=1; i<=Browser_Images->size(); i++)
		{
			if (Browser_Images->selected(i))
			{
				string BrowserStr = Browser_Images->text(i);
				string IdaImageName = ((MainWindow*)win)->ProjectFolder +  BrowserStr;
				string PixImageName = ((MainWindow*)win)->ProjectFolder +  BrowserStr.substr(0,BrowserStr.length()-4) + ".pix";
				cout <<"PixImageName: " <<PixImageName  <<endl;
				Image Obraz;
				ImagePoint P;
				Obraz.ReadFromFile(IdaImageName.c_str());
				if (RB_Points->value()==1)
				{
					if (Obraz.NPoints > 0)
					{
						Str2File << BrowserStr.substr(0,BrowserStr.length()-4) <<endl; //Wpisz numer zdjecia
					}
					for (int j=0; j< Obraz.NPoints; j++)
					{	
						P.load(PixImageName.c_str(),j+1);
						if (P.Type <=4)
						{
							Str2File << P.Name <<" "<< fixed << setprecision (2) << P.X + Bx <<" "<<P.Y + By <<" "<<"1"<<endl;
						}
					}
				}
				if (RB_Lines->value()==1)
				{
					if (Obraz.NPoints > 0)
					{
						for (int j=0; j< Obraz.NPoints; j++)
						{
							P.load(PixImageName.c_str(),j+1);
							if ((P.Type >=4) && (P.Type <=7))
							{
								Str2File << P.Name << " " <<P.LineId <<" "<<P.Type <<" "<< P.ImageId <<" "<<P.X + Bx <<" "<<P.Y + By <<" 1"<<endl;
							}
						}
					}
				}
			}
		}
		Str2File.close();
	}
}

void PointExpBATWindow::cb_RB_Points(Fl_Widget *w, void *v)
{
	((PointExpBATWindow*)v)->cb_RB_Points_i();
}

void PointExpBATWindow::cb_RB_Points_i()
{
	if (RB_Points->value()==1)
	{
		RB_Lines->value(0);
	}
	RB_Points->value(1);
}

void PointExpBATWindow::cb_RB_Lines(Fl_Widget *w, void *v)
{
	((PointExpBATWindow*)v)->cb_RB_Lines_i();
}

void PointExpBATWindow::cb_RB_Lines_i()
{
	if (RB_Lines->value()==1)
	{
		RB_Points->value(0);
	}
	RB_Lines->value(1);
}