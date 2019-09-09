#pragma once
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Button.H>
#include "PointLabelWindow.h"
#include "PointRenameWindow.h"
#include "StereoGlWindow.h"
#include "Image.h"
#include "ImagePoint.h"
#include "WarningWindow.h"

class ImagesShowWindow : public Fl_Double_Window
{
friend class AutoPointMesWindow; //Po to aby klasa AutoPointMesWindow mogla korzystac z funkcji changePixStreamL
public:
	int Id; //Id okna - numer okna
	ImagesShowWindow(int _Id, int X,int Y,int W,int H,const char*L = 0);
	~ImagesShowWindow(void);
	void DispCoords(double X, double Y); //Wyswietlanie wspolrzednych 2D
	void DispCoords(double X, double Y, double Z); //Wyswietlanie wspolrzednych 3D (przeladowanie)
	void *win;
	StereoGlWindow * StereoWindow;
	Fl_Button *Button_SM;
	Fl_Button *Button_TMLRX; //Rodzaj wyswietlanych wspolrzednych Teren, Model, Lewy Obraz, Prawy Obraz, Brak
	Fl_Input_Choice * InputObrazLewy;
	Fl_Input_Choice * InputObrazPrawy;
	Fl_Output *OutputZoom; //Informacja o zoomie
	Fl_Output *OutputDrawingMode; // Sposob rysowania (brak, lewy, prawy, oba, autogrametryczny)
	Fl_Output *OutputCoords; //Wyswietlanie wspolrzednych
	Fl_Toggle_Button *Toggle_ButtonPomiarLewy;
	Fl_Toggle_Button *Toggle_ButtonPomiarPrawy;
	WarningWindow *ImageNotFoundWindow;

	PointLabelWindow *OknoOznaczania;
	PointRenameWindow *OknoZmianyNazwy;

	Image LeftImage;
	Image RightImage;

	short CoordsDispMode; //Tryb wyswietlanai wsporzednych X-0; T-1; M-2; L-3; R-4;
	std::string IdaFileName_Left;
	std::string IdaFileName_Right;
	std::string PixFileName_Left;
	std::string PixFileName_Right;
	Fl_JPEG_Image *ButtonIcon1;
	Fl_JPEG_Image *ButtonIcon2;

	vector<std::string> ImageNames;
	int LeftIdx;
	int RightIdx;

	int loadTex_Lumin(IplImage *image, GLuint *textureName);
	int loadTex_RGB(IplImage *image, GLuint *textureName);
	void load_Textures(IplImage *Image, vector <TextureQuad> *Quadsy, vector <IplImage*> *Obrazki, GLuint*& Tekstury, unsigned int* N); //Ladowanie teksur do StereoWindow->ImagesLeft i StereoWindow->TexturesL;

	void load_Points_L();
	void load_Points_P();
	void changePixStreamP(std::string s);
	void changePixStreamL(std::string s);

private:

	//void generateQuads(vector<TextureQuad> *Quadsy, unsigned int H, unsigned int W);

	static void cb_Input_ObrazLewy (Fl_Widget *w, void *v);
	inline void cb_Input_ObrazLewy_i();

	static void cb_Input_ObrazPrawy (Fl_Widget *w, void *v);
	inline void cb_Input_ObrazPrawy_i();

	static void cb_Output_DrawingMode (Fl_Widget *w, void *v);
	inline void cb_Output_DrawingMode_i();

	static void cb_Toggle_ButtonPomiarLewy (Fl_Widget *w, void *v);
	inline void cb_Toggle_ButtonPomiarLewy_i();

	static void cb_Toggle_ButtonPomiarPrawy (Fl_Widget *w, void *v);
	inline void cb_Toggle_ButtonPomiarPrawy_i();

	static void cb_Button_SM (Fl_Widget *w, void *v);
	inline void cb_Button_SM_i();

	static void cb_Button_TMLRX (Fl_Widget *w, void *v);
	inline void cb_Button_TMLRX_i();


	int handle(int e);
};
