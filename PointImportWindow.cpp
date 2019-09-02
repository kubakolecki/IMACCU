#include "pch.h"
#include "PointImportWindow.h"

PointImportWindow::PointImportWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		Button_Browse = new Fl_Button(75,10,80,25,"Browse...");
		Output_File = new Fl_Output(75,40,250,22,"Input File: ");
		Check_Mask = new Fl_Check_Button(75,75,15,15,"Use \"Nr_X_Y_Z\" mask");
		Check_Accuracy = new Fl_Check_Button(75,102,15,15,"Change or set accuracy to:");
		Input_AccuracyX = new Fl_Float_Input(75,130,40,22,"Mx");
		Input_AccuracyY = new Fl_Float_Input(165,130,40,22,"My");
		Input_AccuracyZ = new Fl_Float_Input(255,130,40,22,"Mz");
		Button_Execute = new Fl_Button(75,200,80,25,"Execute");
		Chooser_GCPFile = new Fl_File_Chooser(NULL,"*.txt\t*.asc\t*.gcp\t*" ,1,"Select GCP file");
	end();
	Input_AccuracyX->deactivate(); //Na poczatku opcja zmiany dokladnosci jest nieaktywna
	Input_AccuracyY->deactivate();
	Input_AccuracyZ->deactivate();

	Check_Accuracy->callback(cb_Check_Accuracy,this);
	Button_Browse->callback(cb_Button_Browse,this);
	Chooser_GCPFile->callback(cb_Chooser_GCPFile,this);
	Button_Execute->callback(cb_Button_Execute,this);
}

PointImportWindow::~PointImportWindow(void)
{	
	

}

void PointImportWindow::cb_Button_Browse(Fl_Widget *w, void *v)
{
	((PointImportWindow*)v)->cb_Button_Browse_i();
}

void PointImportWindow::cb_Button_Browse_i()
{
	Chooser_GCPFile->show();
}

void PointImportWindow::cb_Check_Accuracy(Fl_Widget *w, void *v)
{
	((PointImportWindow*)v)->cb_Check_Accuracy_i();
}

void PointImportWindow::cb_Check_Accuracy_i()
{
	if (Check_Accuracy->value() ==1)
	{
		Input_AccuracyX->activate();
		Input_AccuracyY->activate();
		Input_AccuracyZ->activate();

	}
	else
	{
		Input_AccuracyX->deactivate();
		Input_AccuracyY->deactivate();
		Input_AccuracyZ->deactivate();

	}
}

void PointImportWindow::cb_Chooser_GCPFile(Fl_File_Chooser *w, void *v)
{
	((PointImportWindow*)v)->cb_Chooser_GCPFile_i();
}

void PointImportWindow::cb_Chooser_GCPFile_i()
{
	Output_File->value(Chooser_GCPFile->value());
}

void PointImportWindow::cb_Button_Execute(Fl_Widget *w, void *v)
{
	((PointImportWindow*)v)->cb_Button_Execute_i();
}

void PointImportWindow::cb_Button_Execute_i()
{
	ifstream StreamFromGCP;
	StreamFromGCP.open(Output_File->value());
	if (!StreamFromGCP)
	{
		cout<<"Blad odczytu pliku!" <<endl;
	}
	else
	{
		((MainWindow*)win)->GC.clear();
		((MainWindow*)win)->NGCP = countGCP(Output_File->value());
		string linia;
		for (int i=0; i<((MainWindow*)win)->NGCP; i++)
		{
			getline(StreamFromGCP,linia,'\n'); //Analiza pliku z fotopunktami
			string GCPId; //Id fotopunktu
			double X, Y, Z; //Wspolrzedne fotopunktu
			double Mx, My, Mz; //Dokladnosc a'priori
			short Type; //Kod
			istringstream StreamFromLine(linia);
			if (Check_Mask->value() == 0)
			{
				StreamFromLine >> GCPId >> X >> Y >> Z >> Mx >> My >> Mz >> Type;
			}
			else
			{
				StreamFromLine >> GCPId >> X >> Y >> Z;
			}

			if (Check_Accuracy->value()==0)
			{
				if (Check_Mask->value() == 1)
				{
					((MainWindow*)win)->GC.push_back(TerrainPoint(GCPId,X,Y,Z));
				}
				else
				{	
					((MainWindow*)win)->GC.push_back(TerrainPoint(GCPId,X,Y,Z,Mx,My,Mz,Type));
				}
			
			}
			else
			{
				string sx(Input_AccuracyX->value());
				string sy(Input_AccuracyY->value());
				string sz(Input_AccuracyZ->value());
				istringstream Strsx(sx);
				istringstream Strsy(sy);
				istringstream Strsz(sz);
				Strsx >> Mx;
				Strsy >> My;
				Strsz >> Mz;
				if (Check_Mask->value() == 1)
				{
					((MainWindow*)win)->GC[i] = TerrainPoint(GCPId,X,Y,Z,Mx,My,Mz);
				}
				else
				{
					((MainWindow*)win)->GC[i] = TerrainPoint(GCPId,X,Y,Z,Mx,My,Mz,Type);
				}
			}
			StreamFromLine.clear();		
		}


		std::string gcp_filename = ((MainWindow*)win)->ProjectFolder +
			((MainWindow*)win)->CurrentProjectName.substr(0, ((MainWindow*)win)->CurrentProjectName.length() - 4) +
			".gcp";

		writeGCPToFile (gcp_filename, ((MainWindow*)win)->GC);

	}
	StreamFromGCP.close();

}
