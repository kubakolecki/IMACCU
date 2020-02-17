#pragma once
#include "Model3D.h"
#include <vector>
using namespace std;
class Transf3DData
{
public:
	Transf3DData(void);
	~Transf3DData(void);
	double s; // wspolczynnik skali
	double T[3]; // wektor translacji
	double R[9]; // macierz obrotu
	vector<double> VX; // poprawki
	vector<double> VY;
	vector<double> VZ;
	vector<string> Names; //Nazwy punktow
	vector<int> Outliers;
	double Mx; // bledy wspolrzednych
	double My;
	double Mz;
	double Mp; // blad polozenia po transformacji
	bool EstScale; // czy estymowac skale
	short Priority; // ktorys ze zbiorow punktow moze miec wyzsza dokladnosc co wplywa na rozwiazanie
	// opcje:
	// 0 - zbiory rownodokladne(defaultowe)
	// 1 - lewy zbior dokladniejszy
	// 2 - prawy zbior dokladniejszy
	int FixedPointsIdx[3]; //indeksy punktow, ktore uznawane sa za stale punkty dostosowania
	int FixedPointsN; //liczba stalych punktow dostoswania
	//opcje:
	//0 - domyslnie - estymowane sa wszystkie parametry transformacji s,R,T
	//1 - estymowane sa s,R a T jest wyliczane zgodnie z wektorem miedzy punktami
	//2 - estymowane jest tylko R a s,T sa wyliczane
	//3 - wszystkie parametry moga byc obliczone i nie sa estymowane
	void wypisz();
	void solve(vector<TerrainPoint> *LeftPoints, vector<TerrainPoint> *RightPoints);
	void transform(vector<TerrainPoint> *Source, vector<TerrainPoint>* Target);
	void transform(double *Source, double *Target); //Transformacja pojedynczego punktu
	void transform(vector<double> *Source, vector<double> *Target); //Transformacja pojedynczego punktu
	void calcErrors(vector<TerrainPoint> *LeftPoints, vector<TerrainPoint> *RightPoints); // Do obliczania poprawek i bledow
	void applyToModel(Model3D *InputModel, Image *LeftImage, Image *RightImage, bool SetApr, bool SetObs, bool SetAdj);

	//Funkcje do ransac:
	void checkByRansac(vector<TerrainPoint> *LeftPoints, vector<TerrainPoint> *RightPoints, int NSamples, double Thresh);

private:
	//Do ransac:
	void createcomb(vector <vector<int> > * Comb, int N);
	void create3PointSamples(vector <vector<int> > * Samples, vector<TerrainPoint> *P, int MaxSamples);
	double calcRank(double Thresh);
};

