#pragma once
#include "Camera.h"
class Image
{
public:
	std::string ImageFileName;
	int NPoints = 0; //Ile punktow do tej pory pomierzono na zdjeciu
	std::string CamFileName; //Nazwa pliku kamery
	std::string Name;
	short HasDist; //Czy jest dystorsja: 0 - nie ma, 1 - jest

	int W;
	int H;

	double Transf_x[4];
	double Transf_y[4];

	short InteriorAdjust[3];
	double Interior[3];
	double Apriori_IErr[3];
	double Aposteriori_IErr[3];
	
	short kAdjust[3];
	double k[3];
	double Apriori_kErr[3];
	double Aposteriori_kErr[3];
	
	short pAdjust[2];
	double p[2];
	double Apriori_pErr[2];
	double Aposteriori_pErr[2];

	short AddAdjust[2];
	double Add[2];
	double Apriori_AddErr[2];
	double Aposteriori_AddErr[2];

	std::string AngularSystem; // al-ni-ka, om-fi-ka , roll-pitch-yaw
	
	short AnglesAdjust[3]; // Wektor okreslajacy jak wielkosci traktowac w wyrownaniu 0-przyblizenia, 1-obserwacje, 2-stale 
	
	double AnglesApr[3]; // Katy poczatkowe - przyblizone
	double AnglesObs[3]; // Katy obserwowane
	double AnglesAdj[3]; // Katy wyliczone - adjusted
	
	double AnglesAprErr[3]; //Bledy katow
	double AnglesObsErr[3];
	double AnglesAdjErr[3];
	
	short CoordsAdjust[3]; // Wektor okreslajacy jak wielkosci traktowac w wyrownaniu
	
	double CoordsApr[3]; // Wspolrzedne podane
	double CoordsObs[3]; // Wspolrzedne obserwowane
	double CoordsAdj[3]; // Wspolrzedne wyliczone
	
	double CoordsAprErr[3]; // Bledy wsp
	double CoordsObsErr[3];
	double CoordsAdjErr[3];
	
	double Quaternions[4];
	double R[9]; // Macierz obrotu
	double DLT[11]; //Wspolczynniki DLT
	
	Image(void);
	Image (Camera kamera);
	~Image(void);
	short WriteToFile(const char * Filename);
	short ReadFromFile(const char * Filename);
	void wypisz();
	void setCamera(Camera kamera);
	void setRotationMatrix(double *_R);
	void setRotationMatrix(vector<double> *_R);
	void setRotationMatrix(CvMat *Rot, int flag, std::string Type);
	void setQuaternions(double *Q);
	void setQuaternions(double *Q, int flag, std::string Type);	
	void setAnglesApr(double *Angles);
	void setAnglesObs(double *Angles);
	void setAnglesAdj(double *Angles);
	void setCoordsApr(double *Coords);
	void setCoordsObs(double *Coords);
	void setCoordsAdj(double *Coords);
	void setCoordsAdj(TerrainPoint *Coords);
	
	void setAnglesAprErr(double *Angles);
	void setAnglesObsErr(double *Angles);
	void setAnglesAdjErr(double *Angles);
	void setCoordsAprErr(double *Coords);
	void setCoordsObsErr(double *Coords);
	void setCoordsAdjErr(double *Coords);

	void setDLT(void); //Ustawienie parametrow DLT na podstawie znanych elementow orientacji zdjecia
	short checkDist(void); //Funkcja sprawdza, czy bledy obrazu sa rowne zero - czy zdjecie ma usunieta dystorsje
};
