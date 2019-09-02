#include "pch.h"
#include "PointLabelWindow.h"

PointLabelWindow::PointLabelWindow(int w, int h, const char* title):Fl_Window(w,h,title)
{
	begin();
		InputId = new Fl_Input(60,20,100,20,"Point ID:");
		RB_GCP = new Fl_Round_Button(20,45,140,25,"GCP");
		RB_Tie = new Fl_Round_Button(20,70,140,25,"Tie");
		RB_TieLine = new Fl_Round_Button(20,95,140,25,"Tie line");
		RB_TieLineHorizontal = new Fl_Round_Button(20,120,140,25,"Horizontal tie line ");	
		RB_TieLineVertical = new Fl_Round_Button(20,145,140,25,"Vertical tie line ");
		RB_ACSPoint = new Fl_Round_Button(20,170,140,25,"ACS Point ");

		InputLineId = new Fl_Input(60,205,80,20,"Line ID:");
		ButtonOk = new Fl_Button(20,250,65,25,"Ok");
		ButtonCancel = new Fl_Button(115,250,65,25,"Cancel");
		ChBInc = new Fl_Check_Button(170,20,20,20,"inc.");
		InputInc = new Fl_Int_Input(215,20,20,20);
	end();
	
	RB_GCP->callback(cb_RB_GCP,this);
	RB_Tie->callback(cb_RB_Tie,this);
	RB_TieLine->callback(cb_RB_TieLine,this);
	RB_TieLineHorizontal->callback(cb_RB_TieLineHorizontal,this);
	RB_TieLineVertical->callback(cb_RB_TieLineVertical,this);
	RB_ACSPoint->callback(cb_RB_ACSPoint,this);
	ButtonOk->callback(cb_Button_Ok,this);
	ButtonCancel->callback(cb_Button_Cancel,this);
	RB_Tie->value(1);
	RB_ACSPoint->value(0);
	InputLineId->deactivate();
	InputId->callback(cb_InputId,this);
	InputId->when(1);
	ChBInc->value(0);
	InputInc->value("1");

	RB_Tie_val = 1;
	RB_TieLine_val = 0;
	RB_TieLineHorizontal_val = 0;
	RB_TieLineVertical_val = 0;
	
	IsGCP = false;
}

PointLabelWindow::~PointLabelWindow(void)
{
}

void PointLabelWindow::cb_Button_Cancel(Fl_Widget *w, void *v)
{
	((PointLabelWindow*)v)->cb_Button_Cancel_i();
}
void PointLabelWindow::cb_Button_Cancel_i()
{
	//Ustawiamy taki tryb pracy jaki byl poprzednio
	((ImagesShowWindow*)win)->StereoWindow->set_DrawingMode( ((ImagesShowWindow*)win)->StereoWindow->PreviousDrawingMode );
	this->hide();
}


