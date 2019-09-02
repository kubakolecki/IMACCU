#include "pch.h"
#include "ImagesShowWindow.h"
#include "ImagePoint.h"
#include "kFltk.h"

using namespace std;

ImagesShowWindow::ImagesShowWindow(int _Id, int X,int Y,int W,int H,const char*L) : Id(_Id), Fl_Double_Window(X, Y, W, H, L)
{
	begin();
		StereoWindow = new StereoGlWindow(0,20,W,H-20);
		Button_SM = new Fl_Button(5,0,19,19,"S");
		InputObrazLewy = new Fl_Input_Choice(45,0,100,20,"L:");
		Toggle_ButtonPomiarLewy = new Fl_Toggle_Button(150,0,19,19);		
		InputObrazPrawy = new Fl_Input_Choice(190,0,100,20,"R:");
		Toggle_ButtonPomiarPrawy = new Fl_Toggle_Button(295,0,19,19);
		OutputDrawingMode = new Fl_Output(325,0,15,20);
		OutputZoom = new Fl_Output (375,0,20,20,"Zoom:");
		Button_TMLRX = new Fl_Button (452,0,19,19,"X");
		OutputCoords = new Fl_Output(474,0,240,19);
	end();
	OknoOznaczania = new PointLabelWindow(240,291,"Image Point Setting");
	OknoZmianyNazwy = new PointRenameWindow(430, 210, "Image Point Rename");

	ImageNotFoundWindow = new WarningWindow(180,120,"Error","Image can not be loaded!");
	this->resizable(StereoWindow);
	InputObrazLewy->callback(cb_Input_ObrazLewy,this);
	InputObrazPrawy->callback(cb_Input_ObrazPrawy,this);
	OutputDrawingMode->callback(cb_Output_DrawingMode,this);
	Toggle_ButtonPomiarLewy->callback(cb_Toggle_ButtonPomiarLewy,this);
	Toggle_ButtonPomiarPrawy->callback(cb_Toggle_ButtonPomiarPrawy,this);
	Button_SM->callback(cb_Button_SM,this);
	Button_SM->color(fl_rgb_color(247,181,53));
	Button_TMLRX->callback(cb_Button_TMLRX,this);
	Button_TMLRX->color(fl_rgb_color(206,206,206));
	OutputCoords->hide();
	OutputCoords->textsize(9);
	CoordsDispMode = 0;
	Toggle_ButtonPomiarLewy->deactivate();
	Toggle_ButtonPomiarPrawy->deactivate();
	OutputZoom->value("1:1");
	ButtonIcon1 = new Fl_JPEG_Image("pomiar_active.jpg"); //przycisk z ikona
	ButtonIcon2 = new Fl_JPEG_Image("pomiar_inactive.jpg"); //przycisk z ikona
	Toggle_ButtonPomiarLewy->image(ButtonIcon2);	
	Toggle_ButtonPomiarPrawy->image(ButtonIcon2);
	StereoWindow->ParentWindow = this;
	OknoOznaczania->win = this;
	OknoZmianyNazwy->win = this;
	OutputDrawingMode->value("N");
	OutputDrawingMode->color(FL_RED);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glEnable(GL_TEXTURE_2D);
	LeftIdx = 0;
	RightIdx = 0;
}

ImagesShowWindow::~ImagesShowWindow(void)
{

}



int ImagesShowWindow::loadTex_Lumin(IplImage *image, GLuint *textureName)
{
	if (image==NULL) return -1;	
	glGenTextures(1, textureName);
	glBindTexture(GL_TEXTURE_2D, *textureName);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image->width, image->height,0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->imageData);
	return 0;
}

int ImagesShowWindow::loadTex_RGB(IplImage *image, GLuint *textureName)
{
	if (image==NULL) return -1;
	glGenTextures(1, textureName);
	glBindTexture(GL_TEXTURE_2D, *textureName);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
	return 0;
}

