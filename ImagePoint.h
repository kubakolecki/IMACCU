#pragma once
#include <string>

class ImagePoint
{
public:
	static ofstream StrumienDoZapisu_L; // zmienna statyczna -  strumien do zapisu wspolrzednych ze zdjecia lewego
	static ifstream StrumienDoOdczytu_L; // zmienna statyczna -  strumien do odczytu wspolrzednych ze zdjecia lewego
	static ofstream StrumienDoZapisu_P; // podobnie, tylko dla zdjecia prawego
	static ifstream StrumienDoOdczytu_P;
	static ofstream StrumienDoZapisu; //ogolny strumien do zapisu gdy trzeba zapisac jakies dane do pliku pix
	std::string Name = "";
	std::string ImageId = "0";
	std::string LineId = "0";
	double X = 0.0;
	double Y = 0.0;
	double Mx = 0.5;
	double My = 0.5;
	short Type = 0; //Typ 0 -wiazacy, 1-ZPunkt, 2-PPunkt, 3-XYZ punkt, 4-Check punkt, 
	//5- wiazacy na linii, 6 - wiazacy na linii poziomej, 7 - wiazacy na linii pionowej, 101 - punkt do definicji ACS
	bool Active = true;
	bool Deleted = false;
	ImagePoint(void);
	ImagePoint(std::string Name, std::string ImageId, double X, double Y, short T);
	ImagePoint(string _Name, double _X, double _Y);
	ImagePoint(double _X, double _Y);
	ImagePoint(std::string N, std::string ImId, double WspX, double WspY, double XErr, double YErr, short T);
	ImagePoint(std::string Name, std::string ImageId, std::string LnId,double X, double Y, short T);
	ImagePoint(std::string N, std::string ImId, std::string LnId, double WspX, double WspY, double XErr, double YErr, short T);
	ImagePoint(std::string Linia);
	~ImagePoint(void);
	void write_L();
	void write_P();
	void write();
	void load(const char *Filename,std::string Name);
	void load_L(int LineNr);
	void load_P(int LineNr);
	void load(const char *Filename, int LineNr);
};
