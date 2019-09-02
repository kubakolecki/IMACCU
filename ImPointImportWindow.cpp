#include "pch.h"
#include "ImPointImportWindow.h"

ImPointImportWindow::ImPointImportWindow(int w, int h, const char* title) : Fl_Window(w,h,title)
{
	begin();
		Button_Browse = new Fl_Button(75,10,80,25,"Browse...");
		Output_File = new Fl_Output(75,40,250,22,"Input File: ");
		Check_Accuracy = new Fl_Check_Button(75,102,15,15,"Change or set accuracy to:");
		Input_AccuracyX = new Fl_Float_Input(75,130,40,22,"Mx");
		Input_AccuracyY = new Fl_Float_Input(165,130,40,22,"My");
		Button_Execute = new Fl_Button(75,200,80,25,"Execute");
		Chooser_InputFile = new Fl_File_Chooser(NULL,"*.txt\t*.asc\t*.pix" ,1,"Select input file");
	end();
	Input_AccuracyX->deactivate(); //Na poczatku opcja zmiany dokladnosci jest nieaktywna
	Input_AccuracyY->deactivate();
	Input_AccuracyX->value("0.5");
	Input_AccuracyY->value("0.5");
	Check_Accuracy->callback(cb_Check_Accuracy,this);
	Button_Browse->callback(cb_Button_Browse,this);
	Chooser_InputFile->callback(cb_Chooser_InputFile,this);
	Button_Execute->callback(cb_Button_Execute,this);
}

ImPointImportWindow::~ImPointImportWindow(void)
{

}

void ImPointImportWindow::cb_Button_Browse(Fl_Widget *w, void *v)
{
	((ImPointImportWindow*)v)->cb_Button_Browse_i();
}

void ImPointImportWindow::cb_Button_Browse_i()
{
	Chooser_InputFile->show();
}


void ImPointImportWindow::cb_Check_Accuracy(Fl_Widget *w, void *v)
{
	((ImPointImportWindow*)v)->cb_Check_Accuracy_i();
}

void ImPointImportWindow::cb_Check_Accuracy_i()
{
	if (Check_Accuracy->value() ==1)
	{
		Input_AccuracyX->activate();
		Input_AccuracyY->activate();

	}
	else
	{
		Input_AccuracyX->deactivate();
		Input_AccuracyY->deactivate();
	}
}

void ImPointImportWindow::cb_Chooser_InputFile(Fl_File_Chooser *w, void *v)
{
	((ImPointImportWindow*)v)->cb_Chooser_InputFile_i();
}

void ImPointImportWindow::cb_Chooser_InputFile_i()
{
	Output_File->value(Chooser_InputFile->value());
}



void ImPointImportWindow::cb_Button_Execute(Fl_Widget *w, void *v)
{
	((ImPointImportWindow*)v)->cb_Button_Execute_i();
}

void ImPointImportWindow::cb_Button_Execute_i()
{
	ifstream StreamFromPix;
	StreamFromPix.open(Output_File->value());
	if (!StreamFromPix)
	{
		//cout<<"Blad odczytu pliku!" <<endl;
	}
	else
	{
		double x,y;
		double UsersMx, UsersMy;
		int Type;
		string PointName, ImageName;
		string Linia; //Cala linijka
		string PixName, IdaName; //Nazwy plikow .pix i .ida

		
		if (Check_Accuracy->value() == 1)
		{
			string sx(Input_AccuracyX->value());
			string sy(Input_AccuracyY->value());
			istringstream Strsx(sx);
			istringstream Strsy(sy);
			Strsx >> UsersMx;
			Strsy >> UsersMy;
		}
		while (getline(StreamFromPix, Linia, '\n'))
		{			
			//Jezeli w stringu nie ma spacji to znaczy ze zawiera on numer zdjecia
			int gdzie; //Gdzie jest spacja
			gdzie = Linia.find_first_of(" ");
			istringstream LiniaStream(Linia);
			if (gdzie == -1)
			{
				LiniaStream >> ImageName;
				//cout << ImageName<<endl;
				//Otwieramy strumien do zapisu punktow do pliku pix:
				PixName = ((MainWindow*)win)->ProjectFolder + ImageName + ".pix";
				IdaName = ((MainWindow*)win)->ProjectFolder + ImageName + ".ida"; 
				//cout << PixName <<endl;
				//cout << IdaName <<endl;
			}
			else
			{
				LiniaStream >> PointName>> x >>y;
				cout <<PointName<<" "<<x<<" "<<y<<endl;

				//Trzeba odnalezc typ punktu
				
				if (((MainWindow*)win)->NGCP > 0)
				{
					Type = 0;
					for (int i=0; i< ((MainWindow*)win)->NGCP; i++)
					{
						if (PointName == ((MainWindow*)win)->GC[i].Name)
						{
							Type = ((MainWindow*)win)->GC[i].Type;
						}
					}
				}
				else
				{
					Type = 0;
				}

				ImagePoint P;
				if (Check_Accuracy->value() == 1)
				{
					ImagePoint Punkt(PointName,ImageName,x,y,UsersMx,UsersMy,Type);
					P = Punkt;				
				}
				else
				{
					ImagePoint Punkt(PointName,ImageName,x,y,0.5,0.5,Type);
					P = Punkt;
				}

				//Skierowanie strumienia na plik .pix;
				ImagePoint::StrumienDoZapisu_L.close();
				if(!ImagePoint::StrumienDoZapisu_L)
				{
					//Kasuje flage bledu
					ImagePoint::StrumienDoZapisu_L.clear(ImagePoint::StrumienDoZapisu_L.rdstate() & ~ios::failbit);		
				}
				ImagePoint::StrumienDoZapisu_L.open(PixName.c_str(),ios::app);

				P.write_L(); //Zapisujemy punkt
				
				Image Obraz;
				Obraz.ReadFromFile(IdaName.c_str());
				Obraz.NPoints++;
				Obraz.WriteToFile(IdaName.c_str());

			}		
		}
	}
}