void ImagesShowWindow::load_Textures(IplImage *Image, vector <TextureQuad> *Quadsy, vector <IplImage*> *Obrazki, GLuint*& Tekstury, unsigned int* N) //Ladowanie tekstur do quadsow 256 x 256
{
	//Obliczanie ilosci i wspolrzednych Quadsow
	int Rows = (int)ceil(Image->height / 256.0);
	int Cols = (int)ceil(Image->width / 256.0);
	//cout <<"Rows: "<<Rows<<endl;
	//cout <<"Cols: "<<Cols<<endl;
	Quadsy->clear();
	Quadsy->resize(Rows*Cols); //Rezerwujemy wektor Quadsow

	//Musimy teraz utworzyc nowy obraz ktorego wymiary sa wielokrotnoscia 256
	//Skopiowany do niego zostanie nowy obraz, a w pozostale miejsca wpisane zostana
	//Wartosci 0 - piksele czarne

	IplImage *BigImage = cvCreateImage(cvSize(256 * Cols, 256 * Rows), Image->depth, Image->nChannels);

	uchar *Iptr = NULL; //Wskazniki do poruszania sie po obrazach
	uchar *Bptr = NULL;
	if (BigImage->nChannels == 3) // Przypadek obrazu kolorowego
	{
		for (int y = 0; y < BigImage->height; y++)
		{
			Bptr = (uchar*)(BigImage->imageData + y * BigImage->widthStep);
			if (y < Image->height)
			{
				Iptr = (uchar*)(Image->imageData + y * Image->widthStep);
			}
			for (int x = 0; x < BigImage->width; x++)
			{
				if ((x < Image->width) && (y < Image->height))
				{
					Bptr[3 * x] = Iptr[3 * x];
					Bptr[3 * x + 1] = Iptr[3 * x + 1];
					Bptr[3 * x + 2] = Iptr[3 * x + 2];
				}
				else
				{
					Bptr[3 * x] = 0;
					Bptr[3 * x + 1] = 0;
					Bptr[3 * x + 2] = 0;
				}
			}
		}
	}

	if (BigImage->nChannels == 1) // Przypadek obrazu szarego
	{
		for (int y = 0; y < BigImage->height; y++)
		{
			Bptr = (uchar*)(BigImage->imageData + y * BigImage->widthStep);
			if (y < Image->height)
			{
				Iptr = (uchar*)(Image->imageData + y * Image->widthStep);
			}
			for (int x = 0; x < BigImage->width; x++)
			{
				if ((x < Image->width) && (y < Image->height))
				{
					Bptr[x] = Iptr[x];
				}
				else
				{
					Bptr[x] = 0;
				}
			}
		}
	}


	// GENERUJEMY WSP QUADSOW
	int k = 0; //Numer quadsa w wektorze
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			TextureQuad Q(256 * j, Image->height - 256 * i, 256 * (j + 1), Image->height - 256 * (i + 1), i, j);
			(*Quadsy)[k] = Q;
			k++;
		}
	}

	//for (int i=0; i<Quadsy->size(); i++)
	//{
	//	cout <<"i: "<<i<<"  X1: "<< (*Quadsy)[i].X1<< "  Y1: "<< (*Quadsy)[i].Y1<<"  X2: "<< (*Quadsy)[i].X2<< "  Y2: "<< (*Quadsy)[i].Y2 <<endl;
	//}

	vector <CvMat*> Zakresy;
	Zakresy.resize(Quadsy->size());

	//cout <<"Image step: "<<Image->widthStep <<endl;
	//cout << "Fragmenty obrazu do quadsow: " <<endl;
	for (int i = 0; i < Quadsy->size(); i++)
	{
		int x = (*Quadsy)[i].X1;
		int y = Image->height - (*Quadsy)[i].Y1;
		int w = (*Quadsy)[i].X2 - (*Quadsy)[i].X1;
		int h = (*Quadsy)[i].Y1 - (*Quadsy)[i].Y2;
		CvRect R = cvRect(x, y, w, h);
		//cout <<"x: "<<R.x <<"  y: "<<R.y <<"  w: " << w <<"  h: "<<h <<endl;
		Zakresy[i] = cvCreateMatHeader(h, w, BigImage->depth);
		cvGetSubRect(BigImage, Zakresy[i], R);
	}

	for (int i = 0; i < Obrazki->size(); i++)
	{
		cvReleaseImage(&(*Obrazki)[i]);
	}

	Obrazki->clear();
	Obrazki->resize(Zakresy.size());

	for (int i = 0; i < Obrazki->size(); i++)
	{
		(*Obrazki)[i] = cvCreateImage(cvSize(Zakresy[i]->width, Zakresy[i]->height), Image->depth, Image->nChannels);
		cvCopy(Zakresy[i], (*Obrazki)[i]);
		//cout << (*Obrazki)[i]->widthStep <<endl;
	}

	cvReleaseImage(&BigImage); //Bitmapy do tekstur sa juz gotowe i mozemy zwolnic duzy obraz

	//cvNamedWindow("Example",1);
	//cvShowImage("Example", I);
	//cvWaitKey(0);
	//cvDestroyWindow("Example");

	//CZYSCIMY TEKSTURY JEZELI ISTNIALY
	if (*N > 0)
	{
		glDeleteTextures(*N, Tekstury);
		delete[] Tekstury;
	}
	Tekstury = new unsigned int[Quadsy->size()];
	*N = 0;
	// GENERUJEMY TEKSTURY

	//Tekstury RGB
	if (Image->nChannels == 3)
	{
		for (int i = 0; i < Quadsy->size(); i++)
		{

			glGenTextures(1, Tekstury + i);
			glBindTexture(GL_TEXTURE_2D, *(Tekstury + i));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (*Obrazki)[i]->width, (*Obrazki)[i]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, (*Obrazki)[i]->imageData);
			(*N)++;
		}
	}

	//Tekstury szare
	if (Image->nChannels == 1)
	{
		for (int i = 0; i < Quadsy->size(); i++)
		{
			//cout << i <<endl;
			glGenTextures(1, Tekstury + i);
			glBindTexture(GL_TEXTURE_2D, *(Tekstury + i));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, (*Obrazki)[i]->width, (*Obrazki)[i]->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, (*Obrazki)[i]->imageData);
		}
	}
}

