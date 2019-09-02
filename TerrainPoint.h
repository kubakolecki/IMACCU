#pragma once
#include <string>
using namespace std;
class TerrainPoint
{
public:
	string Name = "1"; //Nazwa
	double X = 0; //Wspolrzedne
	double Y = 0;
	double Z = 0;
	double Mx = 0.005; //Blad a'priori
	double My = 0.005;
	double Mz = 0.005;
	double MAx = 0.003; //Blad a'posteriori
	double MAy = 0.003;
	double MAz = 0.003;
	short Type = 0; //Typ 0 - wiazacy, 1 - ZPunkt, 2 - PPunkt, 3 - XYZ punkt, 4 - Check punkt, 
	//5 - wiazacy na linii, 6 - iazacy na linii poziomej, 7 - wiazacy na linii pionowej, 8-wyznaczany
	string TypParametru = "MES"; //UNN, MES, APX, FIX
	bool Use = true; // Czy punkt ma byc uzyty
	TerrainPoint(void);
	TerrainPoint(string N, double WspX, double WspY, double WspZ);
	TerrainPoint(string N, double WspX, double WspY, double WspZ, short T);
	TerrainPoint(string N, double WspX, double WspY, double WspZ, double ErrX, double ErrY, double ErrZ);
	TerrainPoint(string N, double WspX, double WspY, double WspZ, double ErrX, double ErrY, double ErrZ, short T);
	~TerrainPoint(void);

	//Funkcje skladowe
	void Wypisz(void);
};
