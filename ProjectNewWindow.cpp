#include "pch.h"
#include "ProjectNewWindow.h"
#include "MainWindow.h"
#include "Graphics.h"

using namespace std;

ProjectNewWindow::ProjectNewWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		//Tworzymy widgety
		Input_ProjectName = new Fl_Input(120,20,250,25,"Project name");
		Input_ImageFolder = new Fl_Input(120,60,250,25,"Image folder");
		Button_Select_Folder = new Fl_Button(380, 60, 25, 25, "...");
		Input_ProjectName->when(1);
		Input_ImageFolder->when(1);
		Button_Ok = new Fl_Button (120,140,55,33,"OK");
		//Button_Ok->deactivate();
		Button_Cancel = new Fl_Button (192,140,55,33,"Cancel");
		Window_Warning_Bad_Folder = new WarningWindow(240, 180, "Warning!", "Wrong folder name!");
		Chooser_Folder = new Fl_Native_File_Chooser();

	end();

	Chooser_Folder->type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);

	Button_Cancel->callback(cb_Button_Cancel,this);
	Button_Ok->callback(cb_Button_Ok,this);
	Button_Select_Folder->callback(cb_Button_Select_Folder, this);
	//Input_ProjectName->callback(cb_Input_ProjectName,this);
	//Input_ImageFolder->callback(cb_Input_ImageFolder,this);


}

ProjectNewWindow::~ProjectNewWindow()
{
}

void ProjectNewWindow::cb_Input_ProjectName (Fl_Widget *w, void *v)
{
	((ProjectNewWindow*)v)->cb_Input_ProjectName_i();
}
void ProjectNewWindow::cb_Input_ProjectName_i()
{
	//cout<<"project callback! " <<endl;
	NazwaProjektu = Input_ProjectName->value();
	if ( (NazwaProjektu != "") && (FolderObrazow != ""))
	{
		Button_Ok->activate();
	}
	else
	{
		Button_Ok->deactivate();
	}
}

void ProjectNewWindow::cb_Input_ImageFolder (Fl_Widget *w, void *v)
{
	((ProjectNewWindow*)v)->cb_Input_ImageFolder_i();
}
void ProjectNewWindow::cb_Input_ImageFolder_i()
{
	//cout<<"folder callback! " <<endl;
	FolderObrazow = Input_ImageFolder->value();
	if ( (NazwaProjektu != "") && (FolderObrazow != ""))
	{
		Button_Ok->activate();
	}
	else
	{
		Button_Ok->deactivate();
	}
}



//callback przycisku cancel
void ProjectNewWindow::cb_Button_Cancel (Fl_Widget *w, void *v)
{
	((ProjectNewWindow*)v)->cb_Button_Cancel_i();
}

void ProjectNewWindow::cb_Button_Cancel_i()
{
	this->hide();
}

void ProjectNewWindow::cb_Button_Select_Folder(Fl_Widget *w, void *v)
{
	((ProjectNewWindow*)v)->cb_Button_Select_Folder_i();
}

void ProjectNewWindow::cb_Button_Select_Folder_i()
{
	switch (Chooser_Folder->show())
	{
	case -1:
		break;
	case 1:
		break;
	default:
		string s = Chooser_Folder->filename();
		Input_ImageFolder->value(s.c_str());
		Input_ImageFolder->position(s.length() - 1);

	}
}

//callback przycisku ok
void ProjectNewWindow::cb_Button_Ok (Fl_Widget *w, void *v)
{
	((ProjectNewWindow*)v)->cb_Button_Ok_i();
}

void ProjectNewWindow::cb_Button_Ok_i()
{		
	bool is_name_valid = true;
	
	string str_folder_name(Input_ImageFolder->value());
	for (auto &s : str_folder_name)
	{
		if (s == ' ')
		{
			is_name_valid = false;
			break;
		}
	}
	
	if (is_name_valid)
	{
		NazwaProjektu = Input_ProjectName->value();
		FolderObrazow = Input_ImageFolder->value();
		{
			string s1("PRJ\\");
			string s2(".prj");
			string s3 = s1 + NazwaProjektu + s2;
			ofstream StrumienDoPrj;
			StrumienDoPrj.open(s3.c_str());
			StrumienDoPrj << FolderObrazow;
			StrumienDoPrj.close();
		}
		{
			string s1("md PRJ\\");
			string s3 = s1 + NazwaProjektu;
			system(s3.c_str());
		}

		{  //Zapisanie w pliku obrazy.txt informacji o plikach ze wskazanego katalogu
			string s1("dir ");
			string s3(" > ");
			string s4("PRJ\\");
			string s6 = s4 + NazwaProjektu;
			string s7("\\images.txt");
			string s8 = s1 + FolderObrazow + s3 + s6 + s7;
			system(s8.c_str());
		}

		{
			((MainWindow*)win)->CurrentProjectName = NazwaProjektu;
			((MainWindow*)win)->ProjectFolder = "PRJ\\" + NazwaProjektu + "\\";

			((MainWindow*)win)->ImageFolderName = FolderObrazow;
			string s1 = "  Project: ";
			string s2 = ".prj";
			((MainWindow*)win)->CurrentProjectName = NazwaProjektu + s2;
			string s3 = ((MainWindow*)win)->Version + s1 + NazwaProjektu + s2;
			((MainWindow*)win)->label(s3.c_str());
			system("dir PRJ > PRJ//projects.txt");
			Graphics G(((MainWindow*)win)->ProjectFolder);
			G.write();
			((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->Grafika.setProjectFolder(((MainWindow*)win)->ProjectFolder);
			((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->Grafika.setProjectFolder(((MainWindow*)win)->ProjectFolder);
			((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->Grafika.read();
			//((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->Grafika.wypisz();
			((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->Grafika.read();
		}

		//Robimy folder REP
		string REPstring = "md " + ((MainWindow*)win)->ProjectFolder + "\\REP";
		system(REPstring.c_str());

		//Robimy folder MOD
		string MODstring = "md " + ((MainWindow*)win)->ProjectFolder + "\\MOD";
		system(MODstring.c_str());

		//Robimy folder BLO
		string BLOstring = "md " + ((MainWindow*)win)->ProjectFolder + "\\BLO";
		system(BLOstring.c_str());

		((MainWindow*)win)->GC.clear(); //Czyscimy fotopunkty po juz otwartym projekcie

		//Uaktywniamy Itemy w menu
		activateItem(((MainWindow*)win)->menu, "Project/Edit");
		activateItem(((MainWindow*)win)->menu, "Images");
		activateItem(((MainWindow*)win)->menu, "Points");
		activateItem(((MainWindow*)win)->menu, "Solve");
		activateItem(((MainWindow*)win)->menu, "Tools");
		activateItem(((MainWindow*)win)->menu, "Settings");
		((MainWindow*)win)->redraw();

		this->hide();
	}
	else
	{
		Window_Warning_Bad_Folder->show();
		Window_Warning_Bad_Folder->set_modal();
	}
}