void ImagesShowWindow::DispCoords(double X, double Y) //Wyswietlanie wspolrzednych 2D
{
	string sx;
	string sy;
	stringstream Strumienx, Strumieny;
	Strumienx << fixed << setprecision(3) << X;
	Strumieny << fixed << setprecision(3) << Y;
	Strumienx >> sx;
	Strumieny >> sy;
	sx = sx + " " + sy; 
	OutputCoords->value(sx.c_str());
}

void ImagesShowWindow::DispCoords(double X, double Y, double Z) //Wyswietlanie wspolrzednych 3D (przeladowanie)
{
	string sx;
	string sy;
	string sz;
	stringstream Strumienx, Strumieny, Strumienz;
	Strumienx << fixed << setprecision(4) << X;
	Strumieny << fixed << setprecision(4) << Y;
	Strumienz << fixed << setprecision(4) << Z;
	Strumienx >> sx;
	Strumieny >> sy;
	Strumienz >> sz;
	sx = sx + " " + sy + " " + sz; 
	OutputCoords->value(sx.c_str());
}

void ImagesShowWindow::changePixStreamL(string s)
{
	//Zamkniecie dotychczasowych strumieni odczytu i zapisu z pliku .pix
	ImagePoint::StrumienDoZapisu_L.close();
	if(!ImagePoint::StrumienDoZapisu_L)
	{
		//Kasuje flage bledu
		ImagePoint::StrumienDoZapisu_L.clear(ImagePoint::StrumienDoZapisu_L.rdstate() & ~ios::failbit);		
	}
	ImagePoint::StrumienDoOdczytu_L.close();
	if(!ImagePoint::StrumienDoOdczytu_L)
	{
		//Kasuje flage bledu
		ImagePoint::StrumienDoOdczytu_L.clear(ImagePoint::StrumienDoOdczytu_L.rdstate() & ~ios::failbit);		
	}

	//Otwarcie strumieni odczytu i zapisu z pliku .pix
	ImagePoint::StrumienDoZapisu_L.open(s.c_str(),ios::app);
	ImagePoint::StrumienDoOdczytu_L.open(s.c_str());
}

