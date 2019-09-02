#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <tiffio.h>

#include "OrthoData.h"
#include "ImagePoint.h"
#include "TerrainPoint.h"
#include "Graphics.h"
#include "SimplePoint.h"
#include "TextureQuad.h"

class StereoGlWindow : public Fl_Gl_Window
{
public:
	//Czulosc ruchu w tybie autogrametrycznym
	static double dX;
	static double dY;
	static double dZ;  
	static double SnapSens; //Czulosc snapa i delete
	static double PointSize; //Wielkosc punktow rysunku
	static Graphics Grafika; //Grafika
	
	OrthoData RzutLewy; //Struktura opisujaca rzut ortogonalny dla zdjecia lewego...
	OrthoData RzutPrawy; // ... i prawego
	int Ref[4]; //Tablica zawieraja kolejno dane: RefXL, RefYL, RefXR, RefYR - Wspolrzedna odniesienia dla ruchu myszki w trybie pomiaru
	double OrtRef[4];// OrtRefXL, OrtRefYL, OrtRefXR, OrtRefYR -Wspolrzedne odniesienia dla lewego dolnego naroznika w rzucie ortogonalnym
	int WWidth; //Szerokosc okna poczatkowa
	int WHeight; //Wysokosc poczatkowa okna

	float Zoom; //Wspolczynnik powiekszenia
	unsigned int QuadSize;
	unsigned int MipmapLevel;

	float HFactor; //Wspolczynnik przyspieszenia ruchu pionowego kursora
	bool Mono; //Okienko Mono lub stereo (gdy true to mono)

	double CursorX; //Wspolrzedne kursora myszki w okienkach
	double CursorY;
	double Cross[4]; //Wspolrzedne krzyza pomiarowego w ukladzie obrazu, odpowiednio XL, YL, XP, YP
	bool IsFarFromImageL{ false };
	bool IsFarFromImageR{ false };
	TerrainPoint ModPoint; //Punkt 3D kursura dla ruchu autogrametrycznego
	TerrainPoint TerPoint;
	double SnapCoords[3]; //Wspolrzedne do snapu
	int DrwClicks; //Ile klikniec zostalo wykonanych w ramach rysowania danej linii
	bool ShowDrwLine; //Czy pokazywac aktualnie rysowana linie
	bool TerMov; //Czy sterowanie ruchem kursora odbywa siê w uk³adzie terenowym
	bool MakeTransfer; //Czy dokonywac transferu wsp. do drugiego okna
	short DrawingMode; // Sposob pracy w oknach
	// 1 - brak rysowania, 2 - okno lewe, 3 - okno prawe, 4 - oba okna, 5 - tryb autogrametryczny
	short StereoMode; //Sposob wyswietlania stereo 1- Podwojne okno, 2- anaglif, 3 - inny anaglif, 4 - okulary 3D
	short PreviousDrawingMode; //Poprzedni aktywny sposob pracy w oknach
	short FeatureMode; //Czy jestesmy w trybie rysowania punktow czy linii (0-linie, 1-punkty, 2-shapy)


	string LeftImageFileName;
	string RightImageFileName;
	void *ParentWindow;	
	StereoGlWindow(int X,int Y,int W,int H,const char*L = 0); // konstruktor
	~StereoGlWindow(void); //destruktor
	void draw();
	void resize (int X,int Y,int W,int H);
	
	IplImage* ImageL; //Wskazniki do obrazow: prawego i lewego
	IplImage* ImageR;

	uint32** TilesL; //Tu beda alokowane tablice wskaznikow do tili
	uint32** TilesR;

	bool LoadedL; //Zaladowany obraz lewy
	bool LoadedR; //Zaladowany obraz prawy

	int WL, HL; //Szerokosc i wysokosc obrazu lewego
	int WR, HR; //Szerokosc i wysokosc obrazu prawego

	//Tekstury - to sa wektory pikselami tekstur do quadsow (256 x 256)
	vector <IplImage*> ImagesLeft;
	vector <IplImage*> ImagesRight;

	//Tekstury
	GLuint* TexturesL;
	GLuint* TexturesR;

	unsigned int NTL; //Number of left textures
	unsigned int NTR; //Number of right textures


	//Quady z teksturami
	vector <TextureQuad> QuadsL;
	vector <TextureQuad> QuadsR;

	//Widoczne quadsy
	vector <unsigned int> VisibleQuadsL;
	vector <unsigned int> VisibleQuadsR;

	//Punkty wiazace i fotopunkty
	vector <ImagePoint> LeftPoints;
	vector <ImagePoint> RightPoints;

	//Wierzcholki linii
	vector <SimplePoint<int> > DrwPointsLeft;
	vector <SimplePoint<int> > DrwPointsRight;
	vector < vector <SimplePoint<int> > >DispPointsLeft;
	vector < vector <SimplePoint<int> > >DispPointsRight;
	
	//Punkty
	vector <SimplePoint<int> > DrwPostsLeft;
	vector <SimplePoint<int> > DrwPostsRight;
	vector < vector <SimplePoint<int> > >DispPostsLeft;
	vector < vector <SimplePoint<int> > >DispPostsRight;
	
	bool DrawingState; //Stan rysowania linii - czy w danym momencie rysowana jest linia - aby uniemozliwic niektore operacje
	bool DrawPoints; //Czy rysowac punkty wiazace i fotopunkty
	GLuint TexL, TexR; //Tekstura prawa i lewa
	short CursorPos; //0 - outside all windows, 1 - only inside parrent window, 2 - inside all windows 
	void set_DrawingMode(short Mode);
	void calc_CrossPos(void); //Funkcja oblicza wspolrzedne krzyzy pomiarowych w ukladzie obrazu, czyli aktualizuje wektor Cross
	void send_CoordsToDisp(void); //Funkcja wysyla wspolrzedne ktore maja byc wyswietlane
	void calc_Rzut(double XL, double YL, double XP, double YP);
	void zoomIn(void);
	void zoomOut(void);

private:
	int handle (int e);
	//void draw_triangles( void );
	void draw_cursor (double x, double y, short size);
	void draw_points_L (void);
	void draw_points_P (void);

	void draw_textures(vector <TextureQuad> *Quadsy, GLuint *Tekstury, double depth, unsigned int* N);
	int WindowX, WindowY; //Sa to aktualne wspolrzedne w oknie
	bool disp_info{ true };
};
