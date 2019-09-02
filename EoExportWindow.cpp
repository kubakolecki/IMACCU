#include "pch.h"
#include "EoExportWindow.h"
#include "Image.h"
using namespace std;

EoExportWindow::EoExportWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		Browser_Images = new Fl_Multi_Browser(20,15,150,400,"Images");
		Input_File = new Fl_Input(270,15,120,22,"File Name");
		RB_Export_Apr= new Fl_Round_Button(270,50,100,22,"Approximated");
		RB_Export_Obs = new Fl_Round_Button(270,80,100,22,"Observed");
		RB_Export_Adj = new Fl_Round_Button(270,110,100,22,"Adjusted");
		Input_Rotation= new Fl_Input_Choice(270,140,120,25,"Rotation");
		ChB_Export_Errors = new Fl_Check_Button(270,170,100,22,"Export Errors");
		ChB_Bat_Export = new Fl_Check_Button(270,200,100,22,"Export to BAT");
		Button_Export = new Fl_Button(270,390,80,25,"Export");
	end();
	Button_Export->callback(cb_Button_Export,this);
	ChB_Export_Errors->callback(cb_ChB_Export_Errors,this);
	RB_Export_Apr->callback(cb_RB_Export_Apr,this);
	RB_Export_Obs->callback(cb_RB_Export_Obs,this);
	RB_Export_Adj->callback(cb_RB_Export_Adj,this);
	RB_Export_Apr->value(1);
	RB_Export_Obs->value(0);
	RB_Export_Adj->value(0);
	Input_Rotation->add("al-ni-ka");
	Input_Rotation->add("om-fi-ka");
	Input_Rotation->add("roll-pitch-yaw");
	Input_Rotation->add("quaternion");
	Input_Rotation->add("direction cosines");
	Input_Rotation->value("al-ni-ka");
	Input_Rotation->textsize(12);
	Input_Rotation->callback(cb_Input_Rotation,this);
	ChB_Export_Errors->value(0);
	ChB_Bat_Export->deactivate();



}

EoExportWindow::~EoExportWindow(void)
{

}

void EoExportWindow::cb_ChB_Export_Errors(Fl_Widget *w, void *v)
{
	((EoExportWindow*)v)->cb_ChB_Export_Errors_i();
}

void EoExportWindow::cb_ChB_Export_Errors_i()
{
	if (ChB_Export_Errors->value()==1)
	{
		ChB_Bat_Export->activate();
	}
	if (ChB_Export_Errors->value()==0)
	{
		ChB_Bat_Export->deactivate();
		ChB_Bat_Export->value(0);		
	}
}

void EoExportWindow::cb_RB_Export_Apr(Fl_Widget *w, void *v)
{
	((EoExportWindow*)v)->cb_RB_Export_Apr_i();
}

void EoExportWindow::cb_RB_Export_Apr_i()
{	
	if (RB_Export_Apr->value() == 1)	
	{RB_Export_Obs->value(0);
	RB_Export_Adj->value(0);}
	else 
	RB_Export_Apr->value(1);
}

void EoExportWindow::cb_RB_Export_Obs(Fl_Widget *w, void *v)
{
	((EoExportWindow*)v)->cb_RB_Export_Obs_i();
}

void EoExportWindow::cb_RB_Export_Obs_i()
{
	if (RB_Export_Obs->value() == 1)	
	{RB_Export_Apr->value(0);
	RB_Export_Adj->value(0);}
	else 
	RB_Export_Obs->value(1);
}

void EoExportWindow::cb_RB_Export_Adj(Fl_Widget *w, void *v)
{
	((EoExportWindow*)v)->cb_RB_Export_Adj_i();
}

void EoExportWindow::cb_RB_Export_Adj_i()
{
	if (RB_Export_Adj->value() == 1)	
	{RB_Export_Apr->value(0);
	RB_Export_Obs->value(0);}
	else 
	RB_Export_Adj->value(1);
}

void EoExportWindow::cb_Input_Rotation(Fl_Widget *w, void *v)
{
	((EoExportWindow*)v)->cb_Input_Rotation_i();
}

void EoExportWindow::cb_Input_Rotation_i()
{
	string s = Input_Rotation->value();
	if (s == "direction cosines" || s == "quaternion")
	{
		ChB_Export_Errors->deactivate();
		ChB_Bat_Export->deactivate();
		ChB_Export_Errors->value(0);
		ChB_Bat_Export->value(0);
	}
	else
	{
		ChB_Export_Errors->activate();
		ChB_Bat_Export->activate();
	}
}


void EoExportWindow::cb_Button_Export(Fl_Widget *w, void *v)
{
	((EoExportWindow*)v)->cb_Button_Export_i();
}

