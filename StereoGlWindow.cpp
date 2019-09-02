#include "pch.h"
#include "StereoGlWindow.h"
#include "ImagesShowWindow.h"
#include "Graphics.h"

//#include <GL/glext.h>
using namespace std;

double StereoGlWindow::dX = 0.0010;
double StereoGlWindow::dY = 0.0010;
double StereoGlWindow::dZ = 0.0010;
double StereoGlWindow::SnapSens =0.1;
Graphics StereoGlWindow::Grafika;

StereoGlWindow::StereoGlWindow(int X,int Y,int W,int H,const char*L ): Fl_Gl_Window(X,Y,W,H,L)
{
	WWidth = W; //Tymczasowe!
	WHeight = H;

	Zoom = 1;
	QuadSize = 256;
	MipmapLevel = 0;

	HFactor = 1;

	SnapSens = 0.100;
	ModPoint.X = 0;
	ModPoint.Y = 0;
	ModPoint.Z = 0;
	TerPoint.X = 0;
	TerPoint.Y = 0;
	TerPoint.Z = 0;
	SnapCoords[0] = 0.0;
	SnapCoords[1] = 0.0;
	SnapCoords[2] = 0.0;
	TerMov = false;
	DrwClicks = 0;
	ShowDrwLine = true;
	MakeTransfer = false;
	StereoMode = 1;

	NTL = 0;
	NTR = 0;

	RzutLewy.left = 0;
	RzutLewy.right = RzutLewy.left + WWidth/Zoom;	
	RzutLewy.bottom = 0;
	RzutLewy.top = RzutLewy.bottom + WHeight/Zoom;
	RzutLewy.nearr = -1;
	RzutLewy.farr =1;
	RzutPrawy = RzutLewy;
	DrawingMode = 0;
	DrawingState = false;
	FeatureMode = 0; //Na poczatku linie
	
	DrawPoints = true;

	ImageL = NULL; //Inicjalizacja wskaznikow do obrazow
	ImageR = NULL;

	TilesL = NULL;
	TilesR = NULL;

	LoadedL = false;
	LoadedR = false;

	for (int k = 0; k<4; k++)
	{
		Cross[k] = 0;
	}
	Mono = false;
	


}

StereoGlWindow::~StereoGlWindow(void)
{
	cvReleaseImage(&ImageL);
	cvReleaseImage(&ImageR);
}

