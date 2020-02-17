#include "pch.h"
#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		menu = new Fl_Menu_Bar(0,0,600,30);
		menu->add("Project/New", FL_CTRL+'n', cb_MenuProjectNew, this );
		menu->add("Project/Open", FL_CTRL+'o', cb_MenuProjectOpen,this);
		menu->add("Project/Edit", NULL, cb_MenuProjectEdit,this);
		menu->add("Images/Import", NULL, cb_MenuImagesImport,this);
		menu->add("Images/Display 1", FL_CTRL+'1',cb_MenuImagesDisplayStereo,this);
		menu->add("Images/Display 2", FL_CTRL+'2',cb_MenuImagesDisplayStereo_2,this);
		menu->add("Images/Change Camera", NULL, cb_MenuImagesCamera,this);
		menu->add("Images/Export EO",NULL, cb_MenuImagesExportEO,this);

		menu->add("Points/GCP/Import",NULL,cb_MenuPointsImportGCP,this);
		menu->add("Points/GCP/Deffine Chessboard", NULL, cb_MenuDeffineChessboard, this);
		menu->add("Points/Import Image Points",NULL, cb_MenuPointsImportImPoints,this);
		menu->add("Points/Export Image Points/BAT",NULL, cb_MenuPointsExportBAT,this);
		menu->add("Points/Check Redundant Points",NULL, cb_MenuPointsCheckRedundant,this);

		menu->add("Points/Delete",NULL,cb_MenuPointsDelete,this);

		menu->add("Tools/Detect Chessboard Corners", NULL, cb_MenuToolsChessboardDetect, this);

		menu->add("Solve/Camera Calibration", NULL, cb_MenuSolveOpenCVCalibration, this);
		menu->add("Solve/3D Transform", NULL, cb_MenuSolve3DTransf, this);

		menu->add("Settings/Graphics", NULL, cb_MenuSettingsGraphics,this);

		menu->add("Settings/Stereo Mode", NULL, cb_MenuSettingsStereo,this);
		menu->add("Help/About BUNDLAB",NULL,cb_MenuHelpAbout,this);


		OknoNowyProjekt = new ProjectNewWindow(450,233,"New Project");
		OknoOtworzProjekt = new ProjectOpenWindow(370,500,"Open Project");
		OknoEdycjiProjektu = new ProjectEditWindow(410,100,"Edit Project");
		OknoImportObrazow = new ImagesImportWindow(430,460,"Import Images");
		OknoZmianyKamery = new ImageCameraWindow(540,500,"Change Camera");
		OknoWyswietlaniaObrazow = new ImagesShowWindow(1,0,0,800,560,"Images Display 1");
		OknoWyswietlaniaObrazow_2 = new ImagesShowWindow(2,0,0,800,560,"Images Display 2");
		OknoImportuPunktow = new PointImportWindow(366,250,"Import GCP");
		OknoDefinicjiSzachownicy = new ChessboardDeffineWindow(340, 270, "Deffine Chessboard");
		OknoImportuImPoint = new ImPointImportWindow(366,259,"Import Image Points");
		OknoUsuwaniaPunktow = new PointDeleteWindow(400, 450,"Delete Points");
		OknoExportuBAT = new PointExpBATWindow(400,450,"Export Image Points - BAT");
		OknoKalibOpenCV = new OpenCVCalibWindow(700, 600, "Camera Calibration");
		OknoTransformacji3D = new Transf3DWindow(420, 225, "3D Transform of Point Set");
		OknoImportuEO = new EoImportWindow(360, 520,"Import EO");
		OknoExportuEO = new EoExportWindow(400, 450,"Export EO");
		OknoGrafiki = new GraphicsWindow(500, 450,"Graphics");
		OknoUstawien3D = new StereoModeWindow(160,150,"Stereo Mode");
		OknoChessboardDetect = new ChessboardDetectWindow( "Detect Chessboard Corners");
		OknoPomocy = new HelpWindow(280,230,"About IMACCU");
		LicenceWindow = new WarningWindow(180,120,"Error","License not found!");
		RedundantPointsWindow = new WarningWindow(180,120,"Done!","See REP/rpoints.txt");
		SinglePointsWindow = new WarningWindow(180,120,"Done!","See REP/spoints.txt");
		DeleteSinglePointsWindow = new WarningWindow(180,120,"Done!","Points deleted!");
	end();

	deactivateItem(menu,"Project/Edit");
	deactivateItem(menu,"Images");
	deactivateItem(menu,"Points");
	deactivateItem(menu,"Settings");
	deactivateItem(menu,"Solve/Camera Calibration");
	deactivateItem(menu,"Tools");
	OknoNowyProjekt->win = (void*)this; //Przekazanie swojego adresu do okienka tworzenia nowego projektu
	OknoOtworzProjekt->win = (void*)this;
	OknoEdycjiProjektu->win = (void*)this;
	OknoImportObrazow->win = (void*)this;
	OknoZmianyKamery->win = (void*)this;
	OknoWyswietlaniaObrazow->win = (void*)this;
	OknoWyswietlaniaObrazow_2->win = (void*)this;
	OknoImportuPunktow->win =(void*)this;
	OknoDefinicjiSzachownicy->win = (void*)this;
	OknoImportuImPoint->win = (void*)this;
	OknoExportuBAT->win = (void*)this;
	OknoUsuwaniaPunktow->win = (void*)this;
	OknoExportuEO->win = (void*)this;
	OknoImportuEO->win = (void*)this;
	OknoKalibOpenCV->win = (void*)this;
	OknoGrafiki->win = (void*)this;
	OknoUstawien3D->win = (void*)this;
	OknoChessboardDetect->win = (void*)this;

	//cout<<"test: "<<OknoBloku->win<<endl;

	Version = this->label();
	NGCP = 0;
	ActiveDrawing = "";
	ActiveDrawingNum = -1;

}