void ImagesShowWindow::changePixStreamP(string s)
{
	//Zamkniecie dotychczasowych strumieni odczytu i zapisu z pliku .pix
	ImagePoint::StrumienDoZapisu_P.close();
	if(!ImagePoint::StrumienDoZapisu_P)
	{
		//Kasuje flage bledu
		ImagePoint::StrumienDoZapisu_P.clear(ImagePoint::StrumienDoZapisu_P.rdstate() & ~ios::failbit);		
	}
	ImagePoint::StrumienDoOdczytu_P.close();
	if(!ImagePoint::StrumienDoOdczytu_P)
	{
		ImagePoint::StrumienDoOdczytu_P.clear(ImagePoint::StrumienDoOdczytu_P.rdstate() & ~ios::failbit);		
	}

	//Otwarcie strumieni odczytu i zapisu z pliku .pix
	ImagePoint::StrumienDoZapisu_P.open(s.c_str(),ios::app);
	ImagePoint::StrumienDoOdczytu_P.open(s.c_str());

}

void ImagesShowWindow::load_Points_L ()
{
	//Funkcja do wczytywania fotopunktow i punktow wiazacych
	StereoWindow->LeftPoints.clear(); //Wyczyszczenie wektora
	StereoWindow->LeftPoints.resize(LeftImage.NPoints);
	string IdaFileName = InputObrazLewy->value();
	PixFileName_Left = ((MainWindow*)win)->ProjectFolder + IdaFileName.substr(0,IdaFileName.length()-4) + ".pix";
	changePixStreamL(PixFileName_Left);
	for (int i=0; i<StereoWindow->LeftPoints.size(); i++)
	{
		string Linia;
		getline(ImagePoint::StrumienDoOdczytu_L,Linia);
		ImagePoint P(Linia); 
		StereoWindow->LeftPoints[i] = P;
	}
}

void ImagesShowWindow::load_Points_P ()
{
	//Funkcja do wczytywania fotopunktow i punktow wiazacych
	StereoWindow->RightPoints.clear(); //Wyczyszczenie wektora
	StereoWindow->RightPoints.resize(RightImage.NPoints);
	string IdaFileName = InputObrazPrawy->value();
	PixFileName_Right = ((MainWindow*)win)->ProjectFolder + IdaFileName.substr(0,IdaFileName.length()-4) + ".pix";
	changePixStreamP(PixFileName_Right);
	for (int i=0; i<StereoWindow->RightPoints.size(); i++)
	{
		string Linia;
		getline(ImagePoint::StrumienDoOdczytu_P,Linia);
		ImagePoint P(Linia); 
		StereoWindow->RightPoints[i] = P;
	}
}



void ImagesShowWindow::cb_Input_ObrazLewy (Fl_Widget *w, void *v)
{
	((ImagesShowWindow*)v)->cb_Input_ObrazLewy_i();
}

