#include "pch.h"
#include "ProjectEditWindow.h"

ProjectEditWindow::ProjectEditWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		Input_Folder = new Fl_Input(120,15,250,22,"Image folder");
		Button_Ok = new Fl_Button(120,50,55,33,"OK");
	end();
	Button_Ok->callback(cb_Button_Ok,this);
}

ProjectEditWindow::~ProjectEditWindow(void)
{
}

void ProjectEditWindow::cb_Button_Ok(Fl_Widget *w, void *v)
{
	((ProjectEditWindow*)v)->cb_Button_Ok_i();
}
void ProjectEditWindow::cb_Button_Ok_i()
{
	//cout << ((MainWindow*)win)->CurrentProjectName <<endl;
	string NewFolder = Input_Folder->value();	
	string PrjName = "PRJ\\" + ((MainWindow*)win)->CurrentProjectName;
	ofstream PrjFileStr;
	PrjFileStr.open(PrjName.c_str());
	PrjFileStr << NewFolder <<endl;
	PrjFileStr.close();

	PrjName = ((MainWindow*)win)->ProjectFolder + "prj.txt";
	ifstream PrjStr;
	PrjStr.open(PrjName.c_str());
	while (PrjStr)
	{
		string Linia;
		getline(PrjStr,Linia);
		int PozRozsz = Linia.find(".ida",0);
		if (PozRozsz != -1) //Znaleziono rozszerzenie .ida
		{
			int PozSpa = Linia.rfind(" ",Linia.length());//Pozcyja spacji - szukanie od konca
			string NazwaPliku = Linia.substr(PozSpa+1, PozRozsz-PozSpa+3);
			Image Obraz;
			string NazwaPlikuIda = ((MainWindow*)win)->ProjectFolder + NazwaPliku; 
			Obraz.ReadFromFile(NazwaPlikuIda.c_str());
			int PozSlesz = Obraz.ImageFileName.rfind("\\",Obraz.ImageFileName.length());
			string NazwaPlikuZdjecia = Obraz.ImageFileName.substr(PozSlesz+1, Obraz.ImageFileName.length());
			Obraz.ImageFileName = NewFolder + "\\" + NazwaPlikuZdjecia;
			Obraz.WriteToFile(NazwaPlikuIda.c_str());
		}

	}
	PrjStr.close();
	this->hide();

}
