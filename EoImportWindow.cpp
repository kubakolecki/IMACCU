#include "pch.h"
#include "EoImportWindow.h"
#include "Image.h"
using namespace std;

EoImportWindow::EoImportWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		Output_File = new Fl_Output(80,15,220,22,"File Name");
		Button_Browse = new Fl_Button(310,15,22,22,"...");
		Browser_Images = new Fl_Multi_Browser(20,45,150,430,"Images");
		ChB_Approximated = new Fl_Check_Button(180,45,200,22,"As Approximated");
		ChB_Observed = new Fl_Check_Button(180,75,200,22,"As Observed");
		ChB_Adjusted = new Fl_Check_Button(180,105,200,22,"As Adjusted");	
		ChB_X = new Fl_Check_Button(180,145,30,22,"X");
		ChB_Y = new Fl_Check_Button(240,145,30,22,"Y");
		ChB_Z = new Fl_Check_Button(300,145,30,22,"Z");
		ChB_Ang1 = new Fl_Check_Button(180,175,30,22,"Ang.1");
		ChB_Ang2 = new Fl_Check_Button(240,175,30,22,"Ang.2");
		ChB_Ang3 = new Fl_Check_Button(300,175,30,22,"Ang.3");
		Input_RotSystem = new Fl_Input_Choice(180,205,120,22);
		ChB_Error = new Fl_Check_Button(180,235,200,22,"Import Errors");
		Box_Alignment = new Fl_Box(180,265,120,22,"Alignment Matrix:");		
		
		Input_R0 = new Fl_Float_Input(180,295,50,22);
		Input_R1 = new Fl_Float_Input(238,295,50,22);
		Input_R2 = new Fl_Float_Input(296,295,50,22);

		Input_R3 = new Fl_Float_Input(180,325,50,22);
		Input_R4 = new Fl_Float_Input(238,325,50,22);
		Input_R5 = new Fl_Float_Input(296,325,50,22);

		Input_R6 = new Fl_Float_Input(180,355,50,22);
		Input_R7 = new Fl_Float_Input(238,355,50,22);
		Input_R8 = new Fl_Float_Input(296,355,50,22);

		Box_LeverArm= new Fl_Box(180,385,120,22,"Lever Arm:");	
		Input_lx = new Fl_Float_Input(180,415,50,22);
		Input_ly = new Fl_Float_Input(238,415,50,22);
		Input_lz = new Fl_Float_Input(296,415,50,22);

		Button_Import = new Fl_Button(180,445,120,30,"Import");

		Chooser_Plik = new Fl_File_Chooser(NULL,"*.txt",1,"Select EO File");
	end();
	ChB_Approximated->value(1);
	ChB_Observed->value(0);
	ChB_Adjusted->value(1);
	ChB_X->value(1);
	ChB_Y->value(1);
	ChB_Z->value(1);
	ChB_Ang1->value(1);
	ChB_Ang2->value(1);
	ChB_Ang3->value(1);

	Input_RotSystem->add("al-ni-ka");
	Input_RotSystem->add("om-fi-ka");
	Input_RotSystem->add("roll-pitch-yaw");
	Input_RotSystem->add("direction cosines");

	Input_RotSystem->value("al-ni-ka");

	Input_R0->value("1"); Input_R1->value("0"); Input_R2->value("0");
	Input_R3->value("0"); Input_R4->value("1"); Input_R5->value("0");
	Input_R6->value("0"); Input_R7->value("0"); Input_R8->value("1");
	Input_lx->value("0"); Input_ly->value("0"); Input_lz->value("0");
	Button_Browse->callback(cb_Button_Browse,this);
	Chooser_Plik->callback(cb_Chooser_Plik,this);
	Button_Import->callback(cb_Button_Import,this);
	Input_RotSystem->callback(cb_Input_RotSystem,this);
}

EoImportWindow::~EoImportWindow(void)
{

}

void EoImportWindow::cb_Chooser_Plik(Fl_File_Chooser *w, void *v)
{
	((EoImportWindow*)v)->cb_Chooser_Plik_i();
}

void EoImportWindow::cb_Chooser_Plik_i()
{
	Output_File->value(Chooser_Plik->value());
}