MainWindow::~MainWindow()
{
	ImagePoint::StrumienDoZapisu_L.close(); //Zamykamy strumien do pliku z pomiarami na zdjeciach
	ImagePoint::StrumienDoZapisu_P.close();
	ImagePoint::StrumienDoOdczytu_L.close();
	ImagePoint::StrumienDoOdczytu_P.close();
}

//Project / New
void MainWindow:: cb_MenuProjectNew(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuProjectNew_i();
}

void MainWindow::cb_MenuProjectNew_i()
{
	OknoNowyProjekt->show();
}

//Project / Open
void MainWindow:: cb_MenuProjectOpen(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuProjectOpen_i();
}

void MainWindow::cb_MenuProjectOpen_i()
{

	OknoOtworzProjekt->Browser_Projects->clear();
	system("dir PRJ > PRJ//projects.txt");
	fillbrowserfs("PRJ", ".prj", OknoOtworzProjekt->Browser_Projects);

    //misProt *prot= new misProt("fotogrametria.agh.edu.pl","1234567890");
    OknoOtworzProjekt->show();
//    if (!prot->misCKL())
//    {
//    	OknoOtworzProjekt->show();
//    }
//    else
//    {
//        //cout << endl << "LICENSE BAD, EXIT" << endl;
//    	OknoOtworzProjekt->show(); //ZMIENIC
//        LicenceWindow->show();
//    }

}

//Project / Edit
void MainWindow::cb_MenuProjectEdit(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuProjectEdit_i();
}

void MainWindow::cb_MenuProjectEdit_i()
{
	OknoEdycjiProjektu->show();
}

//Images / Import
void MainWindow:: cb_MenuImagesImport(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuImagesImport_i();
}