void PointLabelWindow::cb_Button_Ok(Fl_Widget *w, void *v)
{
	((PointLabelWindow*)v)->cb_Button_Ok_i();
}
void PointLabelWindow::cb_Button_Ok_i()
{
	string PointId(InputId->value());
	string LineId(InputLineId->value());
	if (LineId == "") LineId="0";
	if (PointId !="") //Jezeli nazwa jest inna niz pusta
	{
		if (RB_ACSPoint->value() == 0) //jezeli jest inny tryb niz ACS
		{
			// Jezeli aktywny jest pomiar na obrazie lewym
			if ( ((ImagesShowWindow*)win)->Toggle_ButtonPomiarLewy->value() == 1 )
			{
				double X = ((ImagesShowWindow*)win)->StereoWindow->Cross[0]; //Pobieranie wspolrzednych
				double Y = ((ImagesShowWindow*)win)->StereoWindow->Cross[1];
				short Typ;
				// Pobieranie typu
				if (RB_GCP->value() == 1 )
				{
					Typ = 3;
					//Dopisac kod obslugujacy typy 2 i 3
				}
				if (RB_Tie->value() == 1 ) Typ = 0;
				if (RB_TieLine->value() == 1 ) Typ = 5;
				if (RB_TieLineHorizontal->value() == 1 ) Typ = 6;
				if (RB_TieLineVertical->value() == 1 ) Typ = 7;
				string s( ((ImagesShowWindow*)win)->InputObrazLewy->value() );
				string Image = s.substr(0,s.length()-4); //Aby wyeliminowac ".ida"
				ImagePoint P(PointId,Image,LineId,X,Y,Typ);
				P.write_L();
				((ImagesShowWindow*)win)->LeftImage.NPoints++;
				((ImagesShowWindow*)win)->LeftImage.WriteToFile( ((ImagesShowWindow*)win)->IdaFileName_Left.c_str());
				((ImagesShowWindow*)win)->StereoWindow->LeftPoints.push_back(P);
			}

			// Jezeli aktywny jest pomiar na obrazie prawym
			if ( ((ImagesShowWindow*)win)->Toggle_ButtonPomiarPrawy->value() == 1 )
			{
				double X =  ((ImagesShowWindow*)win)->StereoWindow->Cross[2] ; //Pobieranie wspolrzednych
				double Y =  ((ImagesShowWindow*)win)->StereoWindow->Cross[3] ;
				short Typ;
				// Pobieranie typu
				if (RB_GCP->value() == 1 ) Typ = 3;
				if (RB_Tie->value() == 1 ) Typ = 0;
				if (RB_TieLine->value() == 1 ) Typ = 5;
				if (RB_TieLineHorizontal->value() == 1 ) Typ = 6;
				if (RB_TieLineVertical->value() == 1 ) Typ = 7;
				string s( ((ImagesShowWindow*)win)->InputObrazPrawy->value() );
				string Image = s.substr(0,s.length()-4); //Aby wyeliminowac ".ida"
				ImagePoint P(PointId,Image,LineId,X,Y,Typ);
				P.write_P();
				((ImagesShowWindow*)win)->RightImage.NPoints++;
				((ImagesShowWindow*)win)->RightImage.WriteToFile( ((ImagesShowWindow*)win)->IdaFileName_Right.c_str());
				((ImagesShowWindow*)win)->StereoWindow->RightPoints.push_back(P);
			}
		}
		else
		{
			double X1 = ((ImagesShowWindow*)win)->StereoWindow->Cross[0]; //Pobieranie wspolrzednych
			double Y1 = ((ImagesShowWindow*)win)->StereoWindow->Cross[1];
			double X2 =  ((ImagesShowWindow*)win)->StereoWindow->Cross[2]; //Pobieranie wspolrzednych
			double Y2 =  ((ImagesShowWindow*)win)->StereoWindow->Cross[3];

			ImagePoint P1(PointId,"",LineId,X1,Y1,101);
			ImagePoint P2(PointId,"",LineId,X2,Y2,101);


			TerrainPoint TP = ((ImagesShowWindow*)win)->StereoWindow->ModPoint;
			TP.Name = PointId;
		}
	}
	//Ustawiamy taki tryb pracy jaki byl poprzednio	
	this->hide(); // Zamknij okno
	
	if ( (ChBInc->value()== 1) && PointId.length() > 0)
	{
		//string LastChar = PointId.substr(PointId.length()-1,1);
		//char ch = LastChar[0];
		//if (isdigit(ch))
		//{
		//	stringstream ss;
		//	int temp;
		//	ss << LastChar;
		//	ss >> temp;
		//	temp++;
		//	ss.str("");
		//	ss.clear();
		//	ss << temp;
		//	LastChar = ss.str();
		//	string NewNumber = PointId.substr(0,PointId.length()-1) + LastChar;
		//	InputId->value(NewNumber.c_str());
		//}
			stringstream ss;
			stringstream incvalstr;
			int temp;
			short incval;
			string incvalstring = InputInc->value();
			incvalstr<<incvalstring;
			incvalstr>>incval;
			ss << PointId;
			ss >> temp;
			temp = temp + incval;
			ss.str("");
			ss.clear();
			ss << temp;
			string NewNumber;
			ss >> NewNumber;
			InputId->value(NewNumber.c_str());
			InputId->do_callback();
	}


	((ImagesShowWindow*)win)->StereoWindow->set_DrawingMode( ((ImagesShowWindow*)win)->StereoWindow->PreviousDrawingMode );
	//cout<<"PreviousDrawingMode: "<< ((ImagesShowWindow*)win)->StereoWindow->PreviousDrawingMode <<endl;
	((ImagesShowWindow*)win)->StereoWindow ->redraw();

}


void PointLabelWindow::cb_RB_GCP(Fl_Widget *w, void *v)
{
	((PointLabelWindow*)v)->cb_RB_GCP_i();
}
void PointLabelWindow::cb_RB_GCP_i()
{
	RB_GCP->value(0);
	if (IsGCP==true)
	{
		RB_GCP->value(1);
	}
	else
	{
		RB_GCP->value(0);
	}
}

void PointLabelWindow::cb_RB_ACSPoint(Fl_Widget *w, void *v)
{
	((PointLabelWindow*)v)->cb_RB_ACSPoint_i();
}
void PointLabelWindow::cb_RB_ACSPoint_i()
{
	RB_ACSPoint->value(0);
}

