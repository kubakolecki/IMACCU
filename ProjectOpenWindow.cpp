#include "pch.h"
#include "ProjectOpenWindow.h"


using namespace std;
extern void fillbrowser(const char *filename, const char *mask, Fl_Browser *browser);

ProjectOpenWindow::	ProjectOpenWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	//Rozmieszczamy widgety
	begin();
		Browser_Projects = new Fl_Hold_Browser(20,30,230,400,"Projects");
		Button_Ok = new Fl_Button (270,30,60,33,"OK");
		Button_Cancel = new Fl_Button (270,397,60,33,"Cancel");
		//Przypisujemy zadania
		Button_Cancel->callback(cb_Button_Cancel,this);
		Button_Ok->callback(cb_Button_Ok,this);
	end();
}

ProjectOpenWindow::~ProjectOpenWindow(void)
{
	
}


void ProjectOpenWindow::cb_Button_Ok (Fl_Widget *w, void *v)
{
	((ProjectOpenWindow*)v)->cb_Button_Ok_i();
}

void ProjectOpenWindow::cb_Button_Ok_i()
{
	for (int i=1; i<= Browser_Projects->size(); i++)
	{
		if (Browser_Projects->selected(i))
		{
			//Aktualizujemy nazwe projektu
			//cout<<"Zaznaczono element: " <<i <<endl; 
			string NazwaProjektu(Browser_Projects->text(i));
			((MainWindow*)win)->CurrentProjectName = NazwaProjektu;
			
			//Aktualizujemy label w okienku glownym
			{
			string s1 = "  Project: "; 
			string s2 = ((MainWindow*)win)->Version + s1 + NazwaProjektu;
			((MainWindow*)win)->label(s2.c_str());
			this->hide();
			}
			//Aktualizujemy sciezke dostepu do folderu z obrazami
			//Jest ona zapisana w pliku .prj
			{
			ifstream ZrodloPRJ;
			string s1 = "PRJ\\";
			string NazwaPlikuPRJ = s1 + NazwaProjektu;
			//cout << "NazwaPlikuPRJ " << NazwaPlikuPRJ <<endl;
			ZrodloPRJ.open(NazwaPlikuPRJ.c_str());
			char ImageFolderPath[200];
			ZrodloPRJ.get(ImageFolderPath, 200, '\n');
			ZrodloPRJ.close();
			string s2(ImageFolderPath); 
			((MainWindow*)win)->ImageFolderName = s2;
			}
			//Uaktywniamy Itemy w menu
			activateItem(((MainWindow*)win)->menu,"Project/Edit");
			activateItem(((MainWindow*)win)->menu,"Images");
			activateItem(((MainWindow*)win)->menu,"Points");
			activateItem(((MainWindow*)win)->menu,"Solve/Camera Calibration");
			activateItem(((MainWindow*)win)->menu,"Settings");
			activateItem(((MainWindow*)win)->menu,"Tools");
			((MainWindow*)win)->redraw();

			//importujemy fotopunkty jezeli sa
			{
			string s1 = "PRJ\\";
			string s2 = ((MainWindow*)win)->CurrentProjectName;
			string s3 = "\\";
			string s4 = ".gcp";
			string s5 = s1 + s2.substr(0,s2.length()-4) + s3 + s2.substr(0,s2.length()-4) +s4;
			//string s6 = ".pix"; //To bylo jak pomiary na zdjeciach zapisywalo sie w 1 pliku			
			//string s7 = s1 + s2.substr(0,s2.length()-4) + s3 + s2.substr(0,s2.length()-4) +s6;

			((MainWindow*)win)->NGCP = countGCP(s5.c_str());
			((MainWindow*)win)->GC.clear();
			
			int Sukces = importGC(s5.c_str(),&((MainWindow*)win)->GC, ((MainWindow*)win)->NGCP );
			
			((MainWindow*)win)->ProjectFolder = s1 + s2.substr(0,s2.length()-4) + s3; //Folder projektu
			
			//cout << "Zaimportowano: " << ((MainWindow*)win)->NGCP << " punktow." <<endl;
			//for (int p=0; p < ((MainWindow*)win)->NGCP; p++)
			//{
			//	cout << ((MainWindow*)win)->GC[p].Name <<" " <<((MainWindow*)win)->GC[p].X <<" "<< ((MainWindow*)win)->GC[p].Y <<endl;
			//}
			string st3("\\prj.txt");
			string st4 = ((MainWindow*)win)->ProjectFolder;
			string st5("dir ");
			string st6(" > ");
			string st7 = st5 + st4 + st6 + st4 + st3;
			system(st7.c_str());
			}
			
			((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->
			Grafika.setProjectFolder( ((MainWindow*)win)->ProjectFolder );
			
			((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->
			Grafika.setProjectFolder( ((MainWindow*)win)->ProjectFolder );
			
			((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->Grafika.read();
			((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->Grafika.read();
			
			//pozyskanie nazw plikow pix i ida
			string s = ((MainWindow*)win)->ProjectFolder + "prj.txt";
			vector<string> IdaFileNames;

			getfilenames(s.c_str(), &IdaFileNames, ".ida");
			for (auto &pf : IdaFileNames)
			{
				pf = ((MainWindow*)win)->ProjectFolder + pf;
			}

		}
	}
	
	((MainWindow*)win)->OknoKalibOpenCV->Browser_Images->clear();
	((MainWindow*)win)->OknoKalibOpenCV->Browser_Control_Points->clear();
	((MainWindow*)win)->OknoKalibOpenCV->Browser_Results->clear();
	this->hide();


}


void ProjectOpenWindow::cb_Button_Cancel (Fl_Widget *w, void *v)
{
	((ProjectOpenWindow*)v)->cb_Button_Cancel_i();
}

void ProjectOpenWindow::cb_Button_Cancel_i()
{
	this->hide();
}