//void EoImportWindow::cb_RB_Apriori(Fl_Widget *w, void *v)
//{
//	((EoImportWindow*)v)->cb_RB_Apriori_i();
//}
//
//void EoImportWindow::cb_RB_Apriori_i()
//{	
//	if (RB_Apriori->value() == 1)	
//	RB_Aposteriori->value(0);
//	else 
//	RB_Apriori->value(1);
//}
//
//void EoImportWindow::cb_RB_Aposteriori(Fl_Widget *w, void *v)
//{
//	((EoImportWindow*)v)->cb_RB_Aposteriori_i();
//}
//
//void EoImportWindow::cb_RB_Aposteriori_i()
//{
//	if (RB_Aposteriori->value() == 1)	
//	RB_Apriori->value(0);
//	else 
//	RB_Aposteriori->value(1);
//}

void EoImportWindow::cb_Button_Browse(Fl_Widget *w, void *v)
{
	((EoImportWindow*)v)->cb_Button_Browse_i();
}

void EoImportWindow::cb_Button_Browse_i()
{
	Chooser_Plik->show();
}

void EoImportWindow::cb_Input_RotSystem(Fl_Widget *w, void *v)
{
	((EoImportWindow*)v)->cb_Input_RotSystem_i();
}

void EoImportWindow::cb_Input_RotSystem_i()
{
	string s = Input_RotSystem->value();
	if (s == "direction cosines")
	{
		ChB_Error->value(0);
		ChB_Error->deactivate();
		ChB_Ang1->deactivate();
		ChB_Ang2->deactivate();
		ChB_Ang3->deactivate();
		ChB_Ang1->redraw();
		ChB_Ang2->redraw();
		ChB_Ang3->redraw();
	}
	else
	{
		ChB_Error->value(0);
		ChB_Error->activate();
		ChB_Ang1->activate();
		ChB_Ang2->activate();
		ChB_Ang3->activate();
		ChB_Ang1->redraw();
		ChB_Ang2->redraw();
		ChB_Ang3->redraw();

	}
}

void EoImportWindow::cb_Button_Import(Fl_Widget *w, void *v)
{
	((EoImportWindow*)v)->cb_Button_Import_i();
}