void ImagesShowWindow::cb_Input_ObrazLewy_i()
{
	ifstream IdaStream; // Tworzymy strumien do pliku .ida
	IdaFileName_Left = ((MainWindow*)win)->ProjectFolder +  InputObrazLewy->value();

	//wyszukanie numeru na liscie
	string szukany = InputObrazLewy->value();
	for (int i=0, s = ImageNames.size(); i<s; i++)
	{
		if (ImageNames.at(i)==szukany)
		{
			LeftIdx = i;
			break;
		}
	}
	//wyszukanie numeru na liscie koniec
	
	IdaStream.open(IdaFileName_Left.c_str()); //Przydzielamy strumieniowi zrodlo - plik .ida
	char NazwaPlikuObrazu[200];
	IdaStream.get(NazwaPlikuObrazu, 200, '\n'); //Wczytujemy pelna nazwe obrazu do zmiennej NazwaPlikuObrazu
	IdaStream.close(); //Zamykamy strumien
	LeftImage.ReadFromFile(IdaFileName_Left.c_str());
	
	load_Points_L();
	

	cvReleaseImage(&(StereoWindow->ImageL)); //Uwalniamy dotychczasowy obraz. Obraz jest skladnikiem klasy StereoWindow

	IplImage *TempIm = cvLoadImage(NazwaPlikuObrazu, -1);
	if (TempIm)
	{

		StereoWindow->ImageL = cvCreateImage(cvSize(TempIm->width,TempIm->height),TempIm->depth, TempIm->nChannels);
		if (TempIm->nChannels == 3) //Jezeli to jest obraz kolorowy
		{
			cvCvtColor(TempIm,StereoWindow->ImageL,CV_RGB2BGR);
		}
		else //Jezli obraz jest szary
		{
			cvCopy(TempIm, StereoWindow->ImageL); 
		}
		//if (StereoWindow->ImageL)
		//{
		//	loadTex_RGB(StereoWindow->ImageL, &StereoWindow->TexL);
		//	Toggle_ButtonPomiarLewy->activate();
		//}
		cvReleaseImage(&TempIm);
		// ********************************************
		
		//Uwolnienie dotychczasowych tekstur
		for (unsigned int i =0; i<StereoWindow->ImagesLeft.size(); i++)
		{
			cvReleaseImage(& StereoWindow->ImagesLeft.at(i));
		}

		if (StereoWindow->ImageL)
		{
			load_Textures(StereoWindow->ImageL, &StereoWindow->QuadsL, &StereoWindow->ImagesLeft, StereoWindow->TexturesL, &StereoWindow->NTL);
			StereoWindow->WL = StereoWindow->ImageL->width;
			StereoWindow->HL = StereoWindow->ImageL->height;
			StereoWindow->LoadedL = true;
			Toggle_ButtonPomiarLewy->activate();
		}
		else
		{
			StereoWindow->LoadedL = false;
			Toggle_ButtonPomiarLewy->deactivate();
		}

		cvReleaseImage(& (StereoWindow->ImageL));



		focus(StereoWindow); //Focus klawiatury na StereoWindow
		StereoWindow->redraw();

	}
	else
	{
		ImageNotFoundWindow->show();
	}
	
}

void ImagesShowWindow::cb_Input_ObrazPrawy (Fl_Widget *w, void *v)
{
	((ImagesShowWindow*)v)->cb_Input_ObrazPrawy_i();
}
void ImagesShowWindow::cb_Input_ObrazPrawy_i()
{
	
	//VTUNE
	ifstream IdaStream; // Tworzymy strumien do pliku .ida
	IdaFileName_Right = ((MainWindow*)win)->ProjectFolder +  InputObrazPrawy->value(); 
	
	//wyszukanie numeru na liscie
	string szukany = InputObrazPrawy->value();
	for (int i=0, s = ImageNames.size(); i<s; i++)
	{
		if (ImageNames.at(i)==szukany)
		{
			RightIdx = i;
			break;
		}
	}
	//wyszukanie numeru na liscie koniec


	IdaStream.open(IdaFileName_Right.c_str()); //Przydzielamy strumieniowi zrodlo - plik .ida
	char NazwaPlikuObrazu[200];
	IdaStream.get(NazwaPlikuObrazu, 200, '\n'); //Wczytujemy pelna nazwe obrazu do zmiennej NazwaPlikuObrazu
	IdaStream.close(); //Zamykamy strumien
	RightImage.ReadFromFile(IdaFileName_Right.c_str());

	load_Points_P();

	cvReleaseImage(&(StereoWindow->ImageR)); //Uwalniamy dotychczasowy obraz. Obraz jest skladnikiem klasy StereoWindow		
	
	IplImage *TempIm = cvLoadImage(NazwaPlikuObrazu,-1);
	if (TempIm)
	{
		StereoWindow->ImageR = cvCreateImage(cvSize(TempIm->width,TempIm->height),TempIm->depth, TempIm->nChannels);
		if (TempIm->nChannels == 3)
		{
			cvCvtColor(TempIm,StereoWindow->ImageR,CV_RGB2BGR);
		}
		else
		{
			cvCopy(TempIm,StereoWindow->ImageR);
		}
		//if(StereoWindow->ImageR)
		//{
		//	loadTex_RGB(StereoWindow->ImageR, &StereoWindow->TexR);
		//	Toggle_ButtonPomiarPrawy->activate();
		//}

		cvReleaseImage(&TempIm);

		//Uwolnienie dotychczasowych tekstur
		for (unsigned int i =0; i<StereoWindow->ImagesRight.size(); i++)
		{
			cvReleaseImage(& StereoWindow->ImagesRight.at(i));
		}


		if (StereoWindow->ImageR)
		{
			load_Textures(StereoWindow->ImageR, &StereoWindow->QuadsR, &StereoWindow->ImagesRight, StereoWindow->TexturesR, &StereoWindow->NTR);
			StereoWindow->WR = StereoWindow->ImageR->width;
			StereoWindow->HR = StereoWindow->ImageR->height;
			StereoWindow->LoadedR = true;
			Toggle_ButtonPomiarPrawy->activate();
		}
		else
		{
			StereoWindow->LoadedR = false;
			Toggle_ButtonPomiarPrawy->deactivate();
		}

		cvReleaseImage(&(StereoWindow->ImageR)); 


		focus(StereoWindow);
		StereoWindow->redraw();
	}
	else
	{
		ImageNotFoundWindow->show();
	}
	
}