void MainWindow::cb_MenuImagesImport_i()
{
	
	fstream PRJ_STREAM;
	string str1 = "PRJ\\" + CurrentProjectName;
	PRJ_STREAM.open(str1.c_str());
	string SciezkaDoObrazow;
	getline(PRJ_STREAM,SciezkaDoObrazow);
	//cout << "SicezkaDoObrazow: " << SciezkaDoObrazow << endl;
	PRJ_STREAM.close();
	
	string Polecenie = "dir " + SciezkaDoObrazow + " > " + ProjectFolder + "images.txt";
	system(Polecenie.c_str());

	system("dir CAM > CAM//cameras.txt"); //Wypelniamy liste kamer
	OknoImportObrazow->Browser_Images->clear();
	if (string(OknoImportObrazow->Input_Camera->value()) == "")
	{
		OknoImportObrazow->Button_Import->deactivate();
	}
	fillbrowserfs(SciezkaDoObrazow, ".jpg.tif.png.bmp.JPG.TIF.PNG.BMP", OknoImportObrazow->Browser_Images);
	fillinputfs("CAM",".cam", OknoImportObrazow->Input_Camera);

	OknoImportObrazow->show();
}

void MainWindow:: cb_MenuImagesCamera(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuImagesCamera_i();
}

void MainWindow::cb_MenuImagesCamera_i()
{
	OknoZmianyKamery->show();
	string s2("prj.txt");
	string s3 = ProjectFolder  + s2;
	//cout<<"s3: " << s3 <<endl;
	system("dir CAM > CAM//cameras.txt"); //Wypelniamy liste kamer
	OknoZmianyKamery->Browser_Images->clear();
	OknoZmianyKamery->Button_Change->deactivate();
	fillbrowser(s3.c_str(), ".ida", OknoZmianyKamery->Browser_Images);
	fillinput("CAM\\cameras.txt",".cam", OknoZmianyKamery->Input_Camera);
}

//Images / Display Stereo

void MainWindow::cb_MenuImagesDisplayStereo(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuImagesDisplayStereo_i();
}

void MainWindow::cb_MenuImagesDisplayStereo_i()
{
	//cvReleaseImage(& (OknoWyswietlaniaObrazow->StereoWindow->ImageL));
	//cvReleaseImage(& (OknoWyswietlaniaObrazow->StereoWindow->ImageR));
	//OknoWyswietlaniaObrazow->show();
	//OknoWyswietlaniaObrazow->fullscreen();
	string PrjName = CurrentProjectName.substr(0,CurrentProjectName.length() - 4);
	string s1("\\prj.txt");
	string s2("PRJ\\");
	string s3 = s2 + PrjName + s1;
	OknoWyswietlaniaObrazow->InputObrazLewy->clear();
	OknoWyswietlaniaObrazow->InputObrazPrawy->clear();
	fillinput(s3.c_str(),".ida",OknoWyswietlaniaObrazow->InputObrazLewy);
	fillinput(s3.c_str(),".ida",OknoWyswietlaniaObrazow->InputObrazPrawy);
	fillvector(s3.c_str(),".ida",&(OknoWyswietlaniaObrazow->ImageNames));

	OknoWyswietlaniaObrazow->InputObrazLewy->value("");
	OknoWyswietlaniaObrazow->InputObrazPrawy->value("");

	//Dolaczenie nazwy rysunku
	string Lab1 = "Images Display 1   " + ActiveDrawing;
	OknoWyswietlaniaObrazow->label(Lab1.c_str());

	OknoWyswietlaniaObrazow->show();
}

void MainWindow::cb_MenuImagesDisplayStereo_2(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuImagesDisplayStereo_2_i();
}

void MainWindow::cb_MenuImagesDisplayStereo_2_i()
{
	//cvReleaseImage(& (OknoWyswietlaniaObrazow_2->StereoWindow->ImageL));
	//cvReleaseImage(& (OknoWyswietlaniaObrazow_2->StereoWindow->ImageR));
	OknoWyswietlaniaObrazow_2->show();
	//OknoWyswietlaniaObrazow->fullscreen();
	string PrjName = CurrentProjectName.substr(0,CurrentProjectName.length() - 4);
	string s1("\\prj.txt");
	string s2("PRJ\\");
	string s3 = s2 + PrjName + s1;
	OknoWyswietlaniaObrazow_2->InputObrazLewy->clear();
	OknoWyswietlaniaObrazow_2->InputObrazPrawy->clear();
	fillinput(s3.c_str(),".ida",OknoWyswietlaniaObrazow_2->InputObrazLewy);
	fillinput(s3.c_str(),".ida",OknoWyswietlaniaObrazow_2->InputObrazPrawy);
	OknoWyswietlaniaObrazow_2->InputObrazLewy->value("");
	OknoWyswietlaniaObrazow_2->InputObrazPrawy->value("");
	fillvector(s3.c_str(),".ida",&(OknoWyswietlaniaObrazow_2->ImageNames));

	//Dolaczenie nazwy rysunku
	string Lab2 = "Images Display 2   " + ActiveDrawing;
	OknoWyswietlaniaObrazow_2->label(Lab2.c_str());
}