void PointLabelWindow::cb_RB_Tie(Fl_Widget *w, void *v)
{
	((PointLabelWindow*)v)->cb_RB_Tie_i();
}
void PointLabelWindow::cb_RB_Tie_i()
{
	RB_Tie->value(1);
	RB_TieLine->value(0);
	RB_TieLineHorizontal->value(0);
	RB_TieLineVertical->value(0);
	InputLineId->deactivate();
}

void PointLabelWindow::cb_RB_TieLine(Fl_Widget *w, void *v)
{
	((PointLabelWindow*)v)->cb_RB_TieLine_i();
}
void PointLabelWindow::cb_RB_TieLine_i()
{
	RB_TieLine->value(1);
	RB_Tie->value(0);
	RB_TieLineHorizontal->value(0);
	RB_TieLineVertical->value(0);
	InputLineId->activate();
}

void PointLabelWindow::cb_RB_TieLineHorizontal(Fl_Widget *w, void *v)
{
	((PointLabelWindow*)v)->cb_RB_TieLineHorizontal_i();
}
void PointLabelWindow::cb_RB_TieLineHorizontal_i()
{
	RB_TieLineHorizontal->value(1);
	RB_Tie->value(0);
	RB_TieLine->value(0);
	RB_TieLineVertical->value(0);
	InputLineId->activate();
}

void PointLabelWindow::cb_RB_TieLineVertical(Fl_Widget *w, void *v)
{	
	((PointLabelWindow*)v)->cb_RB_TieLineVertical_i();
}
void PointLabelWindow::cb_RB_TieLineVertical_i()
{
	RB_TieLineVertical->value(1);
	RB_Tie->value(0);
	RB_TieLine->value(0);
	RB_TieLineHorizontal->value(0);
	InputLineId->activate();
}

void PointLabelWindow::cb_InputId(Fl_Widget *w, void *v)
{	
	((PointLabelWindow*)v)->cb_InputId_i();
}
void PointLabelWindow::cb_InputId_i()
{

	if (RB_ACSPoint->value() == 0)
	{

		string Id(InputId->value());
		int NGCP = ((MainWindow*)((ImagesShowWindow*)win)->win)->NGCP; //Podstawienie za zmienna lokalna dla czytelnosci
		IsGCP = false;


		for (int i=0; i<NGCP; i++)
		{
			if (((MainWindow*)((ImagesShowWindow*)win)->win)->GC.at(i).Name == Id)
			{
				IsGCP = true;

			}
		}
			if ((IsGCP==false) && (RB_GCP->value()==0))
			{
				//cout << "zapisuje wartosci. RB_Tie wynosi " <<RB_Tie_val<<endl;
				RB_Tie_val = RB_Tie->value();

				RB_TieLine_val = RB_TieLine->value();
				RB_TieLineHorizontal_val = RB_TieLineHorizontal->value();
				RB_TieLineVertical_val = RB_TieLineVertical->value();
			}
			if (IsGCP)
			{
				//cout << "zmieniam wartosci dla fotopunktu" <<endl;
				RB_GCP->value(1);
				RB_Tie->value(0);
				RB_TieLine->value(0);
				RB_TieLineHorizontal->value(0);
				RB_TieLineVertical->value(0);
				RB_Tie->deactivate();
				RB_TieLine->deactivate();
				RB_TieLineHorizontal->deactivate();
				RB_TieLineVertical->deactivate();
				InputLineId->deactivate();
			}
			else
			{
				//cout << "zmieniam wartosci dla punktow wiazacych" <<endl;
				RB_GCP->value(0);

				RB_Tie->value(RB_Tie_val);
				RB_TieLine->value(RB_TieLine_val);
				RB_TieLineHorizontal->value(RB_TieLineHorizontal_val);
				RB_TieLineVertical->value(RB_TieLineVertical_val);

				RB_Tie->activate();
				RB_TieLine->activate();
				RB_TieLineHorizontal->activate();
				RB_TieLineVertical->activate();

				if ((RB_Tie->value() == 1) || (RB_GCP->value() == 1))
				{
					InputLineId->deactivate();
				}
				else
				{
					InputLineId->activate();
				}
			}
	}
	else
	{
		IsGCP = false;
	}
}

int PointLabelWindow::handle (int e)
{
	int ret = Fl_Window::handle(e);
	switch (e)
	{
		case FL_KEYBOARD:
		switch (Fl::event_key())
		{
			case FL_Enter:
				ButtonOk->do_callback();
				
				ret = 1;
			break;
		}
		break;
	}
	return ret;
}