void ImagesShowWindow::cb_Output_DrawingMode (Fl_Widget *w, void *v)
{
	((ImagesShowWindow*)v)->cb_Output_DrawingMode_i();
}
void ImagesShowWindow::cb_Output_DrawingMode_i()
{
	focus(StereoWindow);
}

int  ImagesShowWindow::handle(int e)
{
	 int ret = Fl_Window::handle(e);
	 string s1, s2, s3, s4, s5;
	 switch (e)
	 {
	 case FL_FOCUS:

		 s1 = "PRJ\\"; //Budujemy pelna nazwe pliku .ida
		 s2 = ((MainWindow*)win)->CurrentProjectName; //Nazwa aktualnego projektu
		 s3 = "\\";	
		 s4 = InputObrazLewy->value(); // nazwa pliku .ida wybrana z inputa
		 s5 = ".pix";
		 if (s4!="")
		 {
			 PixFileName_Left = s1 + s2.substr(0,s2.length()-4)+ s3 + s4.substr(0,s4.length()-4) + s5;
			 changePixStreamL(PixFileName_Left);
			 //cout <<"Zmieniam zrodlo odczytu i zapisu dla obrazu lewego na: " << PixFileName_Left <<endl;
		 }
		 s4 = InputObrazPrawy->value();
		 if (s4!="")
		 {
			PixFileName_Right = s1 + s2.substr(0,s2.length()-4)+ s3 + s4.substr(0,s4.length()-4) + s5;
			changePixStreamP(PixFileName_Right);
			 //cout <<"Zmieniam zrodlo odczytu i zapisu dla obrazu prawego na: " << PixFileName_Right<<endl;
		 }
		 ret  = 1;
		 break;
	 case FL_UNFOCUS:
		 ret  = 1;
		 break;
	 case FL_LEAVE:
		 //cout << "Mysz poza glownym oknem! " <<endl;
		 StereoWindow->CursorPos = 0;
		 ret = 1;
		 break;
	 case FL_KEYBOARD:
		switch (Fl::event_key())
		{
		case FL_Up:
			if (LeftIdx > 0)
			{
				InputObrazLewy->value(LeftIdx - 1);
				InputObrazLewy->do_callback();
			}
			ret = 1;
			break;
		case FL_Down:
			if (LeftIdx < ImageNames.size() - 1)
			{
				InputObrazLewy->value(LeftIdx + 1);
				InputObrazLewy->do_callback();
			}
			ret = 1;
			break;
		case FL_Left:
			if (StereoWindow->Mono == false)
			{
				if (RightIdx > 0)
				{
					InputObrazPrawy->value(RightIdx - 1);
					InputObrazPrawy->do_callback();
				}
			}
			ret = 1;
			break;
		case FL_Right:
			if (StereoWindow->Mono == false)
			{
				if (RightIdx < ImageNames.size() - 1)
				{
					InputObrazPrawy->value(RightIdx + 1);
					InputObrazPrawy->do_callback();
				}
			}
			ret = 1;
			break;
		}
		break;

	 }
	 return ret;
}