void StereoGlWindow::draw() 
{
    // First time? init viewport, etc.
    if (!valid()) 
	{
        valid(1);
		glViewport(0,0,w(),h());
		glLoadIdentity();
		glOrtho(0, w(), 0, h(),-1, 1);
    }
	
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_MULTISAMPLE);
	if ((StereoMode == 2)) //Stereo Anaglif
	{
		RzutLewy.right = RzutLewy.left + w()/Zoom;
		RzutLewy.top = RzutLewy.bottom + h()/Zoom;
		RzutLewy.nearr = -1;
		RzutLewy.farr = 1;
		RzutPrawy.right = RzutPrawy.left + w()/Zoom;
		RzutPrawy.top = RzutPrawy.bottom + h()/Zoom;
		RzutPrawy.nearr = -1;
		RzutPrawy.farr = 1;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		glOrtho(RzutLewy.left, RzutLewy.right, RzutLewy.bottom, RzutLewy.top,-2,2);
		glViewport(0, 0, w(), h());


		
		glColor3f(1.0, 1.0, 1.0);
	
		CursorX = (w()/2.0) + 0.5;
		CursorY = (h()/2.0) + 0.5;

		glColorMask(true, false, false, false);
		
		//RYSOWANIE SCENY LEWEJ - START
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glColor3f(1.0, 1.0, 1.0);

		draw_textures(&QuadsL, TexturesL,-1, &NTL);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();		
		glOrtho(0, w(), 0, h(), -1, 1);
		
		if (DrawPoints == true)
		{
			draw_points_L();
		}



			
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();		
		glOrtho(0, w(), 0, h(), -1, 1);
		
		draw_cursor(CursorX,CursorY,Grafika.Cross_Size);
		
		
		//glClear(GL_DEPTH_BUFFER_BIT) ;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(RzutPrawy.left, RzutPrawy.right, RzutPrawy.bottom, RzutPrawy.top,-2,2);		
		
		glColorMask(false,true, true, false);
		
		//RYSOWANIE SCENY PRAWEJ - START
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1.0, 1.0, 1.0);

		draw_textures(&QuadsR, TexturesR,-1, &NTR);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();		
		glOrtho(0, w(), 0, h(), -1, 1);
		
		if (DrawPoints == true)
		{
			draw_points_P();
		}

		//RYSOWANIE SCENY PRAWEJ - END
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w(), 0, h(), -1, 1);
		draw_cursor(CursorX,CursorY,Grafika.Cross_Size);
		
		glColorMask(true, true, true, true);

	}
	else
	{
		if (Mono)
		{
			RzutLewy.right = RzutLewy.left + w()/Zoom;
		}
		else
		{
			RzutLewy.right = RzutLewy.left + (w()/2.0)/Zoom;
		}
		RzutLewy.top = RzutLewy.bottom + h()/Zoom;
		RzutLewy.nearr = -1;
		RzutLewy.farr = 1;
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(RzutLewy.left, RzutLewy.right, RzutLewy.bottom, RzutLewy.top,-1,1);
		if (Mono)
		{
			glViewport(0, 0, w(), h());
		}
		else
		{
			glViewport(0, 0, w()/2.0, h());
		}
		
		//RYSUJEMY OBRAZ LEWY
		if (LoadedL==true)
		{
			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			draw_textures(&QuadsL, TexturesL,-1,&NTL);
		}		

		if (Mono == false)
		{
			RzutPrawy.right = RzutPrawy.left + (w()/2.0)/Zoom;
			RzutPrawy.top = RzutPrawy.bottom + h()/Zoom;
			RzutPrawy.nearr = -1;
			RzutPrawy.farr = 1;

			glLoadIdentity();
			glOrtho(RzutPrawy.left, RzutPrawy.right, RzutPrawy.bottom, RzutPrawy.top,-1,1);
			glViewport(w()/2, 0, w()/2, h()); //aby cale okno bylo wypelnione
			//RYSUJEMY OBRAZ PRAWY
			if (LoadedR == true)
			{
				glColor3f(1.0, 1.0, 1.0);
				glEnable(GL_TEXTURE_2D);
				draw_textures(&QuadsR, TexturesR,-1, &NTR);
				
			}
		}

		glDisable(GL_TEXTURE_2D);

		glViewport(0,0,w(),h());
		glLoadIdentity();
		glOrtho(0, w(), 0, h(),-1, 1);
		glColor3f(1.0, 0.0, 0.0);
		//Rysujemy linie przez srodek
		if (Mono == false)
		{
			glBegin(GL_LINES); 
				glVertex3f(w()/2.0, 0, 1);
				glVertex3f(w()/2.0, h(), 1);
			glEnd();
		}

		if (disp_info)
		{
			gl_font(1, 11);
			glColor3f(0.5, 1.0, 0.0);
			glRasterPos3f(10, h() - 20, 1); gl_draw("F1: exit roam");
			glRasterPos3f(10, h() - 35, 1); gl_draw("F2: roam left");
			glRasterPos3f(10, h() - 50, 1); gl_draw("F3: roam right");
			glRasterPos3f(10, h() - 65, 1); gl_draw("F4: roam both");
			glRasterPos3f(10, h() - 80, 1); gl_draw("F5: autogrammetric");
			glRasterPos3f(10, h() - 95, 1); gl_draw("TAB: left<->right");
			glRasterPos3f(10, h() - 110, 1); gl_draw("1: zoom in");
			glRasterPos3f(10, h() - 125, 1); gl_draw("2: zoom out");
			glRasterPos3f(10, h() - 140, 1); gl_draw("DEL: delete point");
			glRasterPos3f(10, h() - 155, 1); gl_draw("N: rename point");
			glRasterPos3f(10, h() - 170, 1); gl_draw("scroll: zoom in/out");
			glRasterPos3f(10, h() - 185, 1); gl_draw("scroll: z-movement if F5");
			glRasterPos3f(10, h() - 200, 1); gl_draw("lmb: measure point");
			glRasterPos3f(10, h() - 215, 1); gl_draw("F6: hide/show this info");
		}
		glColor3f(1.0, 0.0, 0.0);

		//Rysujemy kursory pomiarowe 
		if (Mono)
		{
			CursorX = (w()/2.0) + 0.5;
		}
		else
		{
			CursorX = (w()/4.0) + 0.5;
		}
		CursorY = (h()/2.0) + 0.5;
		

		glLoadIdentity();
		if (Mono)
		{
			glOrtho(0, w(), 0, h(), -1, 1);
			glViewport(0, 0, w(), h());
		}
		else
		{
			glOrtho(0, w()/2.0, 0, h(), -1, 1);
			glViewport(0, 0, w()/2.0, h());
		}
		
		draw_cursor(CursorX,CursorY,Grafika.Cross_Size);
		
		if (LoadedL==true) 
		{
			//Rysujemy pomierzone punkty
			
			if (DrawPoints == true)
			{
				draw_points_L();
			}


		}
		
		if (Mono == false)
		{
			glLoadIdentity();	
			glOrtho(0, w()/2, 0, h(), -1, 1);
			glViewport(w()/2, 0, w()/2, h());
				
			draw_cursor(CursorX,CursorY,Grafika.Cross_Size );

			if (LoadedR == true) 
			{
				//Rysujemy pomierzone punkty
				if (DrawPoints == true)
				{
					draw_points_P();
				}
			}
		}
	} //do if ((DrawingMode == 5) && (StereoMode == 2))
}

void StereoGlWindow::resize(int X,int Y,int W,int H) 
{
    Fl_Gl_Window::resize(X,Y,W,H);
    
	WWidth = W;
	WHeight = H;
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, W, H);	
	glOrtho(0, W-1, 0, H-1, -1, 1);

	redraw();
}


void StereoGlWindow::draw_cursor (double x, double y, short size)
{

	glColor3f(Grafika.Cursor_Color[0],Grafika.Cursor_Color[1],Grafika.Cursor_Color[2]);
	glBegin(GL_LINES);
		glVertex2f(x - size, y);
		glVertex2f(x + size+1 , y);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(x, y - size);
		glVertex2f(x, y + size+1);
	glEnd();

	if (Grafika.Circular_Cursor==true)
	{
		int r = Grafika.Circle_Radius;
		float pi = 3.1415927;
		float cx1, cy1;
		float cx2, cy2;
		for (int i=0; i<=100; i++)
		{
			
			cx1 = x + r*cos(2*i*pi/100);
			cy1 = y + r*sin(2*i*pi/100);
			cx2 = x + r*cos(2*(i+1)*pi/100);
			cy2 = y + r*sin(2*(i+1)*pi/100);
			glBegin(GL_LINES);
				glVertex2f(cx1, cy1);
				glVertex2f(cx2, cy2);
			glEnd();

		}

	}
}

void StereoGlWindow::draw_textures(vector <TextureQuad> *Quadsy, GLuint *Tekstury, double depth, unsigned int* N )
{
	for (unsigned int i=0; i< *N; i++)
	{
		glBindTexture( GL_TEXTURE_2D, Tekstury[i] );
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3i((*Quadsy)[i].X1,(*Quadsy)[i].Y1 ,depth);
			glTexCoord2f(0.0, 1.0); glVertex3i((*Quadsy)[i].X1,(*Quadsy)[i].Y2, depth);
			glTexCoord2f(1.0, 1.0); glVertex3i((*Quadsy)[i].X2,(*Quadsy)[i].Y2, depth);
			glTexCoord2f(1.0, 0.0); glVertex3i((*Quadsy)[i].X2,(*Quadsy)[i].Y1, depth);
		glEnd();
	}
}