void EoExportWindow::cb_Button_Export_i()
{
	ofstream STR;
	string OutputFile = ((MainWindow*)win)->ProjectFolder + Input_File->value();
	STR.open(OutputFile.c_str());
	Image Obraz;
	for (int i=1; i<=Browser_Images->size(); i++)
	{
		if (Browser_Images->selected(i))
		{
			string BrowserStr = Browser_Images->text(i);
			string IdaImageName = ((MainWindow*)win)->ProjectFolder +  BrowserStr;
			//cout <<IdaImageName <<endl;
			//cout <<Input_File->value() <<endl;
			Obraz.ReadFromFile(IdaImageName.c_str());
			
			double AnglesOrig[3];
			double Angles[3];
			double Coords[3];
			double Q[4];
			double R[9];
			string RotationString = Input_Rotation->value();

			if (RB_Export_Adj->value() == 1)
			{
				AnglesOrig[0] = Obraz.AnglesAdj[0];
				AnglesOrig[1] = Obraz.AnglesAdj[1];
				AnglesOrig[2] = Obraz.AnglesAdj[2];
				Coords[0] = Obraz.CoordsAdj[0];
				Coords[1] = Obraz.CoordsAdj[1];
				Coords[2] = Obraz.CoordsAdj[2];
			}
			if (RB_Export_Obs->value() == 1)
			{
				AnglesOrig[0] = Obraz.AnglesObs[0];
				AnglesOrig[1] = Obraz.AnglesObs[1];
				AnglesOrig[2] = Obraz.AnglesObs[2];
				Coords[0] = Obraz.CoordsObs[0];
				Coords[1] = Obraz.CoordsObs[1];
				Coords[2] = Obraz.CoordsObs[2];		
			}
			if (RB_Export_Apr->value() == 1)			
			{
				AnglesOrig[0] = Obraz.AnglesApr[0];
				AnglesOrig[1] = Obraz.AnglesApr[1];
				AnglesOrig[2] = Obraz.AnglesApr[2];
				Coords[0] = Obraz.CoordsApr[0];
				Coords[1] = Obraz.CoordsApr[1];
				Coords[2] = Obraz.CoordsApr[2];					
			}

			if ((RotationString == "al-ni-ka") || (RotationString == "om-fi-ka") || (RotationString == "roll-pitch-yaw"))
			{
				//Przeliczenia na inne sekwencje obrotu
				fT_angles2rot(AnglesOrig,R,Obraz.AngularSystem);
				fT_rot2angles(R,Angles,RotationString);
				STR << Obraz.Name <<" "<<fixed<<setprecision(4) <<Coords[0]<<" "<<Coords[1]<<" "<<Coords[2]<<" "
						<< setprecision(5)<< Angles[0]<<" "<<Angles[1]<<" "<<Angles[2];			
			}

			if (RotationString == "quaternion")
			{
				fT_angles2rot(AnglesOrig,R,Obraz.AngularSystem);
				fT_rot2quaternions(R,Q);
				STR << Obraz.Name <<" "<<fixed<<setprecision(4) <<Coords[0]<<" "<<Coords[1]<<" "<<Coords[2]<<" "
						<< setprecision(12)<< Q[0]<<" "<<Q[1]<<" "<<Q[2]<<" "<<Q[3];	
			}

			if (RotationString == "direction cosines")
			{
				fT_angles2rot(AnglesOrig,R,Obraz.AngularSystem);
				STR << Obraz.Name <<" "<<fixed<<setprecision(4) <<Coords[0]<<" "<<Coords[1]<<" "<<Coords[2]<<" "
					<< setprecision(12)<< R[0]<<" "<<R[1]<<" "<<R[2]<<" "<<R[3]<<" "<<R[4]<<" "<<R[5]<<" "<<R[6]<<" "<<R[7]<<" "<<R[8];	
			}

			if (ChB_Export_Errors->value() == 1)
			{
				if (RB_Export_Apr->value() == 1)
				{
					STR <<fixed << setprecision(3)<<" "<<Obraz.CoordsAprErr[0]<<" "<<Obraz.CoordsAprErr[1]<<" "<<Obraz.CoordsAprErr[2] <<" "
						<< setprecision(4) <<Obraz.AnglesAprErr[0]<<" "<<Obraz.AnglesAprErr[1]<<" "<<Obraz.AnglesAprErr[2];
				}
				if (RB_Export_Obs->value() == 1)
				{
					STR <<fixed << setprecision(3)<<" "<<Obraz.CoordsObsErr[0]<<" "<<Obraz.CoordsAprErr[1]<<" "<<Obraz.CoordsAprErr[2] <<" "
						<< setprecision(4) << Obraz.AnglesObsErr[0]<<" "<<Obraz.AnglesAprErr[1]<<" "<<Obraz.AnglesAprErr[2];
				}
				if (RB_Export_Adj->value() == 1)
				{
					STR << fixed <<setprecision(3) <<" "<<Obraz.CoordsObsErr[0]<<" "<<Obraz.CoordsObsErr[1]<<" "<<Obraz.CoordsObsErr[2] <<" "
						<< setprecision(4)<<Obraz.AnglesObsErr[0]<<" "<<Obraz.AnglesObsErr[1]<<" "<<Obraz.AnglesObsErr[2];
				}

				if (ChB_Bat_Export->value() == 1)
				{
					STR<<" 0 1"<<endl;
				}
				else
				{
					STR<<endl;
				}
			}
			else
			{
				STR <<endl;
			}
			

		}
	}
	STR.close();
}