void EoImportWindow::cb_Button_Import_i()
{
	ifstream STR;
	string Linia, BrowserStr, IdaImageName;
	istringstream LNSTR;
	STR.open(Output_File->value());
	Image Obraz;
	string ImName;
	while (STR) //poki plik da sie czytac
	{
		getline(STR,Linia); //Czytaj linie
		if (STR)
		{
			LNSTR.str(Linia); 
			LNSTR >> ImName; //Odczytujemy jedynie nazwe obrazu
			for (int i=1; i<=Browser_Images->size(); i++) //Dla kazdego obrazu
			{
				if (Browser_Images->selected(i)) //Jezeli obraz jest zaznaczony
				{
					string s1(Browser_Images->text(i));

					if (ImName == s1.substr(0,s1.length()-4)) //Jezeli nazwa obrazu zgadza sie z ta odczytana z pliku
					{
						//cout <<"****** Sprawdzam obraz: "<<ImName <<endl;
						double COORDS[3]; //wspolrzedne
						double ANGLES[3]; //katy
						double COOERR[3]; // bledy wsp.
						double ANGERR[3]; // bledy katow
						double ROT[9]; //MACIERZ OBROTU
						double BOR[9]; //Macierz boresight
						double ROT_NEW[9]; //nowa macierz obrotu
						double ANGLES_NEW[3]; //nowe wartosci katow
						double LEV[3]; //wektor lever arm
						double DPOS[3]; //wektor o jaki nalezy poprawic polozenie
						string RotParametrization = Input_RotSystem->value(); //Odczytujemy jaka jest sekwencja obrotow
						
						if (RotParametrization == "direction cosines")
						{
							LNSTR >> COORDS[0] >> COORDS[1] >> COORDS[2] >> ROT[0] >> ROT[1] >> ROT[2] >> ROT[3] >> ROT[4] >> ROT[5] >> ROT[6] >> ROT[7] >> ROT[8];
						}
						else
						{
							LNSTR >> COORDS[0] >> COORDS[1] >> COORDS[2] >> ANGLES[0] >> ANGLES[1] >> ANGLES[2];
							if (ChB_Error->value() == 1)
							{
								LNSTR >> COOERR[0] >> COOERR[1] >> COOERR[2] >> ANGERR[0] >> ANGERR[1] >> ANGERR[2];
							}
						}
						// ******************* WCZYTANIE WLASCIWEGO OBRAZU *********************************
						BrowserStr = Browser_Images->text(i);
						IdaImageName = ((MainWindow*)win)->ProjectFolder +  BrowserStr;
						Obraz.ReadFromFile(IdaImageName.c_str());

						//******************** PRZYGOTOWANIE KATOW *****************************************

						string BorStr[9]; //stringi z wartosciami macierzy boresight
						string LevStr[3]; //stringi z elementami wektora lever arm
						BorStr[0] = Input_R0->value(); BorStr[1] = Input_R1->value(); BorStr[2] = Input_R2->value(); 
						BorStr[3] = Input_R3->value(); BorStr[4] = Input_R4->value(); BorStr[5] = Input_R5->value(); 					
						BorStr[6] = Input_R6->value(); BorStr[7] = Input_R7->value(); BorStr[8] = Input_R8->value(); 					
						LevStr[0] = Input_lx->value(); LevStr[1] = Input_ly->value(); LevStr[2] = Input_lz->value();
						for (int bindex = 0; bindex <9; bindex++) //konwersja na typ double
						{
							BOR[bindex] = str2num_d(BorStr[bindex]);
						}
						for (int lindex = 0; lindex<3; lindex++)
						{
							LEV[lindex] = str2num_d(LevStr[lindex]);
						}

						if (RotParametrization != "direction cosines")
						{
							fT_angles2rot(ANGLES,ROT,RotParametrization);
						}
						fT_matMult3(ROT,BOR,ROT_NEW);
						fT_vectorTrans3(ROT_NEW,LEV,DPOS);
						//cout <<"BOR:"<<endl;
						//fT_dispMatrix3(BOR);
						fT_dispMatrix3(ROT);
						fT_dispMatrix3(ROT_NEW);

						string AngSeq;
						if (RotParametrization == "direction cosines")
						{
							//Jezeli mamy cosinusy kierunkowe to musimy pobrac system katowy z aktualnego obrazu i go stosowac
							AngSeq = Obraz.AngularSystem;
						}
						else
						{
							//Jezeli parametrizacja obrotu wykorzystuje katy to wtedy ona bedzie wpisana do plikow .ida
							AngSeq = RotParametrization;
						}

						fT_rot2angles(ROT_NEW,ANGLES_NEW,AngSeq);
						Obraz.AngularSystem = AngSeq;

						// ******************* USTAWIENIE WARTOSCI *****************************************					
						//Jezeli importujemy jako przybilzone

						if (ChB_Approximated->value() == 1)
						{
							if (ChB_X->value() == 0) COORDS[0] = Obraz.CoordsApr[0];
							if (ChB_Y->value() == 0) COORDS[1] = Obraz.CoordsApr[1];
							if (ChB_Z->value() == 0) COORDS[2] = Obraz.CoordsApr[2];
							if (ChB_Ang1->value() == 0) ANGLES_NEW[0] = Obraz.AnglesApr[0];
							if (ChB_Ang2->value() == 0) ANGLES_NEW[1] = Obraz.AnglesApr[1];
							if (ChB_Ang3->value() == 0) ANGLES_NEW[2] = Obraz.AnglesApr[2];
							if (ChB_Error->value() == 1)
							{
								if (ChB_X->value() == 0) COOERR[0] = Obraz.CoordsAprErr[0];
								if (ChB_Y->value() == 0) COOERR[1] = Obraz.CoordsAprErr[1];
								if (ChB_Z->value() == 0) COOERR[2] = Obraz.CoordsAprErr[2];
								if (ChB_Ang1->value() == 0) ANGERR[0] = Obraz.AnglesAprErr[0];
								if (ChB_Ang2->value() == 0) ANGERR[1] = Obraz.AnglesAprErr[1];
								if (ChB_Ang3->value() == 0) ANGERR[2] = Obraz.AnglesAprErr[2];
								Obraz.setAnglesAprErr(ANGERR);
								Obraz.setCoordsAprErr(COOERR);
							}
							COORDS[0] = COORDS[0] + DPOS[0]; //Uwzgledniamy wektor lever arm
							COORDS[1] = COORDS[1] + DPOS[1];
							COORDS[2] = COORDS[2] + DPOS[2];
							Obraz.setAnglesApr(ANGLES_NEW);
							Obraz.setCoordsApr(COORDS);
						}

						//Jezeli importujemy jako obserwowane
						if (ChB_Observed->value() == 1)
						{
							if (ChB_X->value() == 0) COORDS[0] = Obraz.CoordsObs[0];
							if (ChB_Y->value() == 0) COORDS[1] = Obraz.CoordsObs[1];
							if (ChB_Z->value() == 0) COORDS[2] = Obraz.CoordsObs[2];
							if (ChB_Ang1->value() == 0) ANGLES_NEW[0] = Obraz.AnglesObs[0];
							if (ChB_Ang2->value() == 0) ANGLES_NEW[1] = Obraz.AnglesObs[1];
							if (ChB_Ang3->value() == 0) ANGLES_NEW[2] = Obraz.AnglesObs[2];
							if (ChB_Error->value() == 1)
							{
								if (ChB_X->value() == 0) COOERR[0] = Obraz.CoordsObsErr[0];
								if (ChB_Y->value() == 0) COOERR[1] = Obraz.CoordsObsErr[1];
								if (ChB_Z->value() == 0) COOERR[2] = Obraz.CoordsObsErr[2];
								if (ChB_Ang1->value() == 0) ANGERR[0] = Obraz.AnglesObsErr[0];
								if (ChB_Ang2->value() == 0) ANGERR[1] = Obraz.AnglesObsErr[1];
								if (ChB_Ang3->value() == 0) ANGERR[2] = Obraz.AnglesObsErr[2];
								Obraz.setAnglesObsErr(ANGERR);
								Obraz.setCoordsObsErr(COOERR);
							}
							COORDS[0] = COORDS[0] + DPOS[0]; //Uwzgledniamy wektor lever arm
							COORDS[1] = COORDS[1] + DPOS[1];
							COORDS[2] = COORDS[2] + DPOS[2];
							Obraz.setAnglesObs(ANGLES_NEW);
							Obraz.setCoordsObs(COORDS);
						}

						//Jezeli importujemy jako wyrownane
						if (ChB_Adjusted->value() == 1)
						{
							if (ChB_X->value() == 0) COORDS[0] = Obraz.CoordsAdj[0];
							if (ChB_Y->value() == 0) COORDS[1] = Obraz.CoordsAdj[1];
							if (ChB_Z->value() == 0) COORDS[2] = Obraz.CoordsAdj[2];
							if (ChB_Ang1->value() == 0) ANGLES_NEW[0] = Obraz.AnglesAdj[0];
							if (ChB_Ang2->value() == 0) ANGLES_NEW[1] = Obraz.AnglesAdj[1];
							if (ChB_Ang3->value() == 0) ANGLES_NEW[2] = Obraz.AnglesAdj[2];
							if (ChB_Error->value() == 1)
							{
								if (ChB_X->value() == 0) COOERR[0] = Obraz.CoordsAdjErr[0];
								if (ChB_Y->value() == 0) COOERR[1] = Obraz.CoordsAdjErr[1];
								if (ChB_Z->value() == 0) COOERR[2] = Obraz.CoordsAdjErr[2];
								if (ChB_Ang1->value() == 0) ANGERR[0] = Obraz.AnglesAdjErr[0];
								if (ChB_Ang2->value() == 0) ANGERR[1] = Obraz.AnglesAdjErr[1];
								if (ChB_Ang3->value() == 0) ANGERR[2] = Obraz.AnglesAdjErr[2];
								Obraz.setAnglesAdjErr(ANGERR);
								Obraz.setCoordsAdjErr(COOERR);
							}
							COORDS[0] = COORDS[0] + DPOS[0]; //Uwzgledniamy wektor lever arm
							COORDS[1] = COORDS[1] + DPOS[1];
							COORDS[2] = COORDS[2] + DPOS[2];
							Obraz.setAnglesAdj(ANGLES_NEW);
							Obraz.setCoordsAdj(COORDS);

						}						
						Obraz.wypisz();
						Obraz.WriteToFile(IdaImageName.c_str());
					}//do if
				}//do if
			}//do for
			LNSTR.clear();
		}//do if(STR)
	}//do while
	STR.close();
}
