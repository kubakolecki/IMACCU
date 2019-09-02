#include "pch.h"
#include "ImagesImportWindow.h"
#include "kFltk.h"
#include "Camera.h"
#include "Image.h"
#include "MainWindow.h"
using namespace std;

ImagesImportWindow::ImagesImportWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		Browser_Images = new Fl_Multi_Browser(40,20,150,400,"Images");
		Input_Camera = new Fl_Input_Choice(260,20,150,25,"Camera");
		Input_Orientation = new Fl_Input_Choice(260, 55, 150, 25, "Rotation");
		Button_Import = new Fl_Button(260,387,60,33,"Import");
		Button_Cancel = new Fl_Button(350,387,60,33,"Close");
	end();

	
	//callbacks:
	Button_Cancel->callback(cb_Button_Cancel,this);
	Button_Import->callback(cb_Button_Import,this);
	Input_Camera->callback(cb_Input_Camera,this);

	Input_Orientation->add("al-ni-ka");
	Input_Orientation->add("om-fi-ka");
	Input_Orientation->add("roll-pitch-yaw");

	Input_Orientation->value("al-ni-ka");


}

ImagesImportWindow::~ImagesImportWindow(void)
{

}

//callback przycisku cancel
void ImagesImportWindow::cb_Button_Cancel (Fl_Widget *w, void *v)
{
	((ImagesImportWindow*)v)->cb_Button_Cancel_i();
}

void ImagesImportWindow::cb_Button_Cancel_i()
{
	this->hide();
}


void ImagesImportWindow::cb_Input_Camera(Fl_Widget *w, void *v)
{
	((ImagesImportWindow*)v)->cb_Input_Camera_i();
}

void ImagesImportWindow::cb_Input_Camera_i()
{
	string s1(Input_Camera->value());
	string s2("");
	if (s1 == s2)
	{
		Button_Import->deactivate();
	}
	else
	{
		Button_Import->activate();
	}
}



void ImagesImportWindow::cb_Button_Import (Fl_Widget *w, void *v)
{
	((ImagesImportWindow*)v)->cb_Button_Import_i();
}

void ImagesImportWindow::cb_Button_Import_i()
{
	ifstream PlikKamery;
	string s1("CAM\\");
	string s2(Input_Camera->value());
	string s3 = s1 + s2;
	Camera cam(s3.c_str());
	if (cam.Valid)
	{
		//cam.wypisz();
		for (int i=1; i<= Browser_Images->size(); i++)
		{
			string s1("PRJ\\");
			string s2 = ((MainWindow*)win)->CurrentProjectName.substr(0, ((MainWindow*)win)->CurrentProjectName.length() - 4);
			string s3 = ("\\");
			string s4 = Browser_Images->text(i);
			string s5 = ".ida";
			string s6 = s1 + s2 + s3 + s4.substr(0, s4.length() -4) + s5;
			string s7 = ".pix";
			if (Browser_Images->selected(i))
			{
				Image I(cam);
				string s1(Browser_Images->text(i)); //Nazwa pliku obrazu
				string s2("\\");
				I.ImageFileName = ((MainWindow*)win)->ImageFolderName + s2 + s1; //Pelna sciezka dostepu do obrazu
				I.Name = s4.substr(0, s4.length() -4);
				I.AngularSystem = Input_Orientation->value();
				I.WriteToFile(s6.c_str()); //Zapisujemy dane obrazu do pliku .ida
				ofstream Str2Pix;
				string s3 = ".pix";
				string s8 = ((MainWindow*)win)->ProjectFolder + s4.substr(0, s4.length() -4) + s3;
				//cout << "Nazwa pliku .pix " << s8 <<endl;
				Str2Pix.open(s8.c_str()); //Tworzymy plik .pix
				Str2Pix.close();
			}
		}
		PlikKamery.close();
	    //Zapis zawartosci folderu PRJ do pliku
		string PrjFullName(((MainWindow*)win)->CurrentProjectName);
		string PrjName = PrjFullName.substr(0,PrjFullName.length() - 4);
		string s1("PRJ\\");
		string s2 = s1 + PrjName;
		string s3("\\prj.txt");
		string s4 = s1 + PrjName + s3;
		string s5("dir ");
		string s6(" > ");
		string s7 = s5 + s2 + s6 + s4;
		system(s7.c_str());
	}
	else
	{				
		//cout << "Brak pliku kamery: " <<s3 <<endl;
	}
}
