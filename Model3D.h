#pragma once
#include "SimplePoint.h"

class Model3D //Ta klasa jest uzywana rowniez do przechowywania bloku
{
public:
	string FirstImageName;
	string SecondImageName;
	int NPoints; //Liczba punktow
	double ckL; //Stala kamery na zdjeciu lewym
	double ckR; //Stala kamery na zdjeciu prawym	
	vector < SimplePoint<string> > PL; //Punkty na lewym zdjeciu. 
	vector < SimplePoint<string> > PR; //Punkty na prawym zdjeciu
	vector < TerrainPoint> TerPoints; //Wspolrzedne punktow w ukladzie modelu:
	//Elementy orientacji wzajemnej:
	double r[9];
	double b[3];
	void readFromFile(string Filename); //Odczyt z pliku .mod
	void writeToFile(string Filename); //Zapis do pliku .mod
	void calcModelCoords(void); //Oblicza wspolrzedne w ukladzie modelu
	void wypisz(void);
	Model3D(void);
	~Model3D(void);
};

