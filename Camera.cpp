#include "pch.h"
#include "Camera.h"
using namespace std;

Camera::Camera(void)
{
}

Camera::Camera (const char * Filename)
{
	ifstream Plik;
	Plik.open(Filename);
	if (Plik)
	{
		Valid = 1;
		Name = Filename;
		string Linia;
		string Opis;
		double M; //blad
		
		//Linia 1
		getline(Plik, Linia, '\n');
		istringstream LiniaStream(Linia);
		LiniaStream >> Opis;
		
		//Linia 2
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> Description;

		//Linia 3
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> Opis; //Size

		//Linia 4
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> W >> H;

		//Linia 5
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> Opis; //Interior

		//Linia 6
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> ck >> M;

		//Linia 7
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> x0 >> M;

		//Linia 8
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> y0 >> M;

		//Linia 9
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> Opis; //Radial distortion

		//Linia 10
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> k1 >> M;

		//Linia 11
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> k2 >> M;

		//Linia 12
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> k3 >> M;

		//Linia 13
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> Opis; //Tangential distortion

		//Linia 14
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> p1 >> M;

		//Linia 15
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> p2 >> M;

		//Linia 16
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> Opis; //Y scaling

		//Linia 17
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> y_scaling >> M;

		//Linia 18
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> Opis; //skewness

		//Linia 19
		getline(Plik, Linia, '\n');
		LiniaStream.clear();
		LiniaStream.str(Linia);
		LiniaStream >> skewness >> M;
	}
	else
	{
		Valid = 0;
	}
	Plik.close();
}

Camera::Camera(double* io, double *k, double *p, double *a)
{
	//To jest konstruktor, ktory tworzy plik kamery na podstawie pliku obrazu
	//ktory moze byc wczytany z pliku ida
	Name = "";
	Description = "";
	W = 0;
	H = 0;
	ck = io[0];
	x0 = io[1];
	y0 = io[2];
	k1 = k[0];
	k2 = k[1];
	k3 = k[2];
	p1 = p[0];
	p2 = p[1];
	y_scaling = a[0];
	skewness = a[1];
	Valid = 1;
}

Camera::~Camera(void)
{
}

void Camera::wypisz(void)
{
	cout << "Camera data: " <<endl;
	cout << "Name: " << Name <<endl;
	cout << "Description: " << Description <<endl;
	cout << "W: "<<W<<endl;
	cout << "H: "<<H<<endl;
	cout << "ck: " << ck <<endl;
	cout << "x0: " << x0 <<endl;
	cout << "y0: " << y0 <<endl;
	cout << "k1: " << k1 <<endl;
	cout << "k2: " << k2 <<endl;
	cout << "k3: " << k3 <<endl;
	cout << "p1: " << p1 <<endl;
	cout << "p2: " << p2 <<endl;
	cout << "y_scaling: "<< y_scaling <<endl;
	cout << "skewness: "<<skewness <<endl;
}

void Camera::writeToFile(string & filename) const
{
	ofstream S(filename);
	S << "Name:\n";
	S << Name << "\n";
	S << "Size:\n";
	S << W << " " << H << "\n";
	S << "Interior:\n";
	S << std::fixed << setprecision(3) << ck << " 0.100\n";
	S << std::fixed << setprecision(3) << x0 << " 0.100\n";
	S << std::fixed << setprecision(3) << y0 << " 0.100\n";
	S << "Radial distortion:\n";
	S << std::scientific << setprecision(6) << k1 << " 1.0e-010\n";
	S << std::scientific << setprecision(6) << k2 << " 1.0e-017\n";
	S << std::scientific << setprecision(6) << k3 << " 1.0e-028\n";
	S << "Tangential distortion:\n";
	S << std::scientific << setprecision(6) << p1 << " 1.0e-010\n";
	S << std::scientific << setprecision(6) << p2 << " 1.0e-010\n";
	S << "Y scaling:\n";
	S << std::fixed << setprecision(3) << y_scaling << " 0.0001\n";
	S << "Skewness of axes:\n";
	S << std::fixed << setprecision(3) << skewness << " 0.0001\n";
	S.close();
}