void StereoGlWindow::draw_points_L ()
{
	short d = 4;
	
	for (int i=0; i<LeftPoints.size(); i++)
	{
		double X = (int)( (LeftPoints[i].X + WL/2 - RzutLewy.left)*Zoom + 0.5);
		double Y = (int)( (LeftPoints[i].Y + HL/2 - RzutLewy.bottom)*Zoom +0.5);
	
		switch (LeftPoints[i].Type)
		{
			case 0:
			glColor3f(0.0,1.0,0.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1); 
				glVertex3f(X+d,Y+d,1); 
			glEnd();

			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( LeftPoints[i].Name.c_str());
			break;

			case 5: //Linia wiazaca
			glColor3f(1.0,0.1,1.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1); 
				glVertex3f(X+d,Y+d,1); 
			glEnd();
			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( LeftPoints[i].Name.c_str());
			gl_font(1,9);
			glRasterPos3f(X+d+2,Y-d-2,1); gl_draw( LeftPoints[i].LineId.c_str());
			break;

			case 6: //Linia wiazaca pozioma
			glColor3f(1.0,0.1,1.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1); 
				glVertex3f(X+d,Y+d,1); 
			glEnd();

			glLineWidth(1);
			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d+2,1);
				glVertex3f(X+d,Y+d+2,1);
			glEnd();
			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( LeftPoints[i].Name.c_str());
			gl_font(1,9);
			glRasterPos3f(X+d+2,Y-d-2,1); gl_draw( LeftPoints[i].LineId.c_str());
			break;

			case 7: //Linia wiazaca pozioma
			glColor3f(1.0,0.1,1.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1); 
				glVertex3f(X+d,Y+d,1); 
			glEnd();

			glLineWidth(1);
			glBegin(GL_LINES);
				glVertex3f(X-d-2,Y+d,1);
				glVertex3f(X-d-2,Y-d,1);
			glEnd();
			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( LeftPoints[i].Name.c_str());
			gl_font(1,9);
			glRasterPos3f(X+d+2,Y-d-2,1); gl_draw( LeftPoints[i].LineId.c_str());
			break;
			
			case 3:
			glColor3f(1.0,0.0,0.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1);
				glVertex3f(X+d,Y+d,1);
			glEnd();
			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( LeftPoints[i].Name.c_str());
			break;
		}
	}
	glLineWidth(1);	
}

void StereoGlWindow::draw_points_P ()
{
	short d=4;
	for (int i=0; i<RightPoints.size(); i++)
	{

		double X = (int)( (RightPoints[i].X + WR/2.0 - RzutPrawy.left)*Zoom + 0.5 );
		double Y = (int)( (RightPoints[i].Y + HR/2.0 - RzutPrawy.bottom)*Zoom + 0.5);
		
		switch (RightPoints[i].Type)
		{
			case 0:
			glColor3f(0.0,1.0,0.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1); 
				glVertex3f(X+d,Y+d,1); 
			glEnd();

			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( RightPoints[i].Name.c_str());
			break;

			case 5: //Linia wiazaca
			glColor3f(1.0,0.1,1.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1); 
				glVertex3f(X+d,Y+d,1); 
			glEnd();
			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( RightPoints[i].Name.c_str());
			gl_font(1,9);
			glRasterPos3f(X+d+2,Y-d-2,1); gl_draw( RightPoints[i].LineId.c_str());
			break;

			case 6: //Linia wiazaca pozioma
			glColor3f(1.0,0.1,1.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1); 
				glVertex3f(X+d,Y+d,1); 
			glEnd();


			glLineWidth(1);
			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d+2,1);
				glVertex3f(X+d,Y+d+2,1);
			glEnd();
			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( RightPoints[i].Name.c_str());
			gl_font(1,9);
			glRasterPos3f(X+d+2,Y-d-2,1); gl_draw( RightPoints[i].LineId.c_str());
			break;

			case 7: //Linia wiazaca pozioma
			glColor3f(1.0,0.1,1.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1); 
				glVertex3f(X+d,Y+d,1); 
			glEnd();

			glLineWidth(1);
			glBegin(GL_LINES);
				glVertex3f(X-d-2,Y+d,1);
				glVertex3f(X-d-2,Y-d,1);
			glEnd();
			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw(RightPoints[i].Name.c_str());
			gl_font(1,9);
			glRasterPos3f(X+d+2,Y-d-2,1); gl_draw( RightPoints[i].LineId.c_str());
			break;
			
			case 3:
			glColor3f(1.0,0.0,0.0);
			glLineWidth(2);

			glBegin(GL_LINES);
				glVertex3f(X-d,Y+d,1);
				glVertex3f(X+d,Y-d,1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(X-d,Y-d,1);
				glVertex3f(X+d,Y+d,1);
			glEnd();
			gl_font(1,10);
			glRasterPos3f(X+d+2,Y+d+2,1); gl_draw( RightPoints[i].Name.c_str());
			break;
		}

	}
	glLineWidth(1);
}