void MainWindow::cb_MenuPointsImportGCP(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuPointsImportGCP_i();
}

void MainWindow::cb_MenuPointsImportGCP_i()
{
	OknoImportuPunktow->show();
}

void MainWindow::cb_MenuDeffineChessboard(Fl_Widget * w, void * v)
{
	((MainWindow*)v)->cb_MenuDeffineChessboard_i();
}

inline void MainWindow::cb_MenuDeffineChessboard_i()
{
	OknoDefinicjiSzachownicy->show();
}

void MainWindow::cb_MenuPointsImportImPoints(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuPointsImportImPoints_i();
}

void MainWindow::cb_MenuPointsImportImPoints_i()
{
	OknoImportuImPoint->show();
}

void MainWindow::cb_MenuPointsCheckRedundant(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuPointsCheckRedundant_i();
}

void MainWindow::cb_MenuPointsCheckRedundant_i()
{
	vector <string> PixFileNames;
	string PrjFileName = ProjectFolder + "prj.txt";
	getfilenames(PrjFileName.c_str(),&PixFileNames,".pix");
	ofstream REPSTR;
	string repstring = ProjectFolder+"REP\\rpoints.txt";
	REPSTR.open(repstring.c_str());
	for (int i=0; i<PixFileNames.size(); i++)
	{
		REPSTR<<PixFileNames[i]<<":"<<endl;
		PixFileNames[i] = ProjectFolder + PixFileNames[i];
		string Items[9];
		ifstream PIXSTR;
		vector <string> PNames;

		PIXSTR.open(PixFileNames[i].c_str());
		while (PIXSTR)
		{
			for (int j=0; j<9; j++) PIXSTR >> Items[j];
			if (PIXSTR) PNames.push_back(Items[0]);
		}
		PIXSTR.close();

		vector <short> HM; //How many
		vector <bool> Checked;
		vector <string> Result;
		vector <short> Ile;
		HM.resize(PNames.size());
		Checked.resize(PNames.size());
		for (int j=0; j<HM.size();j++) HM[j]=0;
		for (int j=0; j<Checked.size(); j++) Checked[j]=false;

		for (int j=0; j<PNames.size();j++)
		{
			if (Checked[j]==false)
			{
				for (int k=j; k<PNames.size();k++)
				{
					if (PNames[k]==PNames[j])
					{
						Checked[k]=true;
						HM[j]=HM[j]+1;
					}
				}
				Result.push_back(PNames[j]);
				Ile.push_back(HM[j]);
			}
		}

		bool isok = true;
		for (int j=0; j<Result.size(); j++)
		{
			if (Ile[j]>1)
			{
				REPSTR<<Result[j]<<" "<<Ile[j]<<endl;
				isok = false;
			}
		}
		if (isok == true) REPSTR<<"OK"<<endl;
		REPSTR<<endl;
	}
	REPSTR.close();
	RedundantPointsWindow->show();
}

void MainWindow::cb_MenuPointsDelete(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuPointsDelete_i();
}

void MainWindow::cb_MenuPointsDelete_i()
{
	OknoUsuwaniaPunktow->show();
	OknoUsuwaniaPunktow->Browser_Images->clear();
	string s1 = ProjectFolder + "\\prj.txt";
	fillbrowser(s1.c_str(), ".ida", OknoUsuwaniaPunktow->Browser_Images);
}