void ImagesShowWindow::cb_Toggle_ButtonPomiarLewy (Fl_Widget *w, void *v)
{
	((ImagesShowWindow*)v)->cb_Toggle_ButtonPomiarLewy_i();
}

void ImagesShowWindow::cb_Toggle_ButtonPomiarLewy_i()
{
	if ( Toggle_ButtonPomiarLewy->value()==1) 
	{
		Toggle_ButtonPomiarLewy->image(ButtonIcon1);
	}
	else
	{
		Toggle_ButtonPomiarLewy->image(ButtonIcon2);
	}
	Toggle_ButtonPomiarLewy->redraw();
}

void ImagesShowWindow::cb_Toggle_ButtonPomiarPrawy (Fl_Widget *w, void *v)
{
	((ImagesShowWindow*)v)->cb_Toggle_ButtonPomiarPrawy_i();
}

void ImagesShowWindow::cb_Toggle_ButtonPomiarPrawy_i()
{
	if ( Toggle_ButtonPomiarPrawy->value()==1) 
	{
		Toggle_ButtonPomiarPrawy->image(ButtonIcon1);
	}
	else
	{
		Toggle_ButtonPomiarPrawy->image(ButtonIcon2);
	}
	Toggle_ButtonPomiarPrawy->redraw();
}


void ImagesShowWindow::cb_Button_SM (Fl_Widget *w, void *v)
{
	((ImagesShowWindow*)v)->cb_Button_SM_i();
}

void ImagesShowWindow::cb_Button_SM_i()
{
	string s(Button_SM->label());
	if (s == "S")
	{
		Button_SM->color(fl_rgb_color(172,191,243));
		Button_SM->label("M");
		Toggle_ButtonPomiarPrawy->deactivate();
		Toggle_ButtonPomiarPrawy->value(0);
		Toggle_ButtonPomiarPrawy->image(ButtonIcon2);
		Toggle_ButtonPomiarPrawy->redraw();
		InputObrazPrawy->deactivate();
		StereoWindow->Mono = true;
		if (StereoWindow->DrawingMode > 2) //Mozliwy jest tylko tryb mono dla lewego okna
		{
			StereoWindow->set_DrawingMode(2);
		}
	}
	else
	{
		Button_SM->color(fl_rgb_color(247,181,53));
		Button_SM->label("S");
		Toggle_ButtonPomiarPrawy->activate();
		InputObrazPrawy->activate();
		StereoWindow->Mono = false;
	}
	this->redraw();
	StereoWindow->redraw();

}

void ImagesShowWindow::cb_Button_TMLRX (Fl_Widget *w, void *v)
{
	((ImagesShowWindow*)v)->cb_Button_TMLRX_i();
}

void ImagesShowWindow::cb_Button_TMLRX_i()
{
	string s(Button_TMLRX->label());
	if (s == "X") 
	{
		Button_TMLRX->label("T");
		Button_TMLRX->color(fl_rgb_color(247,181,53));
		OutputCoords->show();
		CoordsDispMode = 1;
	}
	
	if (s == "T") 
	{
		Button_TMLRX->label("M");
		Button_TMLRX->color(fl_rgb_color(243,15,243));
		OutputCoords->show();
		CoordsDispMode = 2;
	}
	
	if (s == "M") 
	{
		Button_TMLRX->label("L");
		Button_TMLRX->color(fl_rgb_color(53,227,53));
		OutputCoords->show();
		CoordsDispMode = 3;
	}
	
	if (s == "L") 
	{
		Button_TMLRX->label("R");
		Button_TMLRX->color(fl_rgb_color(53,227,53));
		OutputCoords->show();
		CoordsDispMode = 4;
	}

	if (s == "R") 
	{
		Button_TMLRX->label("X");
		Button_TMLRX->color(fl_rgb_color(206,206,206));
		OutputCoords->hide();
		CoordsDispMode = 0;
	}
}