void StereoGlWindow::set_DrawingMode(short Mode)
{
	PreviousDrawingMode = Mode;
	DrawingMode = Mode;
	switch(Mode)
	{
	case 1:
		cursor(FL_CURSOR_DEFAULT);
		//cursor(FL_CURSOR_CROSS);
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->color(FL_RED);
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->value("N");
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->redraw();
		break;
	case 2:
		cursor(FL_CURSOR_NONE);
		//cursor(FL_CURSOR_CROSS);
		CursorPos = 2;			
		//Ustawiamy kursor tak aby wypadl dokladnie w srodku okienka lewego
		SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x() +  (int)(w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );		
		Ref[0] = (int)(w()/4+0.5);
		Ref[1] = (int)(h()/2+0.5);
		// Zapisujemy wspolrzedne lewego dolnego naroznika rzutu orto w momencie przejscia 
		// w tryb, aby potem moc sie do nich odwolac w momencie przejscia na nowy orto
		OrtRef[0] = RzutLewy.left; 
		OrtRef[1] = RzutLewy.bottom; 
		//Ustawienia oznaczen
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->color(FL_YELLOW);
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->value("L");
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->redraw();
		break;
	case 3:
		//cursor(FL_CURSOR_NONE);

		cursor(FL_CURSOR_CROSS);
		CursorPos = 2;		
		//Ustawiamy kursor tak aby wypadl dokladnie w srodku okienka lewego
		SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x()+ (int)(3*w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );
		Ref[2] = (int)(3*w()/4+0.5);
		Ref[3] = (int)(h()/2+0.5);
		// Zapisujemy wspolrzedne lewego dolnego naroznika rzutu orto w momencie przejscia 
		// w tryb, aby potem moc sie do nich odwolac w momencie przejscia na nowy orto
		OrtRef[2] = RzutPrawy.left; 
		OrtRef[3] = RzutPrawy.bottom; 
		//Ustawienia oznaczen
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->color(FL_YELLOW);
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->value("R");
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->redraw();
		break;
	case 4:
		CursorPos = 2;
		cursor(FL_CURSOR_NONE);
		//Ustawiamy kursor tak aby wypadl dokladnie w srodku okienka lewego
		SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x()+ (int)(w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );
		Ref[0] = (int)(w()/4 + 0.5);
		Ref[1] = (int)(h()/2 + 0.5);
		// Zapisujemy wspolrzedne lewego dolnego naroznika rzutu orto w momencie przejscia 
		// w tryb, aby potem moc sie do nich odwolac w momencie przejscia na nowy orto
		OrtRef[0] = RzutLewy.left;
		OrtRef[1] = RzutLewy.bottom;
		OrtRef[2] = RzutPrawy.left; 
		OrtRef[3] = RzutPrawy.bottom; 
		//Ustawienia oznaczen
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->color(FL_GREEN);
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->value("S");
		((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->redraw();
		break;
		
	}
}

void StereoGlWindow::send_CoordsToDisp(void)
{
	switch (((ImagesShowWindow*)ParentWindow)->CoordsDispMode)
	{
	case 1: //Terenowe
		((ImagesShowWindow*)ParentWindow)->DispCoords(TerPoint.X, TerPoint.Y, TerPoint.Z);
		break;
	case 2: //Modelu
		((ImagesShowWindow*)ParentWindow)->DispCoords(ModPoint.X, ModPoint.Y, ModPoint.Z);
		break;
	case 3: //Lewe zdjecie
		((ImagesShowWindow*)ParentWindow)->DispCoords(Cross[0], Cross[1]);
		break;
	case 4: //Prawe zdjecie
		((ImagesShowWindow*)ParentWindow)->DispCoords(Cross[2], Cross[3]);
		break;
	}

}


void StereoGlWindow::calc_CrossPos(void)
{
		//Aktualizacja wspolrzednych obrazowych kursora
		if (LoadedL == true)
		{
			if (Mono || StereoMode == 2)
			{
				Cross[0] = RzutLewy.left + (w()/2.0+0.5)/Zoom - WL/2.0;//Dla zdjecia lewego
			}
			else
			{
				Cross[0] = RzutLewy.left + (w()/4.0+0.5)/Zoom - WL/2.0;//Dla zdjecia lewego
			}
			Cross[1] = RzutLewy.bottom + (h()/2.0+0.5)/Zoom - HL/2.0;
			if (Cross[0] < -WL || Cross[0] > WL || Cross[1] < -HL || Cross[1] > HL)
			{
				IsFarFromImageL = true;
			}
			else
			{
				IsFarFromImageL = false;
			}

		}
		if (LoadedR == true)
		{
			if (StereoMode == 2)
			{
				Cross[2] = RzutPrawy.left + (w()/2.0+0.5)/Zoom - WR/2.0;//Dla zdjecia prawego
				Cross[3] = RzutPrawy.bottom + (h()/2.0+0.5)/Zoom - HR/2.0;
			}
			else
			{
				Cross[2] = RzutPrawy.left + (w()/4.0+0.5)/Zoom - WR/2.0;//Dla zdjecia prawego
				Cross[3] = RzutPrawy.bottom + (h()/2.0+0.5)/Zoom - HR/2.0;
			}
			if (Cross[2] < -WR || Cross[2] > WR || Cross[3] < -HR || Cross[3] > HR)
			{
				IsFarFromImageR = true;
			}
			else
			{
				IsFarFromImageR = false;
			}

		}
}


void StereoGlWindow::calc_Rzut(double XL, double YL, double XP, double YP)
{
		//Aktualizacja polorzenia rastra ze wzgledu na dane wsp. na obu zdjeciach
		if (LoadedL == true)
		{
			if (Mono || StereoMode == 2)
			{
				RzutLewy.left = XL - (w()/2.0+0.5)/Zoom + WL/2.0;//Dla zdjecia lewego
			}
			else
			{
				RzutLewy.left = XL - (w()/4.0+0.5)/Zoom + WL/2.0;//Dla zdjecia lewego
			}
			RzutLewy.bottom = YL - (h()/2.0+0.5)/Zoom + HL/2.0;
		}
		if (LoadedR == true)
		{
			if (StereoMode == 2)
			{
				RzutPrawy.left = XP - (w()/2.0+0.5)/Zoom + WR/2.0;//Dla zdjecia prawego
				RzutPrawy.bottom = YP - (h()/2.0+0.5)/Zoom + HR/2.0;
			}
			else
			{
				RzutPrawy.left = XP - (w()/4.0+0.5)/Zoom + WR/2.0;//Dla zdjecia prawego
				RzutPrawy.bottom = YP - (h()/2.0+0.5)/Zoom + HR/2.0;
			}
		}
}



void StereoGlWindow::zoomIn(void)
{
			if (Zoom < 16)
			{
				Zoom = Zoom*2;
				if (Zoom <= 1)
				{
					MipmapLevel--;
					QuadSize = QuadSize / 2;
					
				}
				if (Mono)
				{
					RzutLewy.left = (int) (RzutLewy.left + ((w()/2 + 0.5)/Zoom  ));
					RzutLewy.bottom = (int) (RzutLewy.bottom +  ((h()/2 + 0.5)/Zoom  ));
					//if (StereoMode == 2)
					//{
						RzutPrawy.left = (int) (RzutPrawy.left + ((w()/2 + 0.5)/Zoom  ));
						RzutPrawy.bottom = (int) (RzutPrawy.bottom +  ((h()/2 + 0.5)/Zoom  ));
					//}
				}
				else
				{
					RzutLewy.left = (int) (RzutLewy.left + ((w()/4 + 0.5)/Zoom  ));
					RzutLewy.bottom = (int) (RzutLewy.bottom +  ((h()/2 + 0.5)/Zoom  ));
					RzutPrawy.left = (int) ( RzutPrawy.left +  ((w()/4 + 0.5)/Zoom  ));
					RzutPrawy.bottom = (int) (RzutPrawy.bottom +  ((h()/2 + 0.5)/Zoom  ));				
				}
				OrtRef[0] = RzutLewy.left;
				OrtRef[1] = RzutLewy.bottom;
				if (Mono==false || StereoMode == 2)
				{
					OrtRef[2] = RzutPrawy.left;
					OrtRef[3] = RzutPrawy.bottom;
				}
				SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x()+ (int)(w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );
			}
			else
			{
				Zoom = 16;
			}
			{							
				string s1,s2,s3;
				ostringstream string_zoom;
				string_zoom.clear();
				if (Zoom <= 1) 
				{
					string_zoom << 1/Zoom;
					s2 = string_zoom.str();					
					s1 = "1:"; 
					s3 = s1+s2;
				}
				else
				{
					string_zoom << Zoom;
					s2 = string_zoom.str();					
					s1 = ":1"; 
					s3 = s2+s1;
				}
				//cout<<"Zoom wynosi: " << s3.c_str() << endl;
				((ImagesShowWindow*)ParentWindow)->OutputZoom->value(s3.c_str());
				((ImagesShowWindow*)ParentWindow)->OutputZoom->redraw();
				//cout<<"Zoom: " <<Zoom<<" Mipmaplevel: " << MipmapLevel <<" QuadSize: "<< QuadSize <<endl;
			}
}

void StereoGlWindow::zoomOut(void)
{
			if (Zoom > 0.125)
			{

				Zoom = Zoom/2;
				if (Zoom < 1)
				{
					MipmapLevel++;
					QuadSize = QuadSize * 2;
					

				}

				if (Mono)
				{
					RzutLewy.left = (int) (RzutLewy.left - (w()/4 +0.5)/Zoom);
					RzutLewy.bottom = (int) (RzutLewy.bottom -  (h()/4+0.5)/Zoom) ;
					if (StereoMode == 2)
					{
						RzutPrawy.left = (int) (RzutPrawy.left - (w()/4 +0.5)/Zoom);
						RzutPrawy.bottom = (int) (RzutPrawy.bottom -  (h()/4+0.5)/Zoom) ;
					}
				}
				else
				{
					RzutLewy.left = (int) (RzutLewy.left - (w()/8 +0.5)/Zoom);
					RzutLewy.bottom = (int) (RzutLewy.bottom -  (h()/4+0.5)/Zoom) ;
					RzutPrawy.left = (int) (RzutPrawy.left - (w()/8+0.5)/Zoom) ;
					RzutPrawy.bottom = (int) (RzutPrawy.bottom - (h()/4+0.5)/Zoom) ;
				}
				OrtRef[0] = RzutLewy.left;
				OrtRef[1] = RzutLewy.bottom;
				if (Mono == false || StereoMode == 2)
				{
					OrtRef[2] = RzutPrawy.left;
					OrtRef[3] = RzutPrawy.bottom;
				}
				SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x()+ (int)(w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );
				
			}
			else
			{
				Zoom = 0.125;
			}

			{							
				string s1,s2,s3;				
				ostringstream string_zoom;
				string_zoom.clear();
				if (Zoom <= 1) 
				{
					string_zoom << 1/Zoom;
					s2 = string_zoom.str();					
					s1 = "1:"; 
					s3 = s1+s2;
				}
				else
				{
					string_zoom << Zoom;
					s2 = string_zoom.str();					
					s1 = ":1"; 
					s3 = s2+s1;
				}
				((ImagesShowWindow*)ParentWindow)->OutputZoom->value(s3.c_str());
				((ImagesShowWindow*)ParentWindow)->OutputZoom->redraw();
				//cout<<"Zoom: " <<Zoom<<" Mipmaplevel: " << MipmapLevel <<" QuadSize: "<< QuadSize <<endl;
			}
}

int StereoGlWindow::handle(int e)
{
	 int ret = Fl_Gl_Window::handle(e);
	 switch (e)
	 {
	 case FL_FOCUS:
		 ret  = 1;
		 break;
	 case FL_UNFOCUS:
		 ret  = 1;
		 break;
	 case FL_KEYBOARD:
		switch (Fl::event_key())
		{
		case FL_F + 1 :
			if (DrawingState==false) //Nie mozna wyjsc z okna jezeli jestesmy w trakcie rysowania!!!
			{
				set_DrawingMode(1);
			}
			ret  = 1;
			break;
		case FL_F + 2 : 
			set_DrawingMode(2);
			ret  = 1; 
			break;
		case FL_F + 3 :
			if (Mono==false || StereoMode == 2) set_DrawingMode(3);
			ret = 1; 
			break;
		case FL_F + 4 :
			if (LoadedL && LoadedR) 
			{
				if (Mono == false || StereoMode == 2) set_DrawingMode(4);
			}
			ret = 1;
			break;

		case FL_F + 6:
			disp_info = !disp_info;
			redraw();
			break;
		case FL_Tab :
			if (Mono==false)
			{
				if (DrawingMode == 2) 
				{
					set_DrawingMode(3);
					((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarLewy->value(0);
					((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarLewy->image(((ImagesShowWindow*)ParentWindow)->ButtonIcon2);			
					((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarPrawy->value(1);
					((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarPrawy->image(((ImagesShowWindow*)ParentWindow)->ButtonIcon1);
				}
				else
				{
					//if (DrawingMode == 3) 
					//{
						set_DrawingMode(2);
						((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarLewy->value(1);
						((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarLewy->image(((ImagesShowWindow*)ParentWindow)->ButtonIcon1);
						((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarPrawy->value(0);
						((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarPrawy->image(((ImagesShowWindow*)ParentWindow)->ButtonIcon2);					
					//}
				}
			}
			ret = 1;
			break;
		case '1' :
			zoomIn();
			ret = 1;
			redraw();
			break;
		case '2' :
			zoomOut();
			ret = 1;
			redraw();
			break;
		case FL_Delete:
			
			if (  (((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarLewy->value() == 1 ) && (LoadedL==true) && (((ImagesShowWindow*)ParentWindow)->LeftImage.NPoints > 0)  )
			{
				double X,Y;
				if (Mono)
				{
					X = RzutLewy.left + (w()/2+0.5)/Zoom - WL/2;
				}
				else
				{
					X = RzutLewy.left + (w()/4+0.5)/Zoom - WL/2;
				}
				Y = RzutLewy.bottom + (h()/2+0.5)/Zoom - HL/2;
				double D;
				int N = fT_findColsestImpoint(  &LeftPoints, X, Y, &D);
				//cout <<"Punkt najblizszy: " << ((ImagesShowWindow*)ParentWindow)->LeftPoints[N].Name <<" Odleglosc: " << D <<endl;
				if ( D*Zoom < 10)
				{
					vector <ImagePoint>::iterator it_P;
					it_P = LeftPoints.begin() + N;
					LeftPoints.erase(it_P);

					ImagePoint::StrumienDoZapisu_L.close(); //Zamknij dotychczasowy strumien do zapisu
					//Otworz plik w trybie ios::out - cala wartosc jest tracona
					ImagePoint::StrumienDoZapisu_L.open( ((ImagesShowWindow*)ParentWindow)->PixFileName_Left.c_str(), ios::out);
					for (int i=0; i<((ImagesShowWindow*)ParentWindow)->LeftImage.NPoints - 1; i++)
					{
						LeftPoints[i].write_L();
					}
					ImagePoint::StrumienDoZapisu_L.close(); //Zamykamy strumien
					//Otwieramy strumien w trybie ios::ate - mozna znow do niego dopisywac
					ImagePoint::StrumienDoZapisu_L.open( ((ImagesShowWindow*)ParentWindow)->PixFileName_Left.c_str(), ios::app);
				
					((ImagesShowWindow*)ParentWindow)->LeftImage.NPoints --; //Zmniejszamy ilosc pomierzonych punktow o 1
					((ImagesShowWindow*)ParentWindow)->LeftImage.WriteToFile( ((ImagesShowWindow*)ParentWindow)->IdaFileName_Left.c_str() ); // Zapisujemy dane o obrazie w pliku .ida
				}

			}
			
			if (  ((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarPrawy->value() == 1 && (LoadedR==true) && (((ImagesShowWindow*)ParentWindow)->RightImage.NPoints > 0))
			{
				double X = RzutPrawy.left + (w()/4+0.5)/Zoom - WR/2;
				double Y = RzutPrawy.bottom + (h()/2+0.5)/Zoom - HR/2;
				double D;
				int N = fT_findColsestImpoint(  &RightPoints, X, Y, &D);
				if ( D*Zoom < 10)
				{
					vector <ImagePoint>::iterator it_P;
					it_P = RightPoints.begin() + N;
					RightPoints.erase(it_P);

					ImagePoint::StrumienDoZapisu_P.close(); //Zamknij dotychczasowy strumien do zapisu
					//Otworz plik w trybie ios::out - cala wartosc jest tracona
					ImagePoint::StrumienDoZapisu_P.open( ((ImagesShowWindow*)ParentWindow)->PixFileName_Right.c_str(), ios::out);
					for (int i=0; i<((ImagesShowWindow*)ParentWindow)->RightImage.NPoints - 1; i++)
					{
						RightPoints[i].write_P();
					}
					ImagePoint::StrumienDoZapisu_P.close(); //Zamykamy strumien
					//Otwieramy strumien w trybie ios::ate - mozna znow do niego dopisywac
					ImagePoint::StrumienDoZapisu_P.open( ((ImagesShowWindow*)ParentWindow)->PixFileName_Right.c_str(), ios::app);

					((ImagesShowWindow*)ParentWindow)->RightImage.NPoints --; //Zmniejszamy ilosc pomierzonych punktow o 1
					((ImagesShowWindow*)ParentWindow)->RightImage.WriteToFile( ((ImagesShowWindow*)ParentWindow)->IdaFileName_Right.c_str() ); // Zapisujemy dane o obrazie w pliku .ida
				}
			}
				

			
			redraw();
			ret = 1;
			break;
			
			case 110: //litera n
		
				if ((((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarLewy->value() == 1) && (LoadedL == true) && (((ImagesShowWindow*)ParentWindow)->LeftImage.NPoints > 0))
				{
					double X, Y;
					if (Mono)
					{
						X = RzutLewy.left + (w() / 2 + 0.5) / Zoom - WL / 2;
					}
					else
					{
						X = RzutLewy.left + (w() / 4 + 0.5) / Zoom - WL / 2;
					}
					Y = RzutLewy.bottom + (h() / 2 + 0.5) / Zoom - HL / 2;
					double D;
					int N = fT_findColsestImpoint(&LeftPoints, X, Y, &D);
					
					if (D*Zoom < 10)
					{
						PreviousDrawingMode = DrawingMode;
						DrawingMode = 1; //Bez trybu pomiaru
						cursor(FL_CURSOR_DEFAULT);
						((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->color(FL_RED);
						((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->value("N");
						((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->redraw();
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->set_modal();
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Output_Point_Name_Old->value(LeftPoints.at(N).Name.c_str());
						switch (LeftPoints.at(N).Type)
						{
						case 0:
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Output_Point_Type_Old->value("Tie");
							break;
						case 3:
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Output_Point_Type_Old->value("Control");
							break;
						default:
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Output_Point_Type_Old->value("Other");
							break;
						}
						
						string NewId(((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Input_Point_Name_New->value());
						if (NewId == "")
						{
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Button_Ok->deactivate();
						}
						else
						{
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Button_Ok->activate();
						}

						//przekazanie do okna zmiany nazwy punktu indeksu punktu, ktorego nazwe trzeba zmienic
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Idx_Point_To_Rename = N;
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Input_Point_Name_New->take_focus();
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->show();
						
					
					}

				}

				if (((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarPrawy->value() == 1 && (LoadedR == true) && (((ImagesShowWindow*)ParentWindow)->RightImage.NPoints > 0))
				{
					double X = RzutPrawy.left + (w() / 4 + 0.5) / Zoom - WR / 2;
					double Y = RzutPrawy.bottom + (h() / 2 + 0.5) / Zoom - HR / 2;
					double D;
					int N = fT_findColsestImpoint(&RightPoints, X, Y, &D);
					if (D*Zoom < 10)
					{
						PreviousDrawingMode = DrawingMode;
						DrawingMode = 1; //Bez trybu pomiaru
						cursor(FL_CURSOR_DEFAULT);
						((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->color(FL_RED);
						((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->value("N");
						((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->redraw();
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->set_modal();
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Output_Point_Name_Old->value(RightPoints.at(N).Name.c_str());
						switch (RightPoints.at(N).Type)
						{
						case 0:
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Output_Point_Type_Old->value("Tie");
							break;
						case 3:
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Output_Point_Type_Old->value("Control");
							break;
						default:
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Output_Point_Type_Old->value("Other");
							break;
						}

						string NewId(((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Input_Point_Name_New->value());
						if (NewId == "")
						{
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Button_Ok->deactivate();
						}
						else
						{
							((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Button_Ok->activate();
						}

						//przekazanie do okna zmiany nazwy punktu indeksu punktu, ktorego nazwe trzeba zmienic
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Idx_Point_To_Rename = N;
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->Input_Point_Name_New->take_focus();
						((ImagesShowWindow*)ParentWindow)->OknoZmianyNazwy->show();

					}
				}
				

			case FL_Alt_L:
				HFactor = 5;
				ret =1;
				break;
			case FL_Alt_R:
				HFactor = 5;
				ret =1;
				break;

		}
		break;
	case FL_KEYUP:
		switch (Fl::event_key())
		{
		case FL_Alt_L:
			HFactor = 1;
			ret = 1;
			break;
		case FL_Alt_R:
			HFactor = 5;
			ret =1;
			break;
		}
		break;

	case FL_ENTER:
		if (DrawingMode == 1)
		{
			cursor(FL_CURSOR_DEFAULT);
		}

		CursorPos = 2; //W oknie glownym i w oknie Parent
		focus(this);
		ret  = 1;
		break;
	case FL_LEAVE: //Nie wiadomo czemu ten event jest zawsze zglaszany 2 razy. Co ciekawe: pierwszy raz zawsze gdy 
		// mysz opuszcza okno Parenta a drugi raz gdy okno wlasciwe. Z eventem FL_ENTER nie ma takiego problemu.
		// Reakcja ma byæ jedynie na opuszczenie pierwszego okna. 
		CursorPos = CursorPos - 1;
		switch (DrawingMode)
		{
		case 2:
			SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x()+ (int)(w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );
			Ref[0] = (int)(w()/4+0.5);
			Ref[1] = (int)(h()/2+0.5);
			OrtRef[0] = RzutLewy.left;
			OrtRef[1] = RzutLewy.bottom;
			break;
		case 3:
			SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x()+ (int)(3*w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );
			Ref[2] = (int)(3*w()/4+0.5);
			Ref[3] = (int)(h()/2+0.5);
			OrtRef[2] = RzutPrawy.left;
			OrtRef[3] = RzutPrawy.bottom;
			break;
		case 4:
			SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x()+ (int)(3*w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );
			Ref[2] = (int)(3*w()/4+0.5);
			Ref[3] = (int)(h()/2+0.5);
			OrtRef[0] = RzutLewy.left;
			OrtRef[1] = RzutLewy.bottom;			
			OrtRef[2] = RzutPrawy.left;
			OrtRef[3] = RzutPrawy.bottom;
		}
		ret = 1;

	case FL_MOVE: // Ruch myszk¹
		WindowX = Fl::event_x();
		if (CursorPos == 1)
		{
			WindowY = (Fl::event_y() - 20); //Niezbedna korekcja
		}
		else
		{
			WindowY = Fl::event_y(); 
		}

		

		switch (DrawingMode)
		{
		case 2: // Okno lewe // Zmieniamy parametry rzutu ortogonalnego dla zdjecia lewego
			//cursor(FL_CURSOR_CROSS);
			cursor(FL_CURSOR_NONE);
			if (LoadedL)
			{
				if ((WindowX < 20) || (WindowX > w() / 2 - 20) || (WindowY < 20) || (WindowY > h() - 20))
				{
					SetCursorPos(((ImagesShowWindow*)ParentWindow)->x() + x() + (int)(w() / 4.0 + 0.5), ((ImagesShowWindow*)ParentWindow)->y() + y() + (int)(h() / 2.0 + 0.5));
					Ref[0] = (int)(w() / 4.0 + 0.5);
					Ref[1] = (int)(h() / 2.0 + 0.5);
					OrtRef[0] = RzutLewy.left;
					OrtRef[1] = RzutLewy.bottom;

				}
				else
				{
					if ((CursorPos == 0) || (CursorPos == 2))
					{
						//double rll = OrtRef[0] + (WindowX - Ref[0]) / Zoom;
						//double rlb = OrtRef[1] - (WindowY - Ref[1]) / Zoom;
						//double xi = rll + (w() / 4.0 + 0.5) / Zoom - WL / 2.0;
						//double yi = rlb + (h() / 2.0 + 0.5) / Zoom - HL / 2.0;

						//if (xi > -WL && xi < WL && yi > -HL && yi < HL)
						//{
							RzutLewy.left = OrtRef[0] + (WindowX - Ref[0]) / Zoom;
							RzutLewy.bottom = OrtRef[1] - (WindowY - Ref[1]) / Zoom;
						//}
						//else
						//{
							

						//}

					}
				}
				calc_CrossPos();
				send_CoordsToDisp();
				redraw();
			}
			break;
		case 3: // Okno prawe // Zmieniamy parametry rzutu ortogonalngo dla zdjecia prawego
			//if (Fl::event_x() > w()/2) cursor(FL_CURSOR_CROSS);
			//if (Fl::event_x() <= w()/2) cursor(FL_CURSOR_DEFAULT);			
			//cursor(FL_CURSOR_CROSS);
			cursor(FL_CURSOR_NONE);
			if (LoadedR)
			{
				if ((Fl::event_x() < w() / 2 + 20) || (Fl::event_x() > w() - 20) || (Fl::event_y() < 20) || (Fl::event_y() > h() - 20))
				{
					SetCursorPos(((ImagesShowWindow*)ParentWindow)->x() + x() + (int)(3 * w() / 4 + 0.5), ((ImagesShowWindow*)ParentWindow)->y() + y() + (int)(h() / 2 + 0.5));
					Ref[2] = (int)(3 * w() / 4 + 0.5);
					Ref[3] = (int)(h() / 2 + 0.5);
					OrtRef[2] = RzutPrawy.left;
					OrtRef[3] = RzutPrawy.bottom;

				}
				else
				{
					if ((CursorPos == 0) || (CursorPos == 2))
					{
						RzutPrawy.left = OrtRef[2] + (WindowX - Ref[2]) / Zoom;
						RzutPrawy.bottom = OrtRef[3] - (WindowY - Ref[3]) / Zoom;
					}
				}
				calc_CrossPos();
				send_CoordsToDisp();
				redraw();
			}
			break;
		case 4:
			cursor(FL_CURSOR_NONE);
			if( (WindowX < 20) || (WindowX > w()/2 - 20) || (WindowY < 20) || (WindowY > h() - 20) )
			{
				//cout << "Wyjscie poza krawedz! " <<endl;
				SetCursorPos( ((ImagesShowWindow*)ParentWindow)->x() + x()+ (int)(w()/4+0.5), ((ImagesShowWindow*)ParentWindow)->y() +  y()+ (int)(h()/2+0.5) );
				Ref[0] = (int)(w()/4+0.5);
				Ref[1] = (int)(h()/2+0.5);
				OrtRef[0] = RzutLewy.left;
				OrtRef[1] = RzutLewy.bottom;
				OrtRef[2] = RzutPrawy.left;
				OrtRef[3] = RzutPrawy.bottom;
			}
			else
			{
				if ( (CursorPos == 0) || (CursorPos ==2) )
				{
					RzutLewy.left = OrtRef[0] + (WindowX - Ref[0])/Zoom;
					RzutLewy.bottom = OrtRef[1] - (WindowY - Ref[1])/Zoom;
					RzutPrawy.left = OrtRef[2] + (WindowX - Ref[0])/Zoom;
					RzutPrawy.bottom = OrtRef[3] - (WindowY - Ref[1])/Zoom;
				}
			}
			calc_CrossPos();
			send_CoordsToDisp();
			redraw();
			break;

		ret = 1;
		}
	break;
	case FL_PUSH :
		calc_CrossPos(); //Zamiast powyzszego
		//cout <<"Cross[0]: " <<Cross[0] <<endl;
		//cout <<"Cross[1]: " <<Cross[1] <<endl;
		//cout <<"Cross[2]: " <<Cross[2] <<endl;
		//cout <<"Cross[3]: " <<Cross[3] <<endl;

		switch (Fl::event_button())
		{
		case FL_LEFT_MOUSE:
			
			//Jezeli tryb pomiaru jest aktywny przynajmniej dla 1 okna
			if (  (((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarLewy->value()==1) || (((ImagesShowWindow*)ParentWindow)->Toggle_ButtonPomiarPrawy->value()==1)   )
			{
				PreviousDrawingMode = DrawingMode;
				DrawingMode = 1; //Bez trybu pomiaru
				cursor(FL_CURSOR_DEFAULT);
				((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->color(FL_RED);
				((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->value("N");
				((ImagesShowWindow*)ParentWindow)->OutputDrawingMode->redraw();
				((ImagesShowWindow*)ParentWindow)->OknoOznaczania->set_modal();
				((ImagesShowWindow*)ParentWindow)->OknoOznaczania->show();
				SetCursorPos( ((ImagesShowWindow*)ParentWindow)->OknoOznaczania->x()+60, ((ImagesShowWindow*)ParentWindow)->OknoOznaczania->y()+100);
			}
			
			ret = 1;
			break;

		}
		//cout <<"Mouse clicks: "<< DrwClicks <<endl;
		break;
		case FL_MOUSEWHEEL:
		
			if (Fl::event_dy() < 0)
			{
				zoomIn();
//				cout <<"Mipmap level: "<<MipmapLevel<<endl;
//				cout <<"Quad size: "<<QuadSize<<endl;
				redraw();
			}
			else
			{
				zoomOut();
//				cout <<"Mipmap level: "<<MipmapLevel<<endl;
//				cout <<"Quad size: "<<QuadSize<<endl;
				redraw();
			}

			//cout <<"x: "<<Fl::event_dx()<<" y: "<<Fl::event_dy()<<endl;
			ret = 1;
			break;
	 }
	 return ret;
}