void MainWindow::cb_MenuPointsExportBAT(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuPointsExportBAT_i();
}

void MainWindow::cb_MenuPointsExportBAT_i()
{
	OknoExportuBAT->show();
	OknoExportuBAT->Browser_Images->clear();
	string s1 = ProjectFolder + "\prj.txt";
	fillbrowser(s1.c_str(), ".ida", OknoExportuBAT->Browser_Images);
}

void MainWindow::cb_MenuSolveOpenCVCalibration(Fl_Widget * w, void * v)
{
	((MainWindow*)v)->cb_MenuSolveOpenCVCalibration_i();
}

inline void MainWindow::cb_MenuSolveOpenCVCalibration_i()
{
	OknoKalibOpenCV->show();
	OknoKalibOpenCV->Button_SaveCamera->deactivate();

	string s = ProjectFolder + "//prj.txt";

	OknoKalibOpenCV->Browser_Images->clear();
	OknoKalibOpenCV->Browser_Control_Points->clear();
	OknoKalibOpenCV->Browser_Results->clear();

	fillbrowser(s.c_str(), ".ida", OknoKalibOpenCV->Browser_Images);

	for (int i = 1; i <= OknoKalibOpenCV->Browser_Images->size(); i++)
	{
		string idaname = std::string(OknoKalibOpenCV->Browser_Images->text(i));
		string pixstr = idaname.substr(0, idaname.length() - 4);
		pixstr =  ProjectFolder +  pixstr + ".pix";

		int ngcp = 0;
		std::ifstream STR;
		STR.open(pixstr);
		string pointname, imagename, linename;
		float x, y;
		float mx, my;
		int type, usage;
		while (STR)
		{
			STR >> pointname >> imagename >> linename >> x >> y >> mx >> my >> type >> usage;
			if (STR)
			{
				if (type == 3) ngcp++;
			}
		}

		OknoKalibOpenCV->Browser_Control_Points->add( num2str(ngcp).c_str());

		STR.close();
		
		
	}
}

void MainWindow::cb_MenuSolve3DTransf(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuSolve3DTransf_i();
}

void MainWindow::cb_MenuSolve3DTransf_i()
{
	OknoTransformacji3D->show();
}

void MainWindow::cb_MenuToolsChessboardDetect(Fl_Widget * w, void * v)
{
	((MainWindow*)v)->cb_MenuToolsChessboardDetect_i();
}

inline void MainWindow::cb_MenuToolsChessboardDetect_i()
{
	OknoChessboardDetect->Browser_Images->clear();
	OknoChessboardDetect->Browser_Control_Points->clear();

	fillbrowserfs(ProjectFolder, std::move(std::string(".ida")), OknoChessboardDetect->Browser_Images);
	OknoChessboardDetect->show();
}


void MainWindow::cb_MenuImagesExportEO(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuImagesExportEO_i();
}

void MainWindow::cb_MenuImagesExportEO_i()
{

	OknoExportuEO->Browser_Images->clear();
	string s1 = ProjectFolder + "\\prj.txt";
	fillbrowser(s1.c_str(), ".ida", OknoExportuEO->Browser_Images);	
	OknoExportuEO->show();
}

void MainWindow::cb_MenuSettingsGraphics(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuSettingsGraphics_i();
}

void MainWindow::cb_MenuSettingsGraphics_i()
{
	OknoGrafiki->show();
}

void MainWindow::cb_MenuSettingsStereo(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuSettingsStereo_i();
}

void MainWindow::cb_MenuSettingsStereo_i()
{
	OknoUstawien3D->show();
}


void MainWindow::cb_MenuHelpAbout(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuHelpAbout_i();
}

void MainWindow::cb_MenuHelpAbout_i()
{
	OknoPomocy->show();
}

void MainWindow::cb_MenuHelpLicense(Fl_Widget *w, void *v)
{
	((MainWindow*)v)->cb_MenuHelpLicense_i();
}

void MainWindow::cb_MenuHelpLicense_i()
{
	system("notepad license.txt");
}
