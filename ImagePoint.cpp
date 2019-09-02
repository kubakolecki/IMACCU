#include "pch.h"
#include "ImagePoint.h"
ofstream ImagePoint::StrumienDoZapisu_L; //inicjalizacja zmiennej statycznej
ifstream ImagePoint::StrumienDoOdczytu_L;//inicjalizacja zmiennej statycznej
ofstream ImagePoint::StrumienDoZapisu_P; //inicjalizacja zmiennej statycznej
ifstream ImagePoint::StrumienDoOdczytu_P;//inicjalizacja zmiennej statycznej
ofstream ImagePoint::StrumienDoZapisu; //inicjalizacja zmiennej statycznej

ImagePoint::ImagePoint(void)
{
	LineId = "0";
	Deleted = false;
	Active = true;
	Mx = 0.5;
	My = 0.5;
}

ImagePoint::ImagePoint(string N, string ImId, double WspX, double WspY, short T)
{
	Name = N;
	ImageId = ImId;
	LineId = "0";
	X = WspX;
	Y = WspY;
	Mx = 0.5;
	My = 0.5;
	Type = T;
	Active = true;
	Deleted = false;
}

ImagePoint::ImagePoint(string _Name, double _X, double _Y): Name(_Name), X(_X), Y(_Y)
{
}

ImagePoint::ImagePoint(double _X, double _Y): Name(""), X(_X), Y(_Y)
{

}

ImagePoint::ImagePoint(string N, string ImId, double WspX, double WspY, double XErr, double YErr,  short T)
{
	Name = N;
	ImageId = ImId;
	LineId = "0";
	X = WspX;
	Y = WspY;
	Mx = XErr;
	My = YErr;
	Type = T;
	Active = true;
	Deleted = false;
}

ImagePoint::ImagePoint(string N, string ImId, string LnId, double WspX, double WspY, short T)
{
	Name = N;
	ImageId = ImId;
	LineId = LnId;
	X = WspX;
	Y = WspY;
	Mx = 0.5;
	My = 0.5;
	Type = T;
	Active = true;
	Deleted = false;
}

ImagePoint::ImagePoint(string N, string ImId, string LnId, double WspX, double WspY, double XErr, double YErr, short T)
{
	Name = N;
	ImageId = ImId;
	LineId = LnId;
	X = WspX;
	Y = WspY;
	Mx = XErr;
	My = YErr;
	Type = T;
	Active = true;
	Deleted = false;
}

ImagePoint::ImagePoint(string Linia)
{
	string wiersz(Linia);
	istringstream LiniaPliku(wiersz);
	LiniaPliku >> Name >> ImageId >> LineId >> X >> Y >> Mx >> My >> Type >> Active; 
}

ImagePoint::~ImagePoint(void)
{
}

void ImagePoint::write_L()
{
	StrumienDoZapisu_L <<setw(7)<<Name<<" "<<setw(15)<<ImageId<<" "<<setw(15)<<LineId<<" "<<fixed<< setprecision(2)<<setw(9)<<X<<" "<<setw(9)<<Y<<" "<<setw(5)<<Mx<<" "<<setw(5)<<My<<" "<<setw(3)<<Type<< setw(2) << Active<< endl;
}

void ImagePoint::write_P()
{
	StrumienDoZapisu_P <<setw(7)<<Name<<" "<<setw(15)<<ImageId<<" "<<setw(15)<<LineId<<" "<<fixed<< setprecision(2)<<setw(9)<<X<<" "<<setw(9)<<Y<<" "<<setw(5)<<Mx<<" "<<setw(5)<<My<<" "<<setw(3)<<Type << setw(2) << Active << endl;
}

void ImagePoint::write()
{
	StrumienDoZapisu << setw(7) << Name << " " << setw(15) << ImageId << " " << setw(15) << LineId << " " << fixed << setprecision(2) << setw(9) << X << " " << setw(9) << Y << " " << setw(5) << Mx << " " << setw(5) << My << " " << setw(3) << Type << setw(2) << Active << endl;
}

void ImagePoint::load_L(int LineNr)
{
	char Linia[300];
	int Ln = 0;
	while (StrumienDoOdczytu_L)
	{	
		StrumienDoOdczytu_L.getline(Linia,300,'\n'); //Zczytaj cala linie
		Ln++; //Przeskocz do nastepnej linijki
		if (Ln == LineNr)
		{
			string wiersz(Linia);
			istringstream LiniaPliku(wiersz);
			LiniaPliku >> Name >> ImageId >> LineId >> X >> Y >> Mx >> My >> Type >> Active; 
		}
	}
	StrumienDoOdczytu_L.clear(StrumienDoOdczytu_L.rdstate() & ~(ios::eofbit | ios::failbit));
	StrumienDoOdczytu_L.seekg(0); //Strumien znow pokazuje na poczatek pliku
}

void ImagePoint::load_P(int LineNr)
{
	char Linia[300];
	int Ln = 0;
	while (StrumienDoOdczytu_P)
	{	
		StrumienDoOdczytu_P.getline(Linia,300,'\n'); //Zczytaj cala linie
		Ln++; //Przeskocz do nastepnej linijki
		if (Ln == LineNr)
		{
			string wiersz(Linia);
			istringstream LiniaPliku(wiersz);
			LiniaPliku >> Name >> ImageId >> LineId >> X >> Y >> Mx >> My >> Type >> Active; 
		}
	}
	StrumienDoOdczytu_P.clear(StrumienDoOdczytu_L.rdstate() & ~(ios::eofbit | ios::failbit));
	StrumienDoOdczytu_P.seekg(0); //Strumien znow pokazuje na poczatek pliku
}

void ImagePoint::load(const char *Filename, int LineNr)
{
	ifstream StrumienDoOdczytu;
	StrumienDoOdczytu.open(Filename);	
	char Linia[300];
	int Ln = 0;
	while (StrumienDoOdczytu)
	{	
		StrumienDoOdczytu.getline(Linia,300,'\n'); //Zczytaj cala linie
		Ln++; //Przeskocz do nastepnej linijki
		if (Ln == LineNr)
		{
			string wiersz(Linia);
			istringstream LiniaPliku(wiersz);
			LiniaPliku >> Name >> ImageId >> LineId >> X >> Y >> Mx >> My >> Type >> Active; 
		}
	}
	StrumienDoOdczytu.close();
}
