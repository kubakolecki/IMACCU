#include "pch.h"
#include "SimplePoint.h"
#include "Image.h"
#include <complex>

using namespace std;

int fT_findColsestImpoint(ImagePoint *P,int N, double X, double Y, double *D)
{
	double D2 = pow(P[0].X - X, 2.0) + pow(P[0].Y - Y, 2.0);
	int Index = 0;
	for (int i=1; i<N; i++)
	{
		double L2 = pow(P[i].X - X, 2.0) + pow(P[i].Y - Y, 2.0);
		if (L2 < D2)
		{
			D2 = L2;
			Index = i;
		}
	}
	*D = pow(D2,0.5);
	return Index;
}

int fT_findColsestImpoint(vector <ImagePoint> *P, double X, double Y, double * D)
{
	if ( P->size() > 0)
	{
		double D2 = pow((*P)[0].X - X, 2.0) + pow((*P)[0].Y - Y, 2.0);
		int Index = 0;
		for (int i=1; i<P->size(); i++)
		{
			double L2 = pow((*P)[i].X - X, 2.0) + pow((*P)[i].Y - Y, 2.0);
			if (L2 < D2)
			{
				D2 = L2;
				Index = i;
			}
		}
		*D = pow(D2,0.5);
		return Index;	
	}
	else 
	{
		return -1;
	}
}

int fT_findClosesPoint(vector <TerrainPoint>*T, double X, double Y, double Z, double *D)
{
	double dx0 =  (*T)[0].X - X;
	double dy0 =  (*T)[0].Y - Y;
	double dz0 =  (*T)[0].Z - Z;
	double mindist = sqrt(dx0*dx0 + dy0*dy0 + dz0*dz0);
	double dist = mindist;
	int idx = 0;
	for (int i=1; i<(*T).size(); i++)
	{
		dx0 =  (*T)[i].X - X;
		dy0 =  (*T)[i].Y - Y;
		dz0 =  (*T)[i].Z - Z;
		dist = sqrt(dx0*dx0 + dy0*dy0 + dz0*dz0);
		if (dist < mindist)
		{
			mindist = dist;
			idx = i;
		}
	}
	*D = mindist;
	return idx;
}

double fT_imdist(ImagePoint *P1, ImagePoint *P2)
{
	double d;
	d = pow(pow(P1->X-P2->X,2) + pow(P1->Y-P2->Y,2),0.5);
	return d;
}

double fT_terrainDist(TerrainPoint *T1, TerrainPoint *T2)
{
	double d;
	d = sqrt( pow((T1->X - T2->X),2) + pow((T1->Y - T2->Y),2) + pow((T1->Z - T2->Z),2));
	return d;
}

double fT_terrainDist(double *T1, double *T2)
{
	double d;
	d = sqrt( pow((T1[0] - T2[0]),2) + pow((T1[1] - T2[1]),2) + pow((T1[2] - T2[2]),2));
	return d;
}

double fT_terrainDist(TerrainPoint *T1, double *T2)
{
	double d;
	d = sqrt( pow((T1->X - T2[0]),2) + pow((T1->Y - T2[1]),2) + pow((T1->Z - T2[2]),2));
	return d;
}

void fT_cor2pix(ImagePoint *Source, ImagePoint *Target, int N, int Width, int Height)
{
	//Funkcja przelicza wspolrzedne z ukladu zaczepionego w srodku lewego gornego piksela
	//do ukladu zaczepionego w srodku zdjecia
	//Width - szerokosc obrazka
	//Height - wysokosc obrazka
	for (int i=0; i<N; i++)
	{
		Target[i] = Source[i];
		Target[i].X = Source[i].X - Width/2.0 + 0.5;
		Target[i].Y = Height/2.0 - Source[i].Y - 0.5;
	}	
}

void fT_cor2pix(vector<ImagePoint>& Source, vector<ImagePoint>& Target, int Width, int Height)
{
	Target.clear();
	Target.reserve(Source.size());
	for (auto &p : Source)
	{
		ImagePoint ip = p;
		ip.X = p.X - Width / 2.0 + 0.5;
		ip.Y = Height / 2.0 - p.Y - 0.5;
		Target.push_back(ip);
		
	}
	Target.shrink_to_fit();
}

void fT_pix2fid(vector<ImagePoint> &Source, vector<ImagePoint> &Target, Image& Im)
{
	Target.clear();
	Target.reserve(Source.size());
	for (auto &p : Source)
	{
		ImagePoint ip = p;
		ip.X = p.X - Im.Interior[1];
		ip.Y = p.Y - Im.Interior[2];
		Target.push_back(ip);
	}
	Target.shrink_to_fit();
}



void fT_pix2fid(ImagePoint *Source, ImagePoint *Target, int N, Image* Im)
{
	//Funkcja przelicza wspolrzedne z ukladu odniesienia na obrazie do ukladu tlowego
	for (int i=0; i<N; i++)
	{
		Target[i] = Source[i];
		Target[i].X = Source[i].X - Im->Interior[1];
		Target[i].Y = Source[i].Y - Im->Interior[2];
	}
}

void fT_pix2fid(ImagePoint *Source, ImagePoint *Target, int N, Camera* Cam)
{
	//Funkcja przelicza wspolrzedne z ukladu odniesienia na obrazie do ukladu tlowego
	//Wersja funkcji dla tablic
	for (int i=0; i<N; i++)
	{
		Target[i] = Source[i];
		Target[i].X = Source[i].X - Cam->x0;
		Target[i].Y = Source[i].Y - Cam->y0;
	}
}

void fT_pix2fid( vector<ImagePoint> *Source, vector<ImagePoint> *Target, Camera* Cam)
{
	//Funkcja przelicza wspolrzedne z ukladu odniesienia na obrazie do ukladu tlowego
	//Przeladowanie dla wektorow
	ImagePoint P;
	for (int i=0; i<Source->size(); i++)
	{
		P = Source->at(i);
		P.X = P.X - Cam->x0;
		P.Y = P.Y - Cam->y0;
		Target->push_back(P);
	}
}

void fT_fid2uds(ImagePoint *Source, ImagePoint *Target, int N, Camera* Cam)
{
	//Funkcja przelicza wspolrzedne tlowe na wspolrzedne tlowe bez dystorsji
	double * R = new double[N];
	double * dXRad = new double[N];
	double * dYRad = new double[N];
	double * dXTan = new double[N];
	double * dYTan = new double[N];
	double * dYSca = new double[N];
	double * dYSkw = new double[N];
	
	for (int i=0; i < N; i++)
	{
		R[i] = pow(pow(Source[i].X,2) + pow(Source[i].Y,2),0.5);
	}
	
	for (int i=0; i < N; i++) // Obliczamy skladniki poprawek  z tytulu znieksztalcen systematycznych
	{
		dXRad[i] = Source[i].X * (Cam->k1 * pow(R[i],2) + Cam->k2 * pow(R[i],4) + Cam->k3 * pow(R[i],6));
		dYRad[i] = Source[i].Y * (Cam->k1 * pow(R[i],2) + Cam->k2 * pow(R[i],4) + Cam->k3 * pow(R[i],6));
		dXTan[i] = Cam->p1 * ( pow(R[i],2) + 2* pow(Source[i].X,2) ) + 2 * Cam->p2 * Source[i].X * Source[i].Y;
		dYTan[i] = 2 * Cam->p1 *Source[i].X * Source[i].Y + Cam->p2 * (pow(R[i],2) + 2 * pow(Source[i].Y,2));
		dYSca[i] = Cam->y_scaling * Source[i].Y;
		dYSkw[i] = Cam->skewness * Source[i].X;
	}

	for (int i=0; i <N; i++) // dodajemy poprawki
	{
		//cout <<Target[i].Name <<" distx: " << dXRad[i]<<" disty: " << dYRad[i] <<endl;
		Target[i] = Source[i];
		Target[i].X = Source[i].X - dXRad[i] - dXTan[i];
		Target[i].Y = Source[i].Y - dYRad[i] - dYTan[i] - dYSca[i] - dYSkw[i];
	}

	delete [] R;
	delete [] dXRad;
	delete [] dYRad;
	delete [] dXTan;
	delete [] dYTan;
	delete [] dYSca;
	delete [] dYSkw;
}

void fT_fid2uds(ImagePoint *Source, ImagePoint *Target, int N, Image* Im)
{
	//Funkcja przelicza wspolrzedne tlowe na wspolrzedne tlowe bez dystorsji
	double * R = new double[N];
	double * dXRad = new double[N];
	double * dYRad = new double[N];
	double * dXTan = new double[N];
	double * dYTan = new double[N];
	double * dYSca = new double[N];
	double * dYSkw = new double[N];
	
	for (int i=0; i < N; i++)
	{
		R[i] = pow(pow(Source[i].X,2) + pow(Source[i].Y,2),0.5);
	}
	
	for (int i=0; i < N; i++) // Obliczamy skladniki poprawek  z tytulu znieksztalcen systematycznych
	{
		dXRad[i] = Source[i].X * (Im->k[0] * pow(R[i],2) + Im->k[1] * pow(R[i],4) + Im->k[2] * pow(R[i],6));
		dYRad[i] = Source[i].Y * (Im->k[0] * pow(R[i],2) + Im->k[1] * pow(R[i],4) + Im->k[2] * pow(R[i],6));
		dXTan[i] = Im->p[0] * ( pow(R[i],2) + 2* pow(Source[i].X,2) ) + 2 * Im->p[1] * Source[i].X * Source[i].Y;
		dYTan[i] = 2 * Im->p[0] *Source[i].X * Source[i].Y + Im->p[1] * (pow(R[i],2) + 2 * pow(Source[i].Y,2));
		dYSca[i] = Im->Add[0] * Source[i].Y;
		dYSkw[i] = Im->Add[1] * Source[i].X;
	}

	for (int i=0; i <N; i++) // dodajemy poprawki
	{
		Target[i] = Source[i];
		Target[i].X = Source[i].X - dXRad[i] - dXTan[i];
		Target[i].Y = Source[i].Y - dYRad[i] - dYTan[i] - dYSca[i] - dYSkw[i];
	}

	delete [] R;
	delete [] dXRad;
	delete [] dYRad;
	delete [] dXTan;
	delete [] dYTan;
	delete [] dYSca;
	delete [] dYSkw;
}

void fT_fid2uds(vector<ImagePoint> *Source, vector<ImagePoint> *Target, Camera* Cam)
{
	//Funkcja przelicza wspolrzedne tlowe na wspolrzedne tlowe bez dystorsji
	vector<double>  R;
	vector<double> dXRad;
	vector<double> dYRad;
	vector<double> dXTan;
	vector<double> dYTan;
	vector<double> dYSca;
	vector<double> dYSkw;
	
	for (int i=0; i < Source->size(); i++)
	{
		R.push_back( pow(pow(Source->at(i).X,2) + pow(Source->at(i).Y,2),0.5) );
	}
	
	for (int i=0; i < Source->size(); i++) // Obliczamy skladniki poprawek  z tytulu znieksztalcen systematycznych
	{
		dXRad.push_back( Source->at(i).X * (Cam->k1 * pow(R[i],2) + Cam->k2 * pow(R[i],4) + Cam->k3 * pow(R[i],6)) );
		dYRad.push_back( Source->at(i).Y * (Cam->k1 * pow(R[i],2) + Cam->k2 * pow(R[i],4) + Cam->k3 * pow(R[i],6)) );
		dXTan.push_back( Cam->p1 * ( pow(R[i],2) + 2* pow(Source->at(i).X,2) ) + 2 * Cam->p2 * Source->at(i).X * Source->at(i).Y );
		dYTan.push_back( 2 * Cam->p1 *Source->at(i).X * Source->at(i).Y + Cam->p2 * (pow(R[i],2) + 2 * pow(Source->at(i).Y,2)) );
		dYSca.push_back( Cam->y_scaling * Source->at(i).Y );
		dYSkw.push_back( Cam->skewness * Source->at(i).X );
	}
	
	ImagePoint P;

	for (int i=0; i <Source->size(); i++) // dodajemy poprawki
	{
		//cout <<Target[i].Name <<" distx: " << dXRad[i]<<" disty: " << dYRad[i] <<endl;
		P = Source->at(i);
		P.X = Source->at(i).X - dXRad[i] - dXTan[i];
		P.Y = Source->at(i).Y - dYRad[i] - dYTan[i] - dYSca[i] - dYSkw[i];
		Target->push_back(P);
	}

}

void fT_uds2fid(ImagePoint *Source, ImagePoint *Target, int N,Image *Obraz)
{
	// Funkcja do przeliczania wspolrzednych tlowych wolnych od dystorsji
	// Na wspolrzedne tlowe z dystorsja
	double Fidx, Fidy;
	double x, y;
	for (int i=0; i<N; i++)
	{
		Fidx = Source[i].X;
		Fidy = Source[i].Y;
		x = Fidx;
		y = Fidy;
		for (int j=0; j<5; j++)
		{	
			Fidx = x + fT_xDistCorrection(Obraz,Fidx,Fidy);
			Fidy = y + fT_yDistCorrection(Obraz,Fidx,Fidy);
			//dx = x - (Fidx - fT_xDistCorrection(Obraz,Fidx,Fidy));
			//dy = y - (Fidy - fT_yDistCorrection(Obraz,Fidx,Fidy));
			//cout << P[i].Name << j <<" dx: "<< dx <<" dy: "<<dy<<endl;
		}
	
		Target[i].X = Fidx;
		Target[i].Y = Fidy;
		Target[i].Name = Source[i].Name;
	}
}

void fT_fid2pix(ImagePoint *Source, ImagePoint *Target, int N, Image* Im)
{
	for (int i=0; i<N; i++)
	{
		Target[i] = Source[i];
		Target[i].X = Source[i].X + Im->Interior[1];
		Target[i].Y = Source[i].Y + Im->Interior[2];
	}
}

double fT_xDistCorrection (Image *Obraz, double x, double y)
{
	double rr = pow(x,2) + pow(y,2);
	return  x * (Obraz->k[0] * rr + Obraz->k[1] * pow(rr,2) + Obraz->k[2] * pow(rr,3)) + Obraz->p[0]*( rr + 2* pow(x,2)) + 2 * Obraz->p[1] * x * y;
}

double fT_yDistCorrection (Image *Obraz, double x, double y)
{
	double rr = pow(x,2) + pow(y,2);
	return  y * (Obraz->k[0] * rr + Obraz->k[1] * pow(rr,2) + Obraz->k[2] * pow(rr,3)) + 2*Obraz->p[0] * x * y + Obraz->p[1] * (rr + 2 * pow(y,2)) + Obraz->Add[0] * y + Obraz->Add[1] * x;
}

double fT_findFurthestImpoints (ImagePoint *P, int N, int * Index, int Type)
{
	//Funkcja odszukuje 2 najdalej od siebie polozone punkty
	//z tablicy P.
	//N- ilosc punktow w tablicy P
	//Index - w domysle tablica int[2] w ktorej zostana zapisane indeksy
	//dwoch najdalszych punktow
	//Funkcja jako wartosc zwraca odleglosc miedzy dwoma najdalszymi punktami
	double dist;
	double d = 0;
	int i,j;
	for (i=0; i < N; i++)
	{
		for (j= i+1; j<N; j++)
		{
			if ((P[i].Type == Type) && (P[j].Type == Type))
			{
				dist = fT_imdist(&P[i],&P[j]);
				if (dist > d)
				{
					d = dist;
					Index[0] = i;
					Index[1] = j;
				}
			}
		}
	}
	return d;
}

double fT_distFromLine(ImagePoint *P1, ImagePoint* P2, ImagePoint* P3)
{
	//P1, P2 - wskazniki do punktow przez ktore przechodzi prosta
	//P3 - wskaznik do punktu, ktorego odleglosc od prostej jest liczona
	//Funkcja zwraca odleglosc.

	//Obliczamy wsp. rownania ogolengo
	double A = P1->Y - P2->Y;
	double B = P2->X - P1->X;
	double C = P1->Y*(P1->X - P2->X) - P1->X*(P1->Y - P2->Y);
	double d;
	d = fabs(A*P3->X + B*P3->Y + C) / sqrt(pow(A,2)+pow(B,2));
	return d;

}

double fT_distFromLine(TerrainPoint *T1, TerrainPoint *T2, TerrainPoint *T3, TerrainPoint* ClP)
{
	//T1, T2 - punkty terenowe tworzace linie
	//T3 - punkt, ktorego odleglosc od linii jest liczona
	//ClP - do punktu terenowego. Tam zapisywane sa wspolrzedne punktu na prostej,
	//ktory lezy najblizej punktu T3
	//Funkcja zwraca wartosc odleglosci
	
	double p,q,r; //wsp wektora kierunkowego prostej
	double t; //parametr w rownaniu parametrycznym prostej
	double d; // odleglosc
	p = T2->X - T1->X;
	q = T2->Y - T1->Y;
	r = T2->Z - T1->Z;

	t = (p*T3->X - p*T1->X + q*T3->Y - q*T1->Y + r*T3->Z - r*T1->Z) / (pow(p,2) + pow(q,2) + pow(r,2));

	ClP->X = T1->X + p*t;
	ClP->Y = T1->Y + q*t;
	ClP->Z = T1->Z + r*t;

	d = fT_terrainDist(T3, ClP);
	return d;

}

void fT_transform2first(TerrainPoint *Source, TerrainPoint *Target, CvMat* R)
{
	//Source tablica TerrainPoint[3]
	//Target tablica TerrainPoint[3]
	//Funkcja trnasformuje wspolrzedne punktow Source w taki sposob, ze:
	//Pierwszy punkt w tablicy target ma wsp (0,0,0) a drugi (d(Source[0],Source[1]),0,1)
	//Wsp Y trzeciego punktu jest równa odleglosici trzeciego punktu w tablicy Source
	//od prostej wyznaczonej przez punkty Source[0] i Source[1]
	//R - macierz obrotu ukladu trzech punktow do ukladu pierowtnego
	TerrainPoint C;
	double d1 = fT_distFromLine(Source, Source+1, Source+2, &C);
	double d2 = fT_terrainDist(Source,&C);
	Target[0].X = 0;
	Target[0].Y = 0;
	Target[0].Z = 0;
	Target[1].X = fT_terrainDist(Source,Source+1);
	Target[1].Y = 0;
	Target[1].Z = 0;
	if (((C.X - Source[0].X) / (Source[1].X-Source[0].X)) < 0 ) // parametr t w rownaniu prostej jest ujemny
	{
		Target[2].X = -d2;
	}
	else // parametr t w rownaniu prostej jest nieujemny
	{
		Target[2].X = d2;
	}
	Target[2].Y = d1;
	Target[2].Z = 0;

	//Ustalenie macierzy obrotu
	CvMat * Vector_i = cvCreateMat(3,1,CV_64FC1);
	CvMat * Vector_j = cvCreateMat(3,1,CV_64FC1);
	CvMat * Vector_k = cvCreateMat(3,1,CV_64FC1);

	for (int row = 0; row < Vector_i->rows; row++)
	{
		double* ptr = (double*)(Vector_i->data.ptr + row*Vector_i->step);	
		switch (row)
		{
		case 0:
			*ptr = (Source[1].X - Source[0].X)/Target[1].X; 
			break;
		case 1:
			*ptr = (Source[1].Y - Source[0].Y)/Target[1].X; 
			break;
		case 2:
			*ptr = (Source[1].Z - Source[0].Z)/Target[1].X;
			break;
		}
	}

	for (int row = 0; row < Vector_j->rows; row++)
	{
		double* ptr = (double*)(Vector_j->data.ptr + row*Vector_j->step);	
		switch (row)
		{
		case 0:
			*ptr = (Source[2].X - C.X)/d1; 
			break;
		case 1:
			*ptr = (Source[2].Y - C.Y)/d1; 
			break;
		case 2:
			*ptr = (Source[2].Z - C.Z)/d1;
			break;
		}
	}

	cvCrossProduct(Vector_i,Vector_j,Vector_k);
	
	for (int row = 0; row < R->rows; row++)
	{
		double* ptr = (double*)(R->data.ptr + row*R->step);
		for(int col =0; col < R->cols; col++)
		{
			switch (col)
			{
			case 0:
				*ptr = cvGetReal2D(Vector_i,row,0);
				break;
			case 1:
				*ptr = cvGetReal2D(Vector_j,row,0);
				break;
			case 2:
				*ptr = cvGetReal2D(Vector_k,row,0);
				break;
			}
			ptr++;
		}
	}

}

void fT_biasCoords(TerrainPoint *Source, TerrainPoint *Dest, double DX, double DY, double DZ)
{
	//Funkcja dodaje do kazdej ze wspolrzednych punktu Source przyrosty DX, DY, DZ
	//Wynik zapisywany jest w dest;
	*Dest = *Source;
	Dest->X += DX;
	Dest->Y += DY;
	Dest->Z += DZ;
}

void fT_biasCoords(TerrainPoint *Source,  double DX, double DY, double DZ)
{
	//Funkcja dodaje do kazdej ze wspolrzednych punktu Source przyrosty DX, DY, DZ
	//Wynik zapisywany nie jest zapisywany w nowej zmiennej ale zmieniane sa wspolrzedne w Source
	Source->X += DX;
	Source->Y += DY;
	Source->Z += DZ;
}

short fT_trilateration(TerrainPoint *T, double *Dist, double *Result1, double *Result2)
{
	//Funkcja oblicza punkt przeciecia trzech sfer
	// T - punkty terenowe - srodki sfer
	// Dist - promienie sfer
	// Result1 i Result2 - wyniki obliczen - 2 punkty przeciecia trzech sfer
	// W razie brak rozwiazan Result1 = Result2 = NULL. Ew tylko Result 2 = NULL - jedno rozwiazanie.
	// Funkcja zwraca ilosc rozwiazan

	short NRes = 2; // Ilosc rozwiazan
	TerrainPoint Target[3]; // Tablica punktow po transformacji
	CvMat * R = cvCreateMat(3,3,CV_64FC1); // Macierz obrotu z ukladu Target do ukladu T
	fT_transform2first(T,Target,R);

	double x = (pow(Dist[0],2)-pow(Dist[1],2) + pow(Target[1].X,2)) / (2*Target[1].X);
	double y = (pow(Dist[0],2)-pow(Dist[2],2) + pow(Target[2].X,2) + pow(Target[2].Y,2))/ (2*Target[2].Y) - (Target[2].X * x)/Target[2].Y;
	
	double z1, z2;
	double z = pow(Dist[0],2) - pow(x,2) - pow(y,2);
	
	if (z < 0)
	{
		NRes = 0;		
		z2 = 0;
		z1 = 0;
	}
	
	if (z == 0)
	{
		NRes = 1;
		z2 = 0;
		z1 = 0;
	}
	
	if (z > 0)
	{
		NRes = 2;
		z2 = sqrt(z);
		z1 = - sqrt(z);
	}

	CvMat * V1 = cvCreateMat(3,1,CV_64FC1);
	CvMat * V2 = cvCreateMat(3,1,CV_64FC1);
	CvMat * V1T = cvCreateMat(3,1,CV_64FC1);
	CvMat * V2T = cvCreateMat(3,1,CV_64FC1);

	switch (NRes)
	{
	case 0:
		Result1 = 0;
		Result2 = 0;
		
		break;
	case 1:
		cvSetReal2D(V1,0,0,x);
		cvSetReal2D(V1,1,0,y);
		cvSetReal2D(V1,2,0,z1);
		cvGEMM(R,V1,1,NULL,0,V1T);
		
		Result1[0] = cvGetReal2D(V1T,0,0) + T[0].X;
		Result1[1] = cvGetReal2D(V1T,1,0) + T[0].Y;
		Result1[2] = cvGetReal2D(V1T,2,0) + T[0].Z;

		Result2 = 0;
		break;
	case 2:
		cvSetReal2D(V1,0,0,x);
		cvSetReal2D(V1,1,0,y);
		cvSetReal2D(V1,2,0,z1);		
		cvSetReal2D(V2,0,0,x);
		cvSetReal2D(V2,1,0,y);
		cvSetReal2D(V2,2,0,z2);

		cvGEMM(R,V1,1,NULL,0,V1T);
		cvGEMM(R,V2,1,NULL,0,V2T);
		
		Result1[0] = cvGetReal2D(V1T,0,0) + T[0].X;
		Result1[1] = cvGetReal2D(V1T,1,0) + T[0].Y;
		Result1[2] = cvGetReal2D(V1T,2,0) + T[0].Z;

		Result2[0] = cvGetReal2D(V2T,0,0) + T[0].X;
		Result2[1] = cvGetReal2D(V2T,1,0) + T[0].Y;
		Result2[2] = cvGetReal2D(V2T,2,0) + T[0].Z;
		break;
	}
		cvReleaseMat(&V1T);
		cvReleaseMat(&V2T);
		cvReleaseMat(&V1);
		cvReleaseMat(&V2);

	return NRes;
}

double fT_angleInCamera(ImagePoint *P1, ImagePoint*P2, double ck)
{
	//Funkcja oblicza kat w radianach o wierzcholku w srodku rzutu kamery
	//i o ramionach przechodzacych przez punkty P1 i P2. Punkty te
	//musza miec wspolrzedne w ukladzie tlowym. Funkcja zwraca wartosc kata.
	CvMat* Vect1 = cvCreateMat(3,1,CV_64FC1);
	CvMat* Vect2 = cvCreateMat(3,1,CV_64FC1);
	for (int row = 0; row < Vect1->rows; row++)
	{
		double* ptr = (double*)(Vect1->data.ptr + row*Vect1->step);	
		switch (row)
		{
		case 0:
			*ptr = P1->X;
			break;
		case 1:
			*ptr = P1->Y;
			break;
		case 2:
			*ptr = -ck;
			break;
		}
	}

	for (int row = 0; row < Vect2->rows; row++)
	{
		double* ptr = (double*)(Vect2->data.ptr + row*Vect2->step);
		switch (row)
		{
		case 0:
			*ptr = P2->X;
			break;
		case 1:
			*ptr = P2->Y;
			break;
		case 2:
			*ptr = -ck;
			break;
		}
	}
	return acos( cvDotProduct(Vect1,Vect2)/ (cvNorm(Vect1) * cvNorm(Vect2)) );	
}

double fT_distInCamera(ImagePoint *P, double ck)
{
	double d;
	d = sqrt(pow(P->X,2) + pow(P->Y,2) + pow(ck,2));
	return d;
}

void fT_tetrahedreon2rot(TerrainPoint *GCP, double *C, ImagePoint *P, double ck, CvMat* R)
{
	//Funkcja oblicza macierz obrotu na podstawie danych:
	//3 punktow kontrolnych - GCP
	//Srodka rzutu - C
	//3 punktow na obrazie (wsp tlowe) - P
	//stalej kamery - ck
	//R - wskaznik do zmiennej CvMat w ktorej zostanie zapisany wynik
	double Edge[3][3]; //Wektory od srodka rzutu do punktow na obrazie (w ukladzie terenowym)
	double ImgDist[3]; //Odleglosci od srodka rzutu do punkow na obrazie (w ukladzie terenowym)
	double TerDist[3]; //Odleglosci od srodka rzutu do fotopunktow (w ukladzie terenowym)

	for (int i = 0; i<3; i++)
	{
		ImgDist[i] = fT_distInCamera(&P[i],ck);
		TerDist[i] = fT_terrainDist(&GCP[i],C);
	}

	for (int i = 0; i<3; i++)
	{
		Edge[i][0] = C[0] +(GCP[i].X-C[0])*ImgDist[i]/TerDist[i];
		Edge[i][1] = C[1] +(GCP[i].Y-C[1])*ImgDist[i]/TerDist[i];
		Edge[i][2] = C[2] +(GCP[i].Z-C[2])*ImgDist[i]/TerDist[i];
	}

	CvMat * v_i = cvCreateMat(3,1,CV_64FC1); //Wektory jednostkowe 
	CvMat * v_j = cvCreateMat(3,1,CV_64FC1);
	CvMat * v_k = cvCreateMat(3,1,CV_64FC1);

	CvMat* wekt_12 = cvCreateMat(3,1,CV_64FC1);
	CvMat* wekt_13 = cvCreateMat(3,1,CV_64FC1);
	CvMat* wekt_12x13 = cvCreateMat(3,1,CV_64FC1);
	
	CvMat * RTer = cvCreateMat(3,3,CV_64FC1); //Macierze obrotu z ukladu terenowego i obrazu do ukladu punktow 12
	CvMat * RImg = cvCreateMat(3,3,CV_64FC1);
	
	// Obliczenie wektorow jednostkowych ukladu zdefiniowanego przez punkt 1 na obrazie oraz wygenerowanie macierzy obrotu
	
	// W ukladzie obrazu
	cvSetReal2D(wekt_12,0,0,P[1].X-P[0].X);
	cvSetReal2D(wekt_12,1,0,P[1].Y-P[0].Y);	
	cvSetReal2D(wekt_12,2,0,0);  //wsp z wynosi 0

	cvSetReal2D(wekt_13,0,0,P[2].X-P[0].X);
	cvSetReal2D(wekt_13,1,0,P[2].Y-P[0].Y);
	cvSetReal2D(wekt_13,2,0,0);  //wsp z wynosi 0



	cvCrossProduct(wekt_12,wekt_13,wekt_12x13);
	cvNormalize(wekt_12,v_i,1,0,CV_L2);
	cvNormalize(wekt_12x13,v_k,1,0,CV_L2);
	cvCrossProduct(v_k, v_i, v_j);

	//cout << "Wektory jednostkowe w ukladzie obrazu: "<<endl;
	//cout <<" i: " << " x: "<<cvGetReal2D(v_i,0,0)<< " y: "<<cvGetReal2D(v_i,1,0)<< " z: "<<cvGetReal2D(v_i,2,0) <<endl;
	//cout <<" j: " << " x: "<<cvGetReal2D(v_j,0,0)<< " y: "<<cvGetReal2D(v_j,1,0)<< " z: "<<cvGetReal2D(v_j,2,0) <<endl;
	//cout <<" k: " << " x: "<<cvGetReal2D(v_k,0,0)<< " y: "<<cvGetReal2D(v_k,1,0)<< " z: "<<cvGetReal2D(v_k,2,0) <<endl;

	for (int i=0; i<3; i++)
	{
		cvSetReal2D(RImg,i,0,cvGetReal2D(v_i,i,0));
		cvSetReal2D(RImg,i,1,cvGetReal2D(v_j,i,0));
		cvSetReal2D(RImg,i,2,cvGetReal2D(v_k,i,0));
	}
	
	// W ukladzie terenowym:
	for (int i = 0; i<3; i++) // Tu "i" oznacza indeks wspolrzednej
	{
		cvSetReal2D(wekt_12,i,0,Edge[1][i]-Edge[0][i] );
		cvSetReal2D(wekt_13,i,0,Edge[2][i]-Edge[0][i] );
	}
	
	cvCrossProduct(wekt_12,wekt_13,wekt_12x13);
	cvNormalize(wekt_12,v_i,1,0,CV_L2);
	cvNormalize(wekt_12x13,v_k,1,0,CV_L2);
	cvCrossProduct(v_k, v_i, v_j);
	
	//cout << "Wektory jednostkowe w ukladzie terenowym: "<<endl;
	//cout <<" i: " << " x: "<<cvGetReal2D(v_i,0,0)<< " y: "<<cvGetReal2D(v_i,1,0)<< " z: "<<cvGetReal2D(v_i,2,0) <<endl;
	//cout <<" j: " << " x: "<<cvGetReal2D(v_j,0,0)<< " y: "<<cvGetReal2D(v_j,1,0)<< " z: "<<cvGetReal2D(v_j,2,0) <<endl;
	//cout <<" k: " << " x: "<<cvGetReal2D(v_k,0,0)<< " y: "<<cvGetReal2D(v_k,1,0)<< " z: "<<cvGetReal2D(v_k,2,0) <<endl;
	
	for (int i=0; i<3; i++)
	{
		cvSetReal2D(RTer,i,0,cvGetReal2D(v_i,i,0));
		cvSetReal2D(RTer,i,1,cvGetReal2D(v_j,i,0));
		cvSetReal2D(RTer,i,2,cvGetReal2D(v_k,i,0));
	}
	
	
	
	cvGEMM(RTer,RImg,1,NULL,0,R,CV_GEMM_B_T);

	cvReleaseMat(&wekt_12);
	cvReleaseMat(&wekt_13);
	cvReleaseMat(&wekt_12x13);
	
	cvReleaseMat(&v_i);
	cvReleaseMat(&v_j);
	cvReleaseMat(&v_k);
	cvReleaseMat(&RTer);
	cvReleaseMat(&RImg);
}

double fT_checkEO(double *Coords, CvMat * R, TerrainPoint * T, ImagePoint *P, double ck)
{
	//Funkcja sprawdza czy elementy orientacji zdjecia sa poprawne
	//Sprawdzane jest czy punkt na zdjeciu znajduje sie pomiedzy srodkiem rzutu
	//a fotopunktem
	//Coords - tablica double[3] ze wspolrzednymi srodka rzutu
	//R - macierz obrotu
	//T - wybrany fotopunkt
	//P - odpowiadajace fotopunktowi punkt na zdjeciu (wsp tlowe)
	//ck - stala kamery

	CvMat * V_PCam = cvCreateMat(3,1,CV_64FC1);
	CvMat * V_PTer = cvCreateMat(3,1,CV_64FC1);
	CvMat * V_CT = cvCreateMat(3,1,CV_64FC1);
	//double PTer[3]; // Wsp punktu na zdjeciu w ukladzie terenowym
	cvSetReal2D(V_PCam,0,0,P->X);
	cvSetReal2D(V_PCam,1,0,P->Y);
	cvSetReal2D(V_PCam,2,0,-ck);
	cvGEMM(R,V_PCam,1,NULL,0,V_PTer);
	
	cvSetReal2D(V_CT,0,0,T->X-Coords[0]);
	cvSetReal2D(V_CT,1,0,T->Y-Coords[1]);
	cvSetReal2D(V_CT,2,0,T->Z-Coords[2]);
	
	double DotProd = cvDotProduct(V_PTer,V_CT);
	double d1 = fT_distInCamera(P,ck);
	double d2 = fT_terrainDist(T,Coords);

	cvReleaseMat(&V_PCam);
	cvReleaseMat(&V_PTer);

	return  DotProd/(d1*d2);
}

void fT_rot2angles(CvMat* MacierzObrotu, double* Angles, string System)
{
	string sys1("al-ni-ka");
	string sys2("om-fi-ka");
	double pi = 3.14159265358979323846;
	if (System == sys1)
	{
		Angles[1] = acos(cvGetReal2D(MacierzObrotu,2,2)); //kat ni

		if (cvGetReal2D(MacierzObrotu,0,2)/sin(Angles[1]) >= 0 ) //cwiartka I i II
		{
			Angles[0] = acos(-cvGetReal2D(MacierzObrotu,1,2)/sin(Angles[1])); //kat alfa
		}
		else //cwiartka III i IV
		{
			Angles[0] = 2*pi - acos(-cvGetReal2D(MacierzObrotu,1,2)/sin(Angles[1])); //kat alfa
		}

		if(cvGetReal2D(MacierzObrotu,2,0)/sin(Angles[1]) >= 0) //cwiartka I i II
		{
			Angles[2] = acos(cvGetReal2D(MacierzObrotu,2,1)/sin(Angles[1])); //kat kappa
		}
		else //cwiartkw III i IV
		{
			Angles[2] = 2*pi - acos(cvGetReal2D(MacierzObrotu,2,1)/sin(Angles[1])); //kat kappa
		}

		if (Angles[2] > pi) 
		{
			Angles[2] = Angles[2] - 2*pi; //aby kat kappa byl z przedzialu [-180;180]
		}

		Angles[0] = 180*Angles[0]/pi;
		Angles[1] = 180*Angles[1]/pi;
		Angles[2] = 180*Angles[2]/pi;
	}

	if (System == sys2)
	{

	}
}

void fT_rot2angles(double* R, double* Angles, string System)
{
	string sys1("al-ni-ka");
	string sys2("om-fi-ka");
	string sys3("roll-pitch-yaw");
	double pi = 3.14159265358979323846;
	
	if (System == sys1) //al-ni-ka
	{
		Angles[1] = acos(R[8]); //kat ni
		if (R[2]/sin(Angles[1]) >= 0 ) //cwiartka I i II
		{
			Angles[0] = acos(-R[5]/sin(Angles[1])); //kat alfa
		}
		else //cwiartka III i IV
		{
			Angles[0] = 2*pi - acos(-R[5]/sin(Angles[1])); //kat alfa
		}

		if(R[6]/sin(Angles[1]) >= 0) //cwiartka I i II
		{
			Angles[2] = acos(R[7]/sin(Angles[1])); //kat kappa
		}
		else //cwiartkw III i IV
		{
			Angles[2] = 2*pi - acos(R[7]/sin(Angles[1])); //kat kappa
		}

		if (Angles[2] > pi) 
		{
			Angles[2] = Angles[2] - 2*pi; //aby kat kappa byl z przedzialu [-180;180]
		}

	}

	if (System == sys2) //om-fi-ka
	{
		double omega, fi, kappa;
		//Trzeba przyjac jakis standard doboru kata fi
		//Zakladamy ze fi: <-pi/2:pi/2>
		//Dlatego mozemy policzyc fi w nastepujacy sposob:
		fi = asin(R[2]);
		
		//omega
		double b = -R[5]/cos(fi);
		double c = R[8]/cos(fi);

		if (b < 0)
		{
			if (c < 0) // III cwiartka
			{
				omega = pi - asin(b);
			}
			else // IV cwiartka
			{
				omega = asin(b) + 2*pi;
			}
		}
		else //b>=0
		{
			if (c < 0) //II cwiartka
			{
				omega = pi - asin(b);
			}
			else //I cwiartka
			{
				omega = asin(b);
			}
		}
	

		//kappa
		b = -R[1]/cos(fi);
		c = R[0]/cos(fi);

		if (b < 0)
		{
			if (c < 0) // III cwiartka
			{
				kappa = pi - asin(b);
			}
			else // IV cwiartka
			{
				kappa = asin(b) + 2*pi;
			}
		}
		else //b>=0
		{
			if (c < 0) //II cwiartka
			{
				kappa = pi - asin(b);
			}
			else //I cwiartka
			{
				kappa = asin(b);
			}
		}
		Angles[0] = omega;
		Angles[1] = fi;
		Angles[2] = kappa;
	}
	

	if (System == sys3) //roll-pitch-yaw
	{
		double roll, pitch, yaw;
		//Trzeba przyjac jakis standard doboru kata fi
		//Zakladamy ze pitch: <-pi/2:pi/2>
		//Dlatego mozemy policzyc fi w nastepujacy sposob:
		pitch = asin(-R[6]);
		
		//roll
		double b = R[7]/cos(pitch);
		double c = R[8]/cos(pitch);


		if (b < 0)
		{
			if (c < 0) // III cwiartka
			{
				roll = pi - asin(b);
			}
			else // IV cwiartka
			{
				roll = asin(b) + 2*pi;
			}
		}
		else //b>=0
		{
			if (c < 0) //II cwiartka
			{
				roll = pi - asin(b);
			}
			else //I cwiartka
			{
				roll = asin(b);
			}
		}
	

		//yaw
		b = R[3]/cos(pitch);
		c = R[0]/cos(pitch);

		if (b < 0)
		{
			if (c < 0) // III cwiartka
			{
				yaw = pi - asin(b);
			}
			else // IV cwiartka
			{
				yaw = asin(b) + 2*pi;
			}
		}
		else //b>=0
		{
			if (c < 0) //II cwiartka
			{
				yaw = pi - asin(b);
			}
			else //I cwiartka
			{
				yaw = asin(b);
			}
		}
		Angles[0] = roll;
		Angles[1] = pitch;
		Angles[2] = yaw;
	}
	
	//Zamiana na stopnie
	Angles[0] = 180*Angles[0]/pi;
	Angles[1] = 180*Angles[1]/pi;
	Angles[2] = 180*Angles[2]/pi;

}

void fT_rot2quaternions(CvMat* R, double* Q)
{
	//UWAGA! Trzeba rozwazyc przypadek kiedy pod pierwiastkiem jest < 0
	Q[0] = 0.5*pow(1.0 + cvGetReal2D(R,0,0) + cvGetReal2D(R,1,1)+ cvGetReal2D(R,2,2),0.5);
	Q[1] = (cvGetReal2D(R,1,2) - cvGetReal2D(R,2,1)) / (4*Q[0]);
	Q[2] = (cvGetReal2D(R,2,0) - cvGetReal2D(R,0,2)) / (4*Q[0]);
	Q[3] = (cvGetReal2D(R,0,1) - cvGetReal2D(R,1,0)) / (4*Q[0]);
}

void fT_rot2quaternions(double* R, double* Q)
{
	//UWAGA! Trzeba rozwazyc przypadek kiedy pod pierwiastkiem jest < 0 => dlatego sa 3 ify
	if ((R[4] > -R[8]) && (R[0] > - R[4]) && (R[0] > -R[8]))
	{
		Q[0] = 0.5*pow(1.0 + R[0] + R[4] + R[8],0.5);
		Q[1] = (R[7]-R[5])/(4*Q[0]);
		Q[2] = (R[2]-R[6])/(4*Q[0]);
		Q[3] = (R[3]-R[1])/(4*Q[0]);
	}
	else
	{
		if ((R[4] < -R[8]) && (R[0] > R[4]) && (R[0] > -R[8]))
		{
			Q[1] = 0.5*pow(1.0 + R[0] - R[4] - R[8],0.5);
			Q[0] = (R[7] - R[5])/(4*Q[1]);
			Q[2] = (R[3] + R[1])/(4*Q[1]);
			Q[3] = (R[2] + R[6])/(4*Q[1]);			
		}
		else
		{
			if ((R[4] > R[8]) && (R[0] < R[4]) && (R[0] < - R[8]))
			{
				Q[2] = 0.5*pow(1.0 - R[0] + R[4] - R[8],0.5);
				Q[0] = (R[2] - R[6])/(4*Q[2]);
				Q[1] = (R[3] + R[1])/(4*Q[2]);
				Q[3] = (R[7] + R[5])/(4*Q[2]);
			}
			else
			{
				Q[3] = 0.5*pow(1.0 - R[0] - R[4] + R[8],0.5);
				Q[0] = (R[3] - R[1])/(4*Q[3]);
				Q[1] = (R[2] + R[6])/(4*Q[3]);
				Q[2] = (R[7] + R[5])/(4*Q[3]);
			}
		}
	}

	
}

void fT_rot2vector(CvMat * Rot, double *R)
{
	R[0] = cvGetReal2D(Rot,0,0);
	R[1] = cvGetReal2D(Rot,0,1);
	R[2] = cvGetReal2D(Rot,0,2);
	R[3] = cvGetReal2D(Rot,1,0);
	R[4] = cvGetReal2D(Rot,1,1);
	R[5] = cvGetReal2D(Rot,1,2);
	R[6] = cvGetReal2D(Rot,2,0);
	R[7] = cvGetReal2D(Rot,2,1);
	R[8] = cvGetReal2D(Rot,2,2);
}

void fT_quaternions2rot(double * Q, CvMat *R)
{
	cvSetReal2D(R,0,0, pow(Q[0],2) + pow(Q[1],2) - pow(Q[2],2)- pow(Q[3],2) );
	cvSetReal2D(R,0,1, 2*(Q[1]*Q[2] - Q[0]*Q[3]) );
	cvSetReal2D(R,0,2, 2*(Q[1]*Q[3] + Q[0]*Q[2]) );
	
	cvSetReal2D(R,1,0, 2*(Q[1]*Q[2] + Q[0]*Q[3]) );
	cvSetReal2D(R,1,1, pow(Q[0],2) - pow(Q[1],2) + pow(Q[2],2)- pow(Q[3],2) );
	cvSetReal2D(R,1,2, 2*(Q[2]*Q[3] - Q[0]*Q[1]) );

	cvSetReal2D(R,2,0, 2*(Q[1]*Q[3] - Q[0]*Q[2]) );
	cvSetReal2D(R,2,1, 2*(Q[2]*Q[3] + Q[0]*Q[1]) );
	cvSetReal2D(R,2,2, pow(Q[0],2) - pow(Q[1],2) - pow(Q[2],2)+ pow(Q[3],2) );
}

void fT_quaternions2rot(double * Q, double *R)
{
	double RR[9];

	RR[0] = Q[0]*Q[0] + Q[1]*Q[1] - Q[2]*Q[2] - Q[3]*Q[3];
	//RR[0] = pow(Q[0],2) + pow(Q[1],2) - pow(Q[2],2)- pow(Q[3],2) ;
	RR[1] = 2*(Q[1]*Q[2] - Q[0]*Q[3]);
	RR[2] = 2*(Q[1]*Q[3] + Q[0]*Q[2]);
	
	RR[3] = 2*(Q[1]*Q[2] + Q[0]*Q[3]);
	RR[4] = Q[0]*Q[0] - Q[1]*Q[1] + Q[2]*Q[2] - Q[3]*Q[3];
	//RR[4] = pow(Q[0],2) - pow(Q[1],2) + pow(Q[2],2)- pow(Q[3],2) ;
	RR[5] = 2*(Q[2]*Q[3] - Q[0]*Q[1]);

	RR[6] = 2*(Q[1]*Q[3] - Q[0]*Q[2]);
	RR[7] = 2*(Q[2]*Q[3] + Q[0]*Q[1]);
	RR[8] = Q[0]*Q[0] - Q[1]*Q[1] - Q[2]*Q[2] + Q[3]*Q[3];
	//RR[8] = pow(Q[0],2) - pow(Q[1],2) - pow(Q[2],2)+ pow(Q[3],2) ;

	fT_orthonormalize(RR,R); //na koniec ortonormalizacja
}

void fT_angles2rot(double *Angles, CvMat *R, string System)
{
	string sys1("al-ni-ka");
	string sys2("om-fi-ka");
	string sys3("roll-pitch-yaw");
	double pi = 3.14159265358979323846;

	if (System == sys1)
	{
		double alfa, ni, kappa; // Nazwy dla czytelnosci zapisu		
		alfa = Angles[0]*pi/180;
		ni = Angles[1]*pi/180;
		kappa = Angles[2]*pi/180;

		cvSetReal2D(R,0,0,  cos(alfa)*cos(kappa) - sin(alfa)*cos(ni)*sin(kappa) );
		cvSetReal2D(R,0,1, -cos(alfa)*sin(kappa) - sin(alfa)*cos(ni)*cos(kappa) );
		cvSetReal2D(R,0,2,  sin(alfa)*sin(ni) );
		
		cvSetReal2D(R,1,0,  sin(alfa)*cos(kappa) + cos(alfa)*cos(ni)*sin(kappa) );
		cvSetReal2D(R,1,1, -sin(alfa)*sin(kappa) + cos(alfa)*cos(ni)*cos(kappa) );
		cvSetReal2D(R,1,2, -cos(alfa)*sin(ni) );

		cvSetReal2D(R,2,0, sin(ni)*sin(kappa) );
		cvSetReal2D(R,2,1, sin(ni)*cos(kappa) );
		cvSetReal2D(R,2,2, cos(ni) );
	}
	if (System == sys2)
	{

	}
}

void fT_angles2rot(double *Angles, double *R, string System)
{
	string sys1("al-ni-ka");
	string sys2("om-fi-ka");
	string sys3("roll-pitch-yaw");
	double pi = 3.14159265358979323846;
	if (System == sys1)
	{
		double alfa, ni, kappa; // Nazwy dla czytelnosci zapisu		
		alfa = Angles[0]*pi/180;
		ni = Angles[1]*pi/180;
		kappa = Angles[2]*pi/180;
		
		//Deklaracja skladowych macierzy obrotu
		double Ra[9];
		double Rn[9];
		double Rk[9];
		
		//Inicjalizacja Ra
		Ra[0] = cos(alfa);		Ra[1] = -sin(alfa);		Ra[2] = 0;
		Ra[3] = sin(alfa);		Ra[4] = cos(alfa);		Ra[5] = 0;
		Ra[6] = 0;				Ra[7] = 0;				Ra[8] = 1;

		//Inicjalizacja Rn
		Rn[0] = 1;				Rn[1] = 0;				Rn[2] = 0;
		Rn[3] = 0;				Rn[4] = cos(ni);		Rn[5] = -sin(ni);
		Rn[6] = 0;				Rn[7] = sin(ni);		Rn[8] = cos(ni);

		//Inicjalizacja Rk
		Rk[0] = cos(kappa);		Rk[1] = -sin(kappa);	Rk[2] = 0;
		Rk[3] = sin(kappa);		Rk[4] = cos(kappa);		Rk[5] = 0;
		Rk[6] = 0;				Rk[7] = 0;				Rk[8] = 1;

		double Rnk[9];
		fT_matMult3(Rn,Rk,Rnk);
		fT_matMult3(Ra,Rnk,R);
		//cout<<"Kontrola: "<<R[0]<<" "<<R[1]<<" "<<R[2]<<" "<<R[3]<<endl;
	}

	if (System == sys2)
	{
		double omega, fi, kappa; // Nazwy dla czytelnosci zapisu		
		omega = Angles[0]*pi/180;
		fi = Angles[1]*pi/180;
		kappa = Angles[2]*pi/180;

		//Deklaracja skladowych macierzy obrotu
		double Ro[9];
		double Rf[9];
		double Rk[9];
		
		//Inicjalizacja Ro
		Ro[0] = 1;				Ro[1] = 0;				Ro[2] = 0;
		Ro[3] = 0;				Ro[4] = cos(omega);		Ro[5] = -sin(omega);
		Ro[6] = 0;				Ro[7] = sin(omega);		Ro[8] = cos(omega);

		//Inicjalizacja Rf
		Rf[0] = cos(fi);		Rf[1] = 0;				Rf[2] = sin(fi);
		Rf[3] = 0;				Rf[4] = 1;				Rf[5] = 0;
		Rf[6] = -sin(fi);		Rf[7] = 0;				Rf[8] = cos(fi);

		//Inicjalizacja Rk
		Rk[0] = cos(kappa);		Rk[1] = -sin(kappa);	Rk[2] = 0;
		Rk[3] = sin(kappa);		Rk[4] = cos(kappa);		Rk[5] = 0;
		Rk[6] = 0;				Rk[7] = 0;				Rk[8] = 1;

		double Rfk[9];
		fT_matMult3(Rf,Rk,Rfk);
		fT_matMult3(Ro,Rfk,R);
	}

	if (System == sys3)
	{
		double roll, pitch, yaw; // Nazwy dla czytelnosci zapisu		
		roll = Angles[0]*pi/180;
		pitch = Angles[1]*pi/180;
		yaw = Angles[2]*pi/180;

		//Deklaracja skladowych macierzy obrotu
		double Rr[9];
		double Rp[9];
		double Ry[9];
		
		//Inicjalizacja Rr
		Rr[0] = 1;				Rr[1] = 0;				Rr[2] = 0;
		Rr[3] = 0;				Rr[4] = cos(roll);		Rr[5] = -sin(roll);
		Rr[6] = 0;				Rr[7] = sin(roll);		Rr[8] = cos(roll);

		//Inicjalizacja Rp
		Rp[0] = cos(pitch);		Rp[1] = 0;				Rp[2] = sin(pitch);
		Rp[3] = 0;				Rp[4] = 1;				Rp[5] = 0;
		Rp[6] = -sin(pitch);	Rp[7] = 0;				Rp[8] = cos(pitch);

		//Inicjalizacja Ry
		Ry[0] = cos(yaw);		Ry[1] = -sin(yaw);		Ry[2] = 0;
		Ry[3] = sin(yaw);		Ry[4] = cos(yaw);		Ry[5] = 0;
		Ry[6] = 0;				Ry[7] = 0;				Ry[8] = 1;

		double Rpr[9];
		fT_matMult3(Rp,Rr,Rpr);
		fT_matMult3(Ry,Rpr,R);
	}
}

void fT_findEpiR(Image* Obraz1, Image* Obraz2, double *R)
{
	//funkcja na podstawie danych o obrazach Obraz1 i Obraz2
	//wyznacza odpowiednia macierz obrotu dla stworzenia obrazow epipolarnych
	double pi = 3.14159265358979323846;
	double R1T[9];
	double R2[9];
	fT_transpose3(Obraz1->R,R1T);
	fT_matMult3(R1T,Obraz2->R,R2);
	double Angles[3];
	fT_rot2angles(R2,Angles,"om-fi-ka");
	double B_TER[3]; //baza w ukladzie terenowym
	B_TER[0] = Obraz2->CoordsAdj[0] - Obraz1->CoordsAdj[0];
	B_TER[1] = Obraz2->CoordsAdj[1] - Obraz1->CoordsAdj[1];
	B_TER[2] = Obraz2->CoordsAdj[2] - Obraz1->CoordsAdj[2];
	double B[3]; //baza w ukladzie pierwszego zdjecia
	fT_vectorTrans3(R1T,B_TER,B);
	std::cout<<"El orientacji zdjecia r wzgledem zdjecia l"<<endl;
	std::cout<<"katy: "<<Angles[0]<<" "<<Angles[1]<<" "<<Angles[2]<<endl;
	std::cout<<"baza: "<<B[0]<<" "<<B[1]<<" "<<B[2]<<endl;
	double NewAng[3];
	double NewR[9]; //macierz obrotu z ukladu zdjecia epipolarnego do ukladu pierwszego zdjecia
	NewAng[0] = fT_angMean(0,Angles[0]);
	NewAng[1] = -180*atan(B[2]/sqrt(B[0]*B[0]+B[1]*B[1]))/pi;
	NewAng[2] = 180*atan(B[1]/B[0])/pi;
	std::cout<<"nowe katy "<<NewAng[0]<<" "<<NewAng[1]<<" "<<NewAng[2]<<endl;
	fT_angles2rot(NewAng,NewR,"roll-pitch-yaw");
	double NewRT[9];
	fT_transpose3(NewR,NewRT); //NewRT - macierz obrotu z ukladu pierwszego zdjecia do ukladu zdjecia epipolarnego

	//kontrolna transformacja wspolrzednych wektora bazy z ukladu terenowego do ukladu pierwszego zdjecia
	std::cout<<"kontrolna transformacja wspolrzednych wektora bazy z ukladu pierwszego zdjecia do ukladu zdjec epipolarnych: "<<endl;
	double BEpi[3]; //wsp. wektora bazy w ukladzie zdjec epipolarnych
	fT_vectorTrans3(NewRT,B,BEpi);
	std::cout<<"wektor bazy: "<<BEpi[0] <<" "<<BEpi[1]<<" "<<BEpi[2]<<endl;

	//R bedzie macierza obrotu z ukladu zdjecia epipolarnego do ukladu terenowego
	fT_matMult3(Obraz1->R,NewR,R);
}

void fT_orientation2dlt(double *Io, double *Coords, CvMat *R, double *DLT)
{
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4;
	a1 = Io[1]*cvGetReal2D(R,0,2)-Io[0]*cvGetReal2D(R,0,0);
	a2 = Io[1]*cvGetReal2D(R,1,2)-Io[0]*cvGetReal2D(R,1,0);	
	a3 = Io[1]*cvGetReal2D(R,2,2)-Io[0]*cvGetReal2D(R,2,0);
	a4 = -a1*Coords[0]-a2*Coords[1]-a3*Coords[2];

	b1 = Io[2]*cvGetReal2D(R,0,2)-Io[0]*cvGetReal2D(R,0,1);
	b2 = Io[2]*cvGetReal2D(R,1,2)-Io[0]*cvGetReal2D(R,1,1);	
	b3 = Io[2]*cvGetReal2D(R,2,2)-Io[0]*cvGetReal2D(R,2,1);
	b4 = -b1*Coords[0]-b2*Coords[1]-b3*Coords[2];

	c1 = cvGetReal2D(R,0,2);
	c2 = cvGetReal2D(R,1,2);
	c3 = cvGetReal2D(R,2,2);
	c4 = -cvGetReal2D(R,0,2)*Coords[0] - cvGetReal2D(R,1,2)*Coords[1] - cvGetReal2D(R,2,2)*Coords[2];

	DLT[0] = a1/c4;
	DLT[1] = a2/c4;
	DLT[2] = a3/c4;
	DLT[3] = a4/c4;
	DLT[4] = b1/c4;
	DLT[5] = b2/c4;
	DLT[6] = b3/c4;
	DLT[7] = b4/c4;
	DLT[8] = c1/c4;
	DLT[9] = c2/c4;
	DLT[10] = c3/c4;
}

void fT_orientation2dlt(double *Io, double *Coords, double *Angles, string System, double *DLT)
{
	CvMat * R = cvCreateMat(3,3,CV_64FC1);
	fT_angles2rot(Angles,R,System);
	fT_orientation2dlt(Io,Coords,R,DLT);
	cvReleaseMat(&R);
}

void fT_rotavg(CvMat*Source1, CvMat*Source2, CvMat *Res)
{
	double Q1[4];
	double Q2[4];
	double Q3[4];
	fT_rot2quaternions(Source1,Q1);
	fT_rot2quaternions(Source2,Q2);

	double Mi[3];
	double Mix[3];
	double Miy[3];
	double Miz[3];

	Mi[0] = 2 * acos(Q1[0]);
	Mix[0] = Mi[0]*Q1[1] / sin(Mi[0]/2);
	Miy[0] = Mi[0]*Q1[2] / sin(Mi[0]/2);
	Miz[0] = Mi[0]*Q1[3] / sin(Mi[0]/2);	

	Mi[1] = 2 * acos(Q2[0]);
	Mix[1] = Mi[1]*Q2[1] / sin(Mi[1]/2);
	Miy[1] = Mi[1]*Q2[2] / sin(Mi[1]/2);
	Miz[1] = Mi[1]*Q2[3] / sin(Mi[1]/2);

	Mix[2] = (Mix[0] + Mix[1])/2;
	Miy[2] = (Miy[0] + Miy[1])/2;
	Miz[2] = (Miz[0] + Miz[1])/2;
	Mi[2] = sqrt(pow(Mix[2],2) + pow(Miy[2],2) + pow(Miz[2],2));
	
	Q3[0] = cos(Mi[2]/2);
	Q3[1] = (Mix[2]/Mi[2])*sin(Mi[2]/2);
	Q3[2] = (Miy[2]/Mi[2])*sin(Mi[2]/2);
	Q3[3] = (Miz[2]/Mi[2])*sin(Mi[2]/2);

	double d = pow(Q3[0],2) + pow(Q3[1],2) + pow(Q3[2],2) + pow(Q3[3],2);
		
	fT_quaternions2rot(Q3, Res);
 
}

void fT_sel4Points4Resection(ImagePoint *P, int N, int* Index)
{
	int FurthestP[2];
	int P3idx = 0;
	int P4idx = 0;
	fT_findFurthestImpoints(P,N,FurthestP,3);
	//cout <<"Najdalsze punkty: " << FurthestP[0] <<" "<<FurthestP[1] <<endl;
	double d = 0;
	double dist;
	for (int i=0; i<N; i++)
	{
		if (P[i].Type == 3)
		{
			//cout <<"Szukam odleglosci od linii. Punkt: " << P[i].Name <<" Typ: " <<P[i].Type <<endl;
			dist = fT_distFromLine(&P[FurthestP[0]], &P[FurthestP[1]], &P[i]);
			if (dist > d)
			{
				d = dist;
				P3idx = i;
			}
		}
	}
	d = 0;
	double d1, d2, d3;
	for (int i=0; i<N; i++)
	{
		if ((i!= FurthestP[0]) && (i!= FurthestP[1]) && (i!= P3idx) && (P[i].Type ==3) )
		{
			d1 = fT_distFromLine(&P[FurthestP[0]], &P[FurthestP[1]], &P[i]);
			d2 = fT_distFromLine(&P[FurthestP[0]], &P[P3idx], &P[i]);
			d3 = fT_distFromLine(&P[FurthestP[1]], &P[P3idx], &P[i]);
			dist = pow(d1,0.5) + pow(d2,0.5) + pow(d3,0.5);
			if (dist > d)
			{
				d = dist;
				P4idx = i;
			}
		}
	}
	Index[0] = FurthestP[0];
	Index[1] = FurthestP[1];
	Index[2] = P3idx;
	Index[3] = P4idx;
	//cout<<"Trzeci punkt: "<<Index[2] << " Typ: "  <<P[Index[2]].Type<<" Nazwa: "<<P[Index[2]].Name <<endl;
	//cout<<"Czwarty punkt: " <<Index[3] <<endl;
}

short fT_killianResection(ImagePoint* P, TerrainPoint * T, double ck, double (*ProjCenter)[3], CvMat *RotationMatrix[3])
{
	//ETAP 1 - Wyznaczenie odleglosci od punktow kontrolnych - wstep do trilateracji
	//Obliczamy katy miedzy promieniami do punktow
	double alfa = fT_angleInCamera(&P[0], &P[1], ck);
	double beta1 = fT_angleInCamera(&P[1], &P[2], ck); 
	double gamma1 = fT_angleInCamera(&P[0], &P[2], ck); 
	double beta2 = fT_angleInCamera(&P[1], &P[3], ck);
	double gamma2 = fT_angleInCamera(&P[0], &P[3], ck);

	//Obliczamy odleglosci pomiedzy punktami terenowymi
	long double a = fT_terrainDist(T, T+1);
	long double b1 = fT_terrainDist(T+1, T+2);
	long double c1 = fT_terrainDist(T, T+2);
	long double b2 = fT_terrainDist(T+1, T+3);
	long double c2 = fT_terrainDist(T, T+3);

	//Wielkosci pomicnicze do wspolczynnikow rownania 4tego stopnia
	long double A1 = (pow(b1,2) + pow(c1,2))/pow(a,2);
	long double B1 = pow((pow(b1,2) - pow(c1,2))/pow(a,2),2);
	long double C1 = pow(c1,2)/pow(a,2);
	long double D1 = pow(b1,2)/pow(a,2);
	
	long double A2 = (pow(b2,2) + pow(c2,2))/pow(a,2);
	long double B2 = pow((pow(b2,2) - pow(c2,2))/pow(a,2),2);
	long double C2 = pow(c2,2)/pow(a,2);
	long double D2 = pow(b2,2)/pow(a,2);

	long double VV_1[4]; //Tablice pod pierwiastki rownania 4tego stopnia
	long double VV_2[4];
	long double Cof_1[5]; //Tablice pod wspolczynniki rownania 4tego stopnia
	long double Cof_2[5];	

	// Dwa rownania 4-tego stopnia:
	// Obliczenie wspolczynnikow
	// Rownanie 1:
	Cof_1[0] = 1 - 2*A1 + B1 + 4*C1 *pow(sin(beta1),2);
	Cof_1[1] = 4*(- cos(beta1)*cos(gamma1) + A1*( cos(beta1)*cos(gamma1) + cos(alfa) ) - B1*cos(alfa) - 2*C1*pow(sin(beta1),2)*cos(alfa));
	Cof_1[2] = 2*( 1+2*( pow(cos(beta1),2) - pow(sin(gamma1),2)) - 2*A1*(1+2*cos(beta1)*cos(alfa)*cos(gamma1)) + B1*(1+2*pow(cos(alfa),2)) + (2/pow(a,2)) *(pow(b1,2)*pow(sin(gamma1),2) + pow(c1,2)*pow(sin(beta1),2)));
	Cof_1[3] = 4*(- cos(beta1)*cos(gamma1) + A1*( cos(beta1)*cos(gamma1) + cos(alfa) ) - B1*cos(alfa) -2*D1*pow(sin(gamma1),2)*cos(alfa));
	Cof_1[4] = 1 - 2*A1 + B1 + 4*D1*pow(sin(gamma1),2);

	// Rownanie 2:
	Cof_2[0] = 1 - 2*A2 + B2 + 4*C2 *pow(sin(beta2),2);
	Cof_2[1] = 4*(- cos(beta2)*cos(gamma2) + A2*( cos(beta2)*cos(gamma2) + cos(alfa) ) - B2*cos(alfa) - 2*C2*pow(sin(beta2),2)*cos(alfa));
	Cof_2[2] = 2*( 1+2*( pow(cos(beta2),2) - pow(sin(gamma2),2)) - 2*A2*(1+2*cos(beta2)*cos(alfa)*cos(gamma2)) + B2*(1+2*pow(cos(alfa),2)) + (2/pow(a,2)) *(pow(b2,2)*pow(sin(gamma2),2) + pow(c2,2)*pow(sin(beta2),2)));
	Cof_2[3] = 4*(- cos(beta2)*cos(gamma2) + A2*( cos(beta2)*cos(gamma2) + cos(alfa) ) - B2*cos(alfa) -2*D2*pow(sin(gamma2),2)*cos(alfa));
	Cof_2[4] = 1 - 2*A2 + B2 + 4*D2*pow(sin(gamma2),2);

	//Rozwiazanie rownan czwartego stopnia
	short N; //Ilosc pierwiastkow rownania
	N = ft_solveQuatricsReal(Cof_1,VV_1);
	if (N==0) return 0;
	N = ft_solveQuatricsReal(Cof_2,VV_2);
	if (N==0) return 0;	

	double v = ft_findV(VV_1,VV_2); //Szukamy dwoch v z obu rozwiazan ktore sa sobie najblizsze
	double r[4]; //Tablica pod dlogosci do trilateracji
	//Obliczenie dlogosci na podstawie v
	/*r1*/ r[0] = sqrt(pow(a,2) / (1 + pow(v,2) - 2*v*cos(alfa))); 
	/*r2*/ r[1] = v * r[0];
	
	double Coefs[3]; //Wspolczynniki rownania kwadratowego;
	//Beda cztery rownania drugiego stopnia na r3 i na r4
	double Results1[4]; 
	double Results2[4];
	double Results3[4];
	double Results4[4];
	
	//ROWNANIE NA r3
	Coefs[0] = 1;
	Coefs[1] = -2*r[1]*cos(beta1);
	Coefs[2] = pow(r[1],2) - pow(b1,2);
	N = ft_solveQuadraticEquation(Coefs,Results1);
	if (N ==0) return 0;
	Results1[2] = 0;
	Results1[3] = 0;
	Coefs[0] = 1;
	Coefs[1] = -2*r[0]*cos(gamma1);
	Coefs[2] = pow(r[0],2) - pow(c1,2);
	N = ft_solveQuadraticEquation(Coefs,Results2);
	if (N ==0) return 0;
	Results2[2] = 0;
	Results2[3] = 0;
	r[2] = ft_findV(Results1,Results2);

	//ROWNANIE NA r4
	Coefs[0] = 1;
	Coefs[1] = -2*r[1]*cos(beta2);
	Coefs[2] = pow(r[1],2) - pow(b2,2);
	N = ft_solveQuadraticEquation(Coefs,Results3);
	if (N ==0) return 0;
	Results3[2] = 0;
	Results3[3] = 0;
	Coefs[0] = 1;
	Coefs[1] = -2*r[0]*cos(gamma2);
	Coefs[2] = pow(r[0],2) - pow(c2,2);
	N = ft_solveQuadraticEquation(Coefs,Results4);
	if (N ==0) return 0;
	Results4[2] = 0;
	Results4[3] = 0;
	r[3] = ft_findV(Results3,Results4);


	//ETAP 2 - Trilateracja
	TerrainPoint Trig1[3];
	TerrainPoint Trig2[3];
	double Dist1[3];
	double Dist2[3];
	double Result[4][3];
	short NResult_1, NResult_2;
	
	Trig1[0] = T[0]; Dist1[0] = r[0];
	Trig1[1] = T[1]; Dist1[1] = r[1];
	Trig1[2] = T[2]; Dist1[2] = r[2];
 
	Trig2[0] = T[0]; Dist2[0] = r[0];
	Trig2[1] = T[1]; Dist2[1] = r[1];
	Trig2[2] = T[3]; Dist2[2] = r[3];

	//Obliczenie trilateracji
	NResult_1 = fT_trilateration(Trig1, Dist1, Result[0], Result[1]);
	NResult_2 = fT_trilateration(Trig2, Dist2, Result[2], Result[3]);


	//ETAP 3 - Obliczenie macierzy obrotow
	ImagePoint ImP1[3]; //Pierwszy trojkat na obrazie
	ImagePoint ImP2[3]; //Drugi trojkat na obrazie

	ImP1[0] = P[0];
	ImP1[1] = P[1];
	ImP1[2] = P[2];

	ImP2[0] = P[0];
	ImP2[1] = P[1];
	ImP2[2] = P[3];

	CvMat * R[4]; // 4 potencjalne rozwiazania macierzy obrotu
	for (int i = 0; i< 4; i++)
	{
		R[i] = cvCreateMat(3,3,CV_64FC1);
		if (&Result[i])
		{
			if (( i == 0) | (i==1))
			{
				fT_tetrahedreon2rot(Trig1, Result[i], ImP1, ck, R[i]);
			}
			else // i==2 | i==3
			{
				fT_tetrahedreon2rot(Trig2, Result[i], ImP2, ck, R[i]);
			}
		}
		else
		{
			R[i] = 0;
		}
	}
	
	if	(NResult_1 + NResult_2 != 4) // Brak jednego z 4 rozwiazan. Oznacza ze cos jest nie tak. Bledne pomiary.
	{
		return 0;
	}

	//ETAP 4 - Wybor rozwiazan prawidlowych
	bool ValidResults[4];
	short N_ValidResults = 0; //Ilosc rozwiazan prawidlowych
	for (int i = 0; i< 4; i++)
	{
		if (&Result[i])
		{
			if (fT_checkEO(Result[i], R[i], Trig1, ImP1, ck) > 0.5) // Iloczyn skalarny powinien byc dodatni i bliski 1
			{
				ValidResults[i] = true;
				N_ValidResults ++;
			}
			else
			{
				ValidResults[i] = false;
			}
		}
		else
		{	
			ValidResults[i] = false;
		}
	}

	if (N_ValidResults !=2) //Macierze obrotu nie mogly byc prawidlowo obliczone
	{
		return 0;
	}
	
	short CoordIdx[2]; //Dwa indeksy wspolrzednych prawidlowcy
	short RotIdx[2]; //Dwa indeksy poprawnych macierzy obrotu

	int Idx = 0; //    Idx == 0 lub 1
	for (int i = 0; i< 4; i++)
	{
		if (ValidResults[i] == true)
		{
			CoordIdx[Idx] = i;
			RotIdx[Idx] = i;
			Idx++;
		}
	}

	//ETAP 5 - Usrednianie i przypisanie wynikow
	//Przypisanie dwoch wynikow wspolrzednych
	ProjCenter[0][0] = Result[CoordIdx[0]][0];
	ProjCenter[0][1] = Result[CoordIdx[0]][1];
	ProjCenter[0][2] = Result[CoordIdx[0]][2];
	ProjCenter[1][0] = Result[CoordIdx[1]][0];
	ProjCenter[1][1] = Result[CoordIdx[1]][1];
	ProjCenter[1][2] = Result[CoordIdx[1]][2];
	
	//Usrednianie wspolrzednych
	ProjCenter[2][0] = (Result[CoordIdx[0]][0] + Result[CoordIdx[1]][0])/2;
	ProjCenter[2][1] = (Result[CoordIdx[0]][1] + Result[CoordIdx[1]][1])/2;
	ProjCenter[2][2] = (Result[CoordIdx[0]][2] + Result[CoordIdx[1]][2])/2;
	
	//Usrednianie macierzy obrotu poprzez quaterniony
	cvCopy(R[RotIdx[0]],RotationMatrix[0]);
	cvCopy(R[RotIdx[1]],RotationMatrix[1]);
	fT_rotavg(R[RotIdx[0]], R[RotIdx[1]], RotationMatrix[2]);
	return 1;
}

void fT_ground2ImageByDLT(TerrainPoint *T,double *DLT, int N, ImagePoint* P )
{
	double denom;
	for (int i=0; i<N; i++)
	{
		denom =  DLT[8]* T[i].X + DLT[9]* T[i].Y + DLT[10]* T[i].Z + 1;
		P[i].X = (DLT[0]* T[i].X + DLT[1]* T[i].Y + DLT[2]* T[i].Z + DLT[3])/denom;
		P[i].Y = (DLT[4]* T[i].X + DLT[5]* T[i].Y + DLT[6]* T[i].Z + DLT[7])/denom;
		P[i].Name = T[i].Name;
	}
}

void fT_ground2Image(TerrainPoint* T, Image* Obraz, int N, ImagePoint *P)
{
	double Lx, Ly, M;
	double x, y;
	double Fidx, Fidy;
	double dx, dy;
	//double Distx, Disty;

	for (int i=0; i<N; i++)
	{
		Lx = Obraz->R[0]*(T[i].X - Obraz->CoordsAdj[0]) + Obraz->R[3]*(T[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[6]*(T[i].Z - Obraz->CoordsAdj[2]);
		Ly = Obraz->R[1]*(T[i].X - Obraz->CoordsAdj[0]) + Obraz->R[4]*(T[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[7]*(T[i].Z - Obraz->CoordsAdj[2]);
		M = Obraz->R[2]*(T[i].X - Obraz->CoordsAdj[0]) + Obraz->R[5]*(T[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[8]*(T[i].Z - Obraz->CoordsAdj[2]);
		x = -Obraz->Interior[0]*Lx/M;
		y = -Obraz->Interior[0]*Ly/M;
		Fidx = x;
		Fidy = y;
		for (int j=0; j<5; j++)
		{	
			Fidx = x + fT_xDistCorrection(Obraz,Fidx,Fidy);
			Fidy = y + fT_yDistCorrection(Obraz,Fidx,Fidy);
			//dx = x - (Fidx - fT_xDistCorrection(Obraz,Fidx,Fidy));
			//dy = y - (Fidy - fT_yDistCorrection(Obraz,Fidx,Fidy));
			//cout << P[i].Name << j <<" dx: "<< dx <<" dy: "<<dy<<endl;
		}
		
		//Distx = fT_xDistCorrection(Obraz,Fidx,Fidy);
		//Disty = fT_yDistCorrection(Obraz,Fidx,Fidy);

		//cout<<P[i].Name<<" distx: "<< Distx << " disty: "<< Disty <<endl;		
		P[i].X = Obraz->Interior[1] + Fidx;
		P[i].Y = Obraz->Interior[2] + Fidy;
		P[i].Name = T[i].Name;
	}
}



void fT_ground2IdealImage(TerrainPoint* T, Image* Obraz, int N, ImagePoint *P)
{
	double Lx, Ly, M;
	for (int i=0; i<N; i++)
	{
		Lx = Obraz->R[0]*(T[i].X - Obraz->CoordsAdj[0]) + Obraz->R[3]*(T[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[6]*(T[i].Z - Obraz->CoordsAdj[2]);
		Ly = Obraz->R[1]*(T[i].X - Obraz->CoordsAdj[0]) + Obraz->R[4]*(T[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[7]*(T[i].Z - Obraz->CoordsAdj[2]);
		M =  Obraz->R[2]*(T[i].X - Obraz->CoordsAdj[0]) + Obraz->R[5]*(T[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[8]*(T[i].Z - Obraz->CoordsAdj[2]);
		P[i].X = Obraz->Interior[1]-Obraz->Interior[0]*Lx/M;
		P[i].Y = Obraz->Interior[2]-Obraz->Interior[0]*Ly/M;
		P[i].Name = T[i].Name;
	}
}

void fT_ground2IdealImage(vector <TerrainPoint>* T, Image* Obraz, vector <ImagePoint> *P)
{
	double Lx, Ly, M;
	for (int i=0; i<T->size(); i++)
	{
		Lx = Obraz->R[0]*((*T)[i].X - Obraz->CoordsAdj[0]) + Obraz->R[3]*((*T)[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[6]*((*T)[i].Z - Obraz->CoordsAdj[2]);
		Ly = Obraz->R[1]*((*T)[i].X - Obraz->CoordsAdj[0]) + Obraz->R[4]*((*T)[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[7]*((*T)[i].Z - Obraz->CoordsAdj[2]);
		M = Obraz->R[2]*((*T)[i].X - Obraz->CoordsAdj[0]) + Obraz->R[5]*((*T)[i].Y - Obraz->CoordsAdj[1]) + Obraz->R[8]*((*T)[i].Z - Obraz->CoordsAdj[2]);
		ImagePoint Pt;
		Pt.X = Obraz->Interior[1]-Obraz->Interior[0]*Lx/M;
		Pt.Y = Obraz->Interior[2]-Obraz->Interior[0]*Ly/M;
		Pt.Name = (*T)[i].Name;
		P->push_back(Pt);
	}
}

void fT_ground2IdealImage(double **T, Image* Obraz, int N, ImagePoint *P, bool PCZero)
{
	double Lx, Ly, M;
	if (PCZero == false)
	{
		for (int i=0; i<N; i++)
		{
			Lx = Obraz->R[0]*(T[i][0] - Obraz->CoordsAdj[0]) + Obraz->R[3]*(T[i][1] - Obraz->CoordsAdj[1]) + Obraz->R[6]*(T[i][2] - Obraz->CoordsAdj[2]);
			Ly = Obraz->R[1]*(T[i][0] - Obraz->CoordsAdj[0]) + Obraz->R[4]*(T[i][1] - Obraz->CoordsAdj[1]) + Obraz->R[7]*(T[i][2] - Obraz->CoordsAdj[2]);
			M = Obraz->R[2]*(T[i][0] - Obraz->CoordsAdj[0]) + Obraz->R[5]*(T[i][1] - Obraz->CoordsAdj[1]) + Obraz->R[8]*(T[i][2] - Obraz->CoordsAdj[2]);
			P[i].X = Obraz->Interior[1]-Obraz->Interior[0]*Lx/M;
			P[i].Y = Obraz->Interior[2]-Obraz->Interior[0]*Ly/M;
		}
	}
	else
	{
		for (int i=0; i<N; i++)
		{
			Lx = Obraz->R[0]*T[i][0] + Obraz->R[3]*T[i][1] + Obraz->R[6]*T[i][2];
			Ly = Obraz->R[1]*T[i][0] + Obraz->R[4]*T[i][1] + Obraz->R[7]*T[i][2];
			M = Obraz->R[2]*T[i][0] + Obraz->R[5]*T[i][1] + Obraz->R[8]*T[i][2];
			P[i].X = Obraz->Interior[1]-Obraz->Interior[0]*Lx/M;
			P[i].Y = Obraz->Interior[2]-Obraz->Interior[0]*Ly/M;
		}
	}

}

void fT_ground2IdealImage(double *T , Image* Obraz, ImagePoint *P, bool PCZero)
{
	double Lx, Ly, M;
	if (PCZero == false)
	{
		Lx = Obraz->R[0]*(T[0] - Obraz->CoordsAdj[0]) + Obraz->R[3]*(T[1] - Obraz->CoordsAdj[1]) + Obraz->R[6]*(T[2] - Obraz->CoordsAdj[2]);
		Ly = Obraz->R[1]*(T[0] - Obraz->CoordsAdj[0]) + Obraz->R[4]*(T[1] - Obraz->CoordsAdj[1]) + Obraz->R[7]*(T[2] - Obraz->CoordsAdj[2]);
		M = Obraz->R[2]*(T[0] - Obraz->CoordsAdj[0]) + Obraz->R[5]*(T[1] - Obraz->CoordsAdj[1]) + Obraz->R[8]*(T[2] - Obraz->CoordsAdj[2]);
		P->X = Obraz->Interior[1]-Obraz->Interior[0]*Lx/M;
		P->Y = Obraz->Interior[2]-Obraz->Interior[0]*Ly/M;
	}
	else
	{
		Lx = Obraz->R[0]*T[0] + Obraz->R[3]*T[1] + Obraz->R[6]*T[2];
		Ly = Obraz->R[1]*T[0] + Obraz->R[4]*T[1] + Obraz->R[7]*T[2];
		M = Obraz->R[2]*T[0] + Obraz->R[5]*T[1] + Obraz->R[8]*T[2];
		P->X = Obraz->Interior[1]-Obraz->Interior[0]*Lx/M;
		P->Y = Obraz->Interior[2]-Obraz->Interior[0]*Ly/M;
	}
}

void fT_ground2IdealImage(double *T , Image* Obraz, double *P, bool PCZero)
{
	double Lx, Ly, M;
	if (PCZero == false)
	{
		Lx = Obraz->R[0]*(T[0] - Obraz->CoordsAdj[0]) + Obraz->R[3]*(T[1] - Obraz->CoordsAdj[1]) + Obraz->R[6]*(T[2] - Obraz->CoordsAdj[2]);
		Ly = Obraz->R[1]*(T[0] - Obraz->CoordsAdj[0]) + Obraz->R[4]*(T[1] - Obraz->CoordsAdj[1]) + Obraz->R[7]*(T[2] - Obraz->CoordsAdj[2]);
		M = Obraz->R[2]*(T[0] - Obraz->CoordsAdj[0]) + Obraz->R[5]*(T[1] - Obraz->CoordsAdj[1]) + Obraz->R[8]*(T[2] - Obraz->CoordsAdj[2]);
		P[0] = Obraz->Interior[1]-Obraz->Interior[0]*Lx/M;
		P[1] = Obraz->Interior[2]-Obraz->Interior[0]*Ly/M;
	}
	else
	{
		Lx = Obraz->R[0]*T[0] + Obraz->R[3]*T[1] + Obraz->R[6]*T[2];
		Ly = Obraz->R[1]*T[0] + Obraz->R[4]*T[1] + Obraz->R[7]*T[2];
		M = Obraz->R[2]*T[0] + Obraz->R[5]*T[1] + Obraz->R[8]*T[2];
		P[0] = Obraz->Interior[1]-Obraz->Interior[0]*Lx/M;
		P[1] = Obraz->Interior[2]-Obraz->Interior[0]*Ly/M;
	}
}

int fT_countByType(ImagePoint *P,int N, int T)
{
	//Funkcja liczy ile jest punktow danego typu T
	int Ile = 0;
	for (int i=0; i<N; i++)
	{
		if ( P[i].Type == T) Ile ++;
	}
	return Ile;
}

int fT_countByType(TerrainPoint *T, int N, int Typ)
{
	int Ile = 0;
	for (int i=0; i<N; i++)
	{
		if ( T[i].Type == Typ) Ile ++;
	}
	return Ile;
}

void fT_selectPointByType(ImagePoint *Input, int N, int Type, ImagePoint *Output)
{
	//Funkcja sluzy do wyselekcjonowania punktow typu Type z tablicy punktow Input
	//Punkty zapisywane sa do tablicy Output
	//N - ilosc punktow w tablicy Input
	int k = 0;
	for (int i=0; i<N; i++)
	{
		if (Input[i].Type == Type)
		{
			Output[k++] = Input[i];
		}
	}
}

void fT_selectPointByType(TerrainPoint *Input, int N, int Type, TerrainPoint *Output)
{
	//Funkcja sluzy do wyselekcjonowania punktow typu Type z tablicy punktow Input
	//Punkty zapisywane sa do tablicy Output
	//N - ilosc punktow w tablicy Input
	int k = 0;
	for (int i=0; i<N; i++)
	{
		if (Input[i].Type == Type)
		{
			Output[k++] = Input[i];
		}
	}
}

bool fT_isImPointInArray(ImagePoint *Array, int N, string Name)
{
	bool Result = false;
	for (int i=0; i<N; i++)
	{
		if (Array[i].Name == Name)
		{
			Result = true;
			break;
		}
	}
	return Result;
}

short ft_solveQuadraticEquation(double *Coef, double* Results)
{
	//Funkcja rozwiazuje rownanie kwadratowe
	//Coef - wskaznik do wspolczynnikow rownania kwadratowego
	//Results - wskaznik do wynikow - tablica 2-elementowa
	//Funkcja zwraca ilosc pierwaistkow rownania.
	//Jezeli pierwiastek rownania jest podwojny to zapisywany jest 
	//w obu polach tablicy Results
	double Delta = pow(Coef[1],2) - 4*Coef[0]*Coef[2];
	if (Delta < 0)
	{
		Results[0] = 0;
		Results[1] = 0;
		return 0;
	}
	if (Delta == 0)
	{
		Results[0] = -Coef[1]/(2*Coef[0]);
		Results[1] = Results[0];
		return 1;
	}
	if (Delta > 0)
	{
		Results[0] = (-Coef[1]-sqrt(Delta))/(2*Coef[0]);
		Results[1] = (-Coef[1]+sqrt(Delta))/(2*Coef[0]);
		return 2;
	}
}

short ft_solveQuatricsReal(long double *Coef, long double *Results)
{
	//Funkcja rozwiazuje rownanie czwartego stopnia
	//Coef - wskaznik do wspolczynnikow rownania kwadratowego
	//Results - wskaznik do wynikow - tablica 4-elementowa
	//Funkcja zwraca ilosc pierwaistkow rownania.
	//Jezeli pierwiastek rownania jest liczba zespolona to funkcja wpisuje wartosc zero
	complex<long double> a ( Coef[0],0);
	complex<long double> b ( Coef[1],0);
	complex<long double> c ( Coef[2],0);
	complex<long double> d ( Coef[3],0);
	complex<long double> e ( Coef[4],0);

	complex<long double> c4(4.0,0);
	complex<long double> c05(0.5,0);
	complex<long double> c3(3.0,0);
	complex<long double> c2(2.0,0);
	complex<long double> c1(1.0,0);
	complex<long double> c8(8.0,0);
	complex<long double> c9(9.0,0);
	complex<long double> c12(12.0,0);
	complex<long double> c27(27.0,0);
	complex<long double> c72(72.0,0);
	complex<long double> cf(pow(2.0,1/3.0),0.0); //Pierwiastek trzeciego stopnia z 2

	complex<long double> A = cf * (pow(c,2.0) - c3*b*d + c12*a*e);
	complex<long double> B = c2*pow(c,3) - c9*b*c*d + c27*a*pow(d,2) + c27*pow(b,2)*e - c72*a*c*e;
	complex<long double> C = -c4*pow((pow(c,2.0) - c3*b*d + c12*a*e),3.0);
	complex<long double> D = -pow(b,3)/pow(a,3.0) + c4*b*c/pow(a,2) - c8*d/a;
	complex<long double> E = pow(b,2)/(c4*pow(a,2)) - c2*c/(c3*a);
	complex<long double> F = pow(b,2)/(c2*pow(a,2)) - c4*c/(c3*a);
	
	complex<long double> x1; 
	complex<long double> x2;
	complex<long double> x3;
	complex<long double> x4;
	
	long double g = 0.33333333333333333333333333333333333333333333333333;
	complex<long double> U1 = -b/(c4*a);
	complex<long double> U2 = E + A/(c3*a*pow(B + sqrt(C + pow(B,2.0)),g) );
	complex<long double> U3 = c1/(c3*cf*a)*pow(B + sqrt(C + pow(B,2.0)),g );
	complex<long double> U4 = F - A/(c3*a*pow(B + sqrt(C + pow(B,2.0)),g));
	complex<long double> U5 = A/(c3*a*pow((B + sqrt(C + pow(B,2.0))),g));

	complex<long double> x[4];

	x[0] = U1 - c05*sqrt(U2 + U3) - c05*sqrt(U4 - U3 - D/(c4*sqrt(E + U5 + U3)));
	x[1] = U1 - c05*sqrt(U2 + U3) + c05*sqrt(U4 - U3 - D/(c4*sqrt(E + U5 + U3)));
	x[2] = U1 + c05*sqrt(U2 + U3) - c05*sqrt(U4 - U3 + D/(c4*sqrt(E + U5 + U3)));
	x[3] = U1 + c05*sqrt(U2 + U3) + c05*sqrt(U4 - U3 + D/(c4*sqrt(E + U5 + U3)));

	short k=0;
	for (int i=0; i<4; i++)
	{
		if (fabs(x[i].imag()) < 1.0e-10)
		{
			Results[i] = x[i].real();
			k++;
		}
		else
		{	
			Results[i] = 0;
		}
	}
	return k;
}

long double ft_findV(long double *V1, long double *V2)
{
	//Funkcja uzywana przy obliczeniu wciecia wstecz wg Killiana
	//Ze zbioru dwoch rozwiazn wielomianu 4 stopnia wybierane
	//sa dwani niezerowe rozwiazania, ktore sa nastepnie usredniane
	double d=1000;
	int iV1 = 0;
	int iV2 = 0;
	for (int i=0; i<4; i++)
	{
		if (V1[i] > 0)
		{
			for (int j=0; j<4; j++)
			{
				if (V2[j] > 0)
				{
					if  (fabs(V1[i]-V2[j]) < d)
					{
						d = fabs(V1[i]-V2[j]);
						iV1 = i;
						iV2 = j;
					}

				}
			}
		}
	}
	long double v = 0.5*(V1[iV1] + V2[iV2]);
	return v;
}

double ft_findV(double *V1, double *V2)
{
	//Funkcja uzywana przy obliczeniu wciecia wstecz wg Killiana
	//Ze zbioru dwoch rozwiazn wielomianu 4 stopnia wybierane
	//sa dwani niezerowe rozwiazania, ktore sa nastepnie usredniane
	double d=1000;
	int iV1 = 0;
	int iV2 = 0;
	for (int i=0; i<4; i++)
	{
		if (V1[i] > 0)
		{
			for (int j=0; j<4; j++)
			{
				if (V2[j] > 0)
				{
					if  (fabs(V1[i]-V2[j]) < d)
					{
						d = fabs(V1[i]-V2[j]);
						iV1 = i;
						iV2 = j;
					}

				}
			}
		}
	}
	double v = 0.5*(V1[iV1] + V2[iV2]);
	return v;
}



void fT_findRelative(double *RLeft, double *RRight, double *CoordsL, double *CoordsR, CvMat *R, CvMat *B)
{
	//Funkcja oblicza elementy orientacji wzajemnej modelu na podstawie danych elementów orientacji
	//zewnetrznej zdjec
	// RLeft - macierz obrotu zdjecia lewego
	// RRight - macierz obrotu zdjecia prawego
	// CoordsL - wspolrzedne srodka rzutu zdjecia lewego
	// CoordsR - wspolrzedne srodka rzutu zdjecia prawego
	// R - macierz obrotu zdjecia prawego w ukladzie zdjecia lewego
	// B - wektor bazy w ukladzie modelu

	CvMat * RL; //Macierz obrotu zdjecia lewego
	RL = cvCreateMat(3,3,CV_32FC1);

	CvMat * RP; //Macierz obrotu zdjecia prawego
	RP = cvCreateMat(3,3,CV_32FC1);

	CvMat *Baza; // Wektor bazy w ukladzie terenowym
	Baza = cvCreateMat(3,1,CV_32FC1);

	//Ustawiamy macierz lewa
	cvSetReal2D(RL,0,0, RLeft[0] );
	cvSetReal2D(RL,0,1, RLeft[1] );
	cvSetReal2D(RL,0,2, RLeft[2]);
	
	cvSetReal2D(RL,1,0, RLeft[3] );
	cvSetReal2D(RL,1,1, RLeft[4] );
	cvSetReal2D(RL,1,2, RLeft[5] );

	cvSetReal2D(RL,2,0, RLeft[6] );
	cvSetReal2D(RL,2,1, RLeft[7] );
	cvSetReal2D(RL,2,2, RLeft[8] );

	//Ustawiamy macierz prawa
	cvSetReal2D(RP,0,0, RRight[0] );
	cvSetReal2D(RP,0,1, RRight[1] );
	cvSetReal2D(RP,0,2, RRight[2]);
	
	cvSetReal2D(RP,1,0, RRight[3] );
	cvSetReal2D(RP,1,1, RRight[4] );
	cvSetReal2D(RP,1,2, RRight[5] );

	cvSetReal2D(RP,2,0, RRight[6] );
	cvSetReal2D(RP,2,1, RRight[7] );
	cvSetReal2D(RP,2,2, RRight[8] );

	cvGEMM (RL,RP,1,NULL,0,R,CV_GEMM_A_T);

	cvSetReal2D(Baza,0,0, CoordsR[0] - CoordsL[0]);
	cvSetReal2D(Baza,1,0, CoordsR[1] - CoordsL[1]);
	cvSetReal2D(Baza,2,0, CoordsR[2] - CoordsL[2]);

	cvGEMM(RL,Baza,1,NULL,0,B,CV_GEMM_A_T);
}



void fT_multiImageIntersection(vector<TerrainPoint>& P, vector <vector<double> >& R, vector <float>& cks, vector<cv::Point2f> &Measurements, TerrainPoint &Result)
{
	//cout << "W funkcji Intersection:" << endl;
	//Pozyskanie informacji o wymiarach:
	int n = Measurements.size();
	//cout << "Wymiary: " << P.size() << " " << R.size() << " " << cks.size() << " " << Measurements.size() << endl;
	//alokacja i wypelnianie macierzy A
	double* A = new double[2 * n * 3];
	for (int i = 0; i < n; i++) //2 bo mamy wsp x oraz y
	{
		A[6 * i + 0] = Measurements.at(i).x * R.at(i)[2] + cks[i] * R.at(i)[0]; //wspolczynnik przy X
		A[6 * i + 1] = Measurements.at(i).x * R.at(i)[5] + cks[i] * R.at(i)[3]; //wspolczynnik przy Y
		A[6 * i + 2] = Measurements.at(i).x * R.at(i)[8] + cks[i] * R.at(i)[6]; //wspolczynnik przy Z
		A[6 * i + 3] = Measurements.at(i).y * R.at(i)[2] + cks[i] * R.at(i)[1]; //wspolczynnik przy X
		A[6 * i + 4] = Measurements.at(i).y * R.at(i)[5] + cks[i] * R.at(i)[4]; //wspolczynnik przy Y
		A[6 * i + 5] = Measurements.at(i).y * R.at(i)[8] + cks[i] * R.at(i)[7]; //wspolczynnik przy Z
	}
	//cout << "Utworzono macierz A" << endl;
	//for (int i = 0; i < 2 * n * 3; i++) cout << A[i] << " ";
	//cout << endl;
	//alokacja i wypelnianie macierzy L
	double* L = new double[2 * n];
	for (int i = 0; i < n; i++)
	{
		L[2 * i] = (Measurements.at(i).x*R.at(i)[2] + cks[i] * R.at(i)[0])*P.at(i).X +
			(Measurements.at(i).x*R.at(i)[5] + cks[i] * R.at(i)[3])*P.at(i).Y +
			(Measurements.at(i).x*R.at(i)[8] + cks[i] * R.at(i)[6])*P.at(i).Z;
		L[2 * i + 1] = (Measurements.at(i).y*R.at(i)[2] + cks[i] * R.at(i)[1])*P.at(i).X +
			(Measurements.at(i).y*R.at(i)[5] + cks[i] * R.at(i)[4])*P.at(i).Y +
			(Measurements.at(i).y*R.at(i)[8] + cks[i] * R.at(i)[7])*P.at(i).Z;
	}
	//alokacja macierzy posrednich
	double *AT = new double[2 * n * 3];
	double ATA[9];
	double INV_ATA[9];
	double ATL[3];
	//informacja o wymiarach
	int sA[2]; sA[0] = 2 * n; sA[1] = 3;
	int sAT[2]; sAT[0] = 3; sAT[1] = 2 * n;
	int sL[2]; sL[0] = 2 * n; sL[1] = 1;
	//obliczenia
	//cout << "A:" << endl;
	//fT_dispMatrix(A, sA);
	//cout << "L: " << endl;
	//fT_dispMatrix(L, sL);
	fT_matTranspose(A, sA, AT);
	fT_matMult(AT, A, sAT, sA, ATA);
	fT_matInv3(ATA, INV_ATA);
	//cout << "INV_ATA:" << endl;
	//fT_dispMatrix3(INV_ATA);
	fT_matMult(AT, L, sAT, sL, ATL);
	double Coords[3];
	fT_vectorTrans3(INV_ATA, ATL, Coords);
	Result.X = Coords[0];
	Result.Y = Coords[1];
	Result.Z = Coords[2];
	//zwalnianie pamieci
	delete[] A;
	delete[] L;
	delete[] AT;
	A = 0;
	L = 0;
	AT = 0;
}

double fT_fastModelInteresection(double *ImVecL, double *ImVecR, double *r, double* b, double *Result, double *V)
{
	//Funkcja oblicza wspolrzedne punktu w ukladzie modelu na podstawie pomiaru
	//Zakladamy ze r1 = I oraz X01 = Y01 = Z01 = 0
	//ImVecL, ImVecR wektory wsporzednych pomierzonych na zdjeciach lewym i prawym
	//Maja postac [x y ck]. ZAKLADAMY ZE WSP SA WOLNE OD DYSTORSJI !!!!
	//r - macierz obrotu zdjecia prawego w ukladzie tlowym zdjecia lewego 9-elementow
	//b - wektor bazy w ukladzie zdjecia lewego 3-elementy
	//Result - wynik - trzy wsplrzedne
	//V - tablica 4x1 - poprawki do wspolrzednych obrazowych w kolejnosci [vxl vyl vxp vyp]
	double A[12]; //Porzadek - najpierw kolumny, potem wiersze
	double L[4];
	double ATA[9];
	double DET;
	double Mi[9];
	double ATL[3];
	
	double ckL = ImVecL[2];
	double ckR = ImVecR[2];

	for (int i=0; i<3; i++)
	{
		//Dzielimy przez ck dla lepszego uwarunkowania
		ImVecL[i] = ImVecL[i]/ImVecL[2];
		ImVecR[i] = ImVecR[i]/ImVecR[2];
	}

	//Macierz Wspolczynnikow
	//Wspolczynniki dla zdjecia lewego
	A[0] = ImVecL[2];
	A[1] = 0;
	A[2] = ImVecL[0];
	A[3] = 0;
	A[4] = ImVecL[2];
	A[5] = ImVecL[1];
	//Wspolczynniki dla zdjecia prawego
	A[6] = ImVecR[0]*r[2] + ImVecR[2]*r[0];
	A[7] = ImVecR[0]*r[5] + ImVecR[2]*r[3];
	A[8] = ImVecR[0]*r[8] + ImVecR[2]*r[6];
	A[9] = ImVecR[1]*r[2] + ImVecR[2]*r[1];
	A[10] = ImVecR[1]*r[5] + ImVecR[2]*r[4];
	A[11] = ImVecR[1]*r[8] + ImVecR[2]*r[7];

	//Macierz wyrazow wolnych
	L[0] = 0;
	L[1] = 0;
	L[2] = ImVecR[0]*r[2]*b[0] + ImVecR[0]*r[5]*b[1] + ImVecR[0]*r[8]*b[2] + ImVecR[2]*r[0]*b[0] + ImVecR[2]*r[3]*b[1] + ImVecR[2]*r[6]*b[2];
	L[3] = ImVecR[1]*r[2]*b[0] + ImVecR[1]*r[5]*b[1] + ImVecR[1]*r[8]*b[2] + ImVecR[2]*r[1]*b[0] + ImVecR[2]*r[4]*b[1] + ImVecR[2]*r[7]*b[2];

	//Macierz ulkadu rownan normalnych
	ATA[0] = A[0]*A[0] + A[3]*A[3] + A[6]*A[6] + A[9]*A[9];
	ATA[1] = A[0]*A[1] + A[3]*A[4] + A[6]*A[7] + A[9]*A[10];
	ATA[2] = A[0]*A[2] + A[3]*A[5] + A[6]*A[8] + A[9]*A[11];


	ATA[3] = ATA[1];
	ATA[4] = A[1]*A[1] + A[4]*A[4] + A[7]*A[7] + A[10]*A[10];
	ATA[5] = A[1]*A[2] + A[4]*A[5] + A[7]*A[8] + A[10]*A[11];

	ATA[6] = ATA[2];
	ATA[7] = ATA[5];
	ATA[8] = A[2]*A[2] + A[5]*A[5] + A[8]*A[8] + A[11]*A[11];

	DET = ATA[0]*ATA[4]*ATA[8] + ATA[1]*ATA[5]*ATA[6] + ATA[2]*ATA[3]*ATA[7] - ATA[2]*ATA[4]*ATA[6] - ATA[1]*ATA[3]*ATA[8] - ATA[0]*ATA[5]*ATA[7];
	//cout<<"DET: "<<DET<<endl;
	if (DET < 1E-12)
	{
		Result[0] = 0;
		Result[1] = 0;
		Result[2] = 0;
		V[0] = 1000;
		V[1] = 1000;
		V[2] = 1000;
		V[3] = 1000;
		return DET;
	}
	else
	{
		Mi[0] = ATA[4]*ATA[8]-ATA[5]*ATA[7];
		Mi[1] = ATA[3]*ATA[8]-ATA[5]*ATA[6];
		Mi[2] = ATA[3]*ATA[7]-ATA[4]*ATA[6];
		
		Mi[3] = ATA[1]*ATA[8]-ATA[2]*ATA[7];
		Mi[4] = ATA[0]*ATA[8]-ATA[2]*ATA[6];
		Mi[5] = ATA[0]*ATA[7]-ATA[1]*ATA[6];
		
		Mi[6] = ATA[1]*ATA[5]-ATA[2]*ATA[4];
		Mi[7] = ATA[0]*ATA[5]-ATA[2]*ATA[3];
		Mi[8] = ATA[0]*ATA[4]-ATA[1]*ATA[3];
		
		//Obliczamy odwrotnosc
		ATA[0] = Mi[0]/DET;
		ATA[1] = -Mi[1]/DET;
		ATA[2] = Mi[2]/DET;

		ATA[3] = -Mi[3]/DET;
		ATA[4] = Mi[4]/DET;
		ATA[5] = -Mi[5]/DET;

		ATA[6] = Mi[6]/DET;
		ATA[7] = -Mi[7]/DET;
		ATA[8] = Mi[8]/DET;

		//Obliczamy macierz ATL
		ATL[0] = A[0]*L[0] + A[3]*L[1] +A[6]*L[2] + A[9]*L[3];
		ATL[1] = A[1]*L[0] + A[4]*L[1] +A[7]*L[2] + A[10]*L[3];
		ATL[2] = A[2]*L[0] + A[5]*L[1] +A[8]*L[2] + A[11]*L[3];

		//Obliczamy wynik inv(ATA)*ATL
		fT_vectorTrans3(ATA, ATL, Result);
		
		//Obliczamy poprawki
		double Licz2x = r[0]*(Result[0]-b[0]) + r[3]*(Result[1]-b[1]) + r[6]*(Result[2]-b[2]);
		double Licz2y = r[1]*(Result[0]-b[0]) + r[4]*(Result[1]-b[1]) + r[7]*(Result[2]-b[2]);
		double Mian2 = r[2]*(Result[0]-b[0]) + r[5]*(Result[1]-b[1]) + r[8]*(Result[2]-b[2]);

		for (int i=0; i<3; i++)
		{
			//Znow mnozymy przez ck
			ImVecL[i] = ImVecL[i]*ckL;
			ImVecR[i] = ImVecR[i]*ckR;
		}

		V[0] = ImVecL[0] + ImVecL[2]*Result[0]/Result[2];
		V[1] = ImVecL[1] + ImVecL[2]*Result[1]/Result[2];
		V[2] = ImVecR[0] + ImVecR[2]*Licz2x/Mian2;
		V[3] = ImVecR[1] + ImVecR[2]*Licz2y/Mian2;

		return DET;

	}
}

double fT_fastModelInteresection(double *ImVecL, double *ImVecR, vector<double> *r, vector<double>* b, double *Result, double *V)
{
	//Funkcja oblicza wspolrzedne punktu w ukladzie modelu na podstawie pomiaru
	//Implementacja przeladowana dla r i b jako wektorow
	//Zakladamy ze r1 = I oraz X01 = Y01 = Z01 = 0
	//ImVecL, ImVecR wektory wsporzednych pomierzonych na zdjeciach lewym i prawym
	//Maja postac [x y ck]. ZAKLADAMY ZE WSP SA WOLNE OD DYSTORSJI !!!!
	//r - macierz obrotu zdjecia prawego w ukladzie tlowym zdjecia lewego 9-elementow
	//b - wektor bazy w ukladzie zdjecia lewego 3-elementy
	//Result - wynik - trzy wsplrzedne
	//V - tablica 4x1 - poprawki do wspolrzednych obrazowych w kolejnosci [vxl vyl vxp vyp]
	double A[12]; //Porzadek - najpierw kolumny, potem wiersze
	double L[4];
	double ATA[9];
	double DET;
	double Mi[9];
	double ATL[3];
	
	double ckL = ImVecL[2];
	double ckR = ImVecR[2];

	for (int i=0; i<3; i++)
	{
		//Dzielimy przez ck dla lepszego uwarunkowania
		ImVecL[i] = ImVecL[i]/ImVecL[2];
		ImVecR[i] = ImVecR[i]/ImVecR[2];
	}

	//Macierz Wspolczynnikow
	//Wspolczynniki dla zdjecia lewego
	A[0] = ImVecL[2];
	A[1] = 0;
	A[2] = ImVecL[0];
	A[3] = 0;
	A[4] = ImVecL[2];
	A[5] = ImVecL[1];
	//Wspolczynniki dla zdjecia prawego
	A[6] = ImVecR[0]*r->at(2) + ImVecR[2]*r->at(0);
	A[7] = ImVecR[0]*r->at(5) + ImVecR[2]*r->at(3);
	A[8] = ImVecR[0]*r->at(8) + ImVecR[2]*r->at(6);
	A[9] = ImVecR[1]*r->at(2) + ImVecR[2]*r->at(1);
	A[10] = ImVecR[1]*r->at(5) + ImVecR[2]*r->at(4);
	A[11] = ImVecR[1]*r->at(8) + ImVecR[2]*r->at(7);

	//Macierz wyrazow wolnych
	L[0] = 0;
	L[1] = 0;
	L[2] = ImVecR[0]*r->at(2)*b->at(0) + ImVecR[0]*r->at(5)*b->at(1) + ImVecR[0]*r->at(8)*b->at(2) + ImVecR[2]*r->at(0)*b->at(0) + ImVecR[2]*r->at(3)*b->at(1) + ImVecR[2]*r->at(6)*b->at(2);
	L[3] = ImVecR[1]*r->at(2)*b->at(0) + ImVecR[1]*r->at(5)*b->at(1) + ImVecR[1]*r->at(8)*b->at(2) + ImVecR[2]*r->at(1)*b->at(0) + ImVecR[2]*r->at(4)*b->at(1) + ImVecR[2]*r->at(7)*b->at(2);

	//Macierz ulkadu rownan normalnych
	ATA[0] = A[0]*A[0] + A[3]*A[3] + A[6]*A[6] + A[9]*A[9];
	ATA[1] = A[0]*A[1] + A[3]*A[4] + A[6]*A[7] + A[9]*A[10];
	ATA[2] = A[0]*A[2] + A[3]*A[5] + A[6]*A[8] + A[9]*A[11];


	ATA[3] = ATA[1];
	ATA[4] = A[1]*A[1] + A[4]*A[4] + A[7]*A[7] + A[10]*A[10];
	ATA[5] = A[1]*A[2] + A[4]*A[5] + A[7]*A[8] + A[10]*A[11];

	ATA[6] = ATA[2];
	ATA[7] = ATA[5];
	ATA[8] = A[2]*A[2] + A[5]*A[5] + A[8]*A[8] + A[11]*A[11];

	DET = ATA[0]*ATA[4]*ATA[8] + ATA[1]*ATA[5]*ATA[6] + ATA[2]*ATA[3]*ATA[7] - ATA[2]*ATA[4]*ATA[6] - ATA[1]*ATA[3]*ATA[8] - ATA[0]*ATA[5]*ATA[7];
	//cout<<"DET: "<<DET<<endl;
	if (DET < 1E-12)
	{
		Result[0] = 0;
		Result[1] = 0;
		Result[2] = 0;
		V[0] = 1000;
		V[1] = 1000;
		V[2] = 1000;
		V[3] = 1000;
		return DET;
	}
	else
	{
		Mi[0] = ATA[4]*ATA[8]-ATA[5]*ATA[7];
		Mi[1] = ATA[3]*ATA[8]-ATA[5]*ATA[6];
		Mi[2] = ATA[3]*ATA[7]-ATA[4]*ATA[6];
		
		Mi[3] = ATA[1]*ATA[8]-ATA[2]*ATA[7];
		Mi[4] = ATA[0]*ATA[8]-ATA[2]*ATA[6];
		Mi[5] = ATA[0]*ATA[7]-ATA[1]*ATA[6];
		
		Mi[6] = ATA[1]*ATA[5]-ATA[2]*ATA[4];
		Mi[7] = ATA[0]*ATA[5]-ATA[2]*ATA[3];
		Mi[8] = ATA[0]*ATA[4]-ATA[1]*ATA[3];
		
		//Obliczamy odwrotnosc
		ATA[0] = Mi[0]/DET;
		ATA[1] = -Mi[1]/DET;
		ATA[2] = Mi[2]/DET;

		ATA[3] = -Mi[3]/DET;
		ATA[4] = Mi[4]/DET;
		ATA[5] = -Mi[5]/DET;

		ATA[6] = Mi[6]/DET;
		ATA[7] = -Mi[7]/DET;
		ATA[8] = Mi[8]/DET;

		//Obliczamy macierz ATL
		ATL[0] = A[0]*L[0] + A[3]*L[1] +A[6]*L[2] + A[9]*L[3];
		ATL[1] = A[1]*L[0] + A[4]*L[1] +A[7]*L[2] + A[10]*L[3];
		ATL[2] = A[2]*L[0] + A[5]*L[1] +A[8]*L[2] + A[11]*L[3];

		//Obliczamy wynik inv(AT)*ATL
		fT_vectorTrans3(ATA, ATL, Result);
		
		//Obliczamy poprawki
		double Licz2x = r->at(0)*(Result[0]-b->at(0)) + r->at(3)*(Result[1]-b->at(1)) + r->at(6)*(Result[2]-b->at(2));
		double Licz2y = r->at(1)*(Result[0]-b->at(0)) + r->at(4)*(Result[1]-b->at(1)) + r->at(7)*(Result[2]-b->at(2));
		double Mian2 = r->at(2)*(Result[0]-b->at(0)) + r->at(5)*(Result[1]-b->at(1)) + r->at(8)*(Result[2]-b->at(2));

		for (int i=0; i<3; i++)
		{
			//Znow mnozymy przez ck
			ImVecL[i] = ImVecL[i]*ckL;
			ImVecR[i] = ImVecR[i]*ckR;
		}

		V[0] = ImVecL[0] + ImVecL[2]*Result[0]/Result[2];
		V[1] = ImVecL[1] + ImVecL[2]*Result[1]/Result[2];
		V[2] = ImVecR[0] + ImVecR[2]*Licz2x/Mian2;
		V[3] = ImVecR[1] + ImVecR[2]*Licz2y/Mian2;
		
		return DET;

	}
}

bool fT_testRotationMatrix(double *r, double DIFF)
{
	//Funkcja sprawdza czy macierz obrotu jest macirza obrotu
	//Sprawdza czy jest to macierz ortogonalna
	
	//Sprawdzamy ortogonalnosc (iloczyn wektorowy)

	if ( fabs(r[3]*r[7] - r[6]*r[4] - r[2]) > DIFF)
	{
		return false;
	}
	else
	{
		if ( fabs(r[6]*r[1] - r[0]*r[7] - r[5]) > DIFF)
		{
			return false;
		}
		else
		{
				if ( fabs(r[0]*r[4] - r[3]*r[1] - r[8]) > DIFF)
				{
					return false;
				}
				else
				{
					return true;
				}
		}
	}
	
}

void fT_calcNonorthonormality(double *R, double *Test)
{
	//nonnormality:
	Test[0] = R[0]*R[0] + R[3]*R[3] + R[6]*R[6] - 1;
	Test[1] = R[1]*R[1] + R[4]*R[4] + R[7]*R[7] - 1;
	Test[2] = R[2]*R[2] + R[5]*R[5] + R[8]*R[8] - 1;
	//nonorthogonality:
	Test[3] = R[3]*R[7] - R[6]*R[4] - R[2];
	Test[4] = R[6]*R[1] - R[0]*R[7] - R[5];
	Test[5] = R[0]*R[4] - R[3]*R[1] - R[8];
}

void fT_orthonormalize(double *R, double *RN)
{
	CvMat * D;
	CvMat * B;
	CvMat * V;
	D = cvCreateMat(6,1,CV_32FC1);
	B = cvCreateMat(6,9,CV_32FC1);
	V = cvCreateMat(9,1,CV_32FC1);

	for (int i=0; i<6; i++)
	{
		for (int j=0; j<9; j++)
		{
			cvSetReal2D(B,i,j,0);
		}
	}

	CvMat * BBT = cvCreateMat(6,6,CV_32FC1);
	CvMat * BBT_INV = cvCreateMat(6,6,CV_32FC1);
	CvMat * BTBBT_INV = cvCreateMat(9,6,CV_32FC1);

	//kopiujemy zawartosc R do RN zeby potem moc liczyc w kilku iteracjach
	for (int i=0; i<9; i++)
	{
		RN[i] = R[i];
	}

	for (int iter = 1; iter<=5; iter++) //5 iteracji powinno wystarczyc az za nadto
	{
		cvSetReal2D(D,0,0,RN[0]*RN[0] + RN[3]*RN[3] + RN[6]*RN[6] - 1);
		cvSetReal2D(D,1,0,RN[1]*RN[1] + RN[4]*RN[4] + RN[7]*RN[7] - 1);
		cvSetReal2D(D,2,0,RN[2]*RN[2] + RN[5]*RN[5] + RN[8]*RN[8] - 1);
		cvSetReal2D(D,3,0,RN[3]*RN[7] - RN[6]*RN[4] - RN[2]);
		cvSetReal2D(D,4,0,RN[6]*RN[1] - RN[0]*RN[7] - RN[5]);
		cvSetReal2D(D,5,0,RN[0]*RN[4] - RN[3]*RN[1] - RN[8]);

		cvSetReal2D(B,0,0,2*RN[0]);
		cvSetReal2D(B,0,3,2*RN[3]);
		cvSetReal2D(B,0,6,2*RN[6]);

		cvSetReal2D(B,1,1,2*RN[1]);
		cvSetReal2D(B,1,4,2*RN[4]);
		cvSetReal2D(B,1,7,2*RN[7]);

		cvSetReal2D(B,2,2,2*RN[2]);
		cvSetReal2D(B,2,5,2*RN[5]);
		cvSetReal2D(B,2,8,2*RN[8]);

		cvSetReal2D(B,3,2,-1);
		cvSetReal2D(B,3,3,RN[7]);
		cvSetReal2D(B,3,4,-RN[6]);
		cvSetReal2D(B,3,6,-RN[4]);
		cvSetReal2D(B,3,7,RN[3]);

		cvSetReal2D(B,4,0,-RN[7]);
		cvSetReal2D(B,4,1,RN[6]);
		cvSetReal2D(B,4,5,-1);
		cvSetReal2D(B,4,6,RN[1]);
		cvSetReal2D(B,4,7,-RN[0]);

		cvSetReal2D(B,5,0,RN[4]);
		cvSetReal2D(B,5,1,-RN[3]);
		cvSetReal2D(B,5,3,-RN[1]);
		cvSetReal2D(B,5,4,RN[0]);
		cvSetReal2D(B,5,8,-1);

		cvGEMM(B,B,1,NULL,0,BBT,CV_GEMM_B_T);
		cvInvert(BBT,BBT_INV,CV_SVD);
		cvGEMM(B,BBT_INV,1,NULL,0,BTBBT_INV,CV_GEMM_A_T);
		cvGEMM(BTBBT_INV,D,-1,NULL,0,V,0);

		for(int i=0; i<9; i++)
		{
			RN[i] = RN[i] + cvGetReal2D(V,i,0);
		}

	}

	cvReleaseMat(&BTBBT_INV);
	cvReleaseMat(&BBT_INV);
	cvReleaseMat(&BBT);

	cvReleaseMat(&D);
	cvReleaseMat(&B);
	cvReleaseMat(&V);



}

bool fT_testCheriality(double *r, double *b, double* XL, double* YL, double* XP, double* YP, double ckL, double ckP, int N)
{
	//Funkcja sprawdza czy wyznaczany w drodze wciecia w przod punkt lezy przed zdjeciami czy za zdjeciami
	//Sprawdzana jest rowniez macierz obrotu - czy jest to macierz obrotu!
	//r - macierz obrotu zdjecia drugieg w ukladzie modelu (w ukladzie zdjecia pierwszego)
	//b - wektor bazy
	//XL,YL,XP,YP - wspolrzedne tlowe (po uwzglednieniu dystorsji!) na zdjeciach
	//ckL, ckP - wartosci stalych kamery
	
	//Po pierwsze testujemy macierz obrotu - sprawdzamy czy jest orotogonalna
	bool Cheriality = false;
	if ( fT_testRotationMatrix(r,1E-10) == true)
	{
		double ImL[3];
		double ImR[3];
		double ImRTrans[3];
		double Result[3];
		double V[4]; //Poprawki do wsp
		for (int i=0; i<N; i++)
		{
			ImL[0] = XL[i];
			ImL[1] = YL[i];
			ImL[2] = ckL;
			ImR[0] = XP[i];
			ImR[1] = YP[i];
			ImR[2] = ckP;
			double det = fT_fastModelInteresection(ImL, ImR, r, b, Result, V);


			if (fabs(det) < 1E-12)
			{
				//Nie da siê stwierdzic, Wciecie wprzod slabo wyznaczalne
				return false;
			}
			else
			{
				ImL[2] = -ImL[2]; //Odwracamy znak ck
				ImR[2] = -ImR[2]; //
				//Transformacja wektora ze zdjecia prawego do ukladu modelu
				fT_vectorTrans3(r,ImR,ImRTrans);
				double ResultR[3]; //Wspolrzedne wektora laczacego srodek rzutu zdjecia prawego
				// z wyznaczonym w drodze wciecia w przod punktem
				ResultR[0] = Result[0]-b[0];
				ResultR[1] = Result[1]-b[1];
				ResultR[2] = Result[2]-b[2];

				if ((fT_dotProd3(ImL,Result) > 0) && (fT_dotProd3(ImRTrans,ResultR) > 0) && Result[2] < 0) //trzeci warunek dotyczy ujemnej wsp. z
				{
					Cheriality = true;
				}
				else
				{
					return false;
				}

			}
		}
	}
	else
	{
		return false;
	}

	return Cheriality;


}



void fT_vectorTrans3(double *A, double* v, double *vt)
{
	//Transformacja wektora
	//Funkcja do mnozenia macierzy o wymiarach 3x3 przez wektor 3x1
	//vt = A*v
	vt[0] = A[0]*v[0] + A[1]*v[1] + A[2]*v[2];
	vt[1] = A[3]*v[0] + A[4]*v[1] + A[5]*v[2];
	vt[2] = A[6]*v[0] + A[7]*v[1] + A[8]*v[2];
}

void fT_vectorTrans3(double *A, double *b, double* v, double *vt)
{
	//Transformacja wektora
	//Funkcja do mnozenia macierzy o wymiarach 3x3 przez wektor 3x1
	//vt = A*v+b
	vt[0] = A[0]*v[0] + A[1]*v[1] + A[2]*v[2] + b[0];
	vt[1] = A[3]*v[0] + A[4]*v[1] + A[5]*v[2] + b[1];
	vt[2] = A[6]*v[0] + A[7]*v[1] + A[8]*v[2] + b[2];
}

void fT_vectorTrans3(double *A, TerrainPoint *v, TerrainPoint *vt)
{
	*vt = *v; //Przypisanie wszystkich danych
	vt->X = A[0]*v->X + A[1]*v->Y + A[2]*v->Z;
	vt->Y = A[3]*v->X + A[4]*v->Y + A[5]*v->Z;
	vt->Z = A[6]*v->X + A[7]*v->Y + A[8]*v->Z;
}

void fT_vectorTrans3(double *A, vector<double> *v, vector<double> *vt)
{
	(*vt)[0] = A[0]*(*v)[0] + A[1]*(*v)[1] + A[2]*(*v)[2];
	(*vt)[1] = A[3]*(*v)[0] + A[4]*(*v)[1] + A[5]*(*v)[2];
	(*vt)[2] = A[6]*(*v)[0] + A[7]*(*v)[1] + A[8]*(*v)[2];
}

void fT_transpose3(double *A, double *AT)
{
	//Transpozycja macierzy 3x3;
	AT[0] = A[0];
	AT[1] = A[3];
	AT[2] = A[6];

	AT[3] = A[1];
	AT[4] = A[4];
	AT[5] = A[7];

	AT[6] = A[2];
	AT[7] = A[5];
	AT[8] = A[8];
}

void fT_matTranspose(double *A, int *sizeA, double *AT)
{
	//transpoza macierzy A
	//wynik zapisywany jest w AT
	//wymiary macierzy A podane w sizeA jako [wiersze kolumny]
	for (int i = 0; i < sizeA[0]; i++)
	{
		for (int j = 0; j < sizeA[1]; j++)
		{
			AT[sizeA[0] * j + i] = A[sizeA[1] * i + j];
		}
	}
}

void fT_matInv3(double*A, double *B)
{
	//funkcja oblica odwrotnosc macierzy 3 x 3;
	double Mi[9]; //Macierz minorow
	double DET = A[0] * A[4] * A[8] + A[1] * A[5] * A[6] + A[2] * A[3] * A[7] - A[2] * A[4] * A[6] - A[1] * A[3] * A[8] - A[0] * A[5] * A[7];

	Mi[0] = A[4] * A[8] - A[5] * A[7];
	Mi[1] = A[3] * A[8] - A[5] * A[6];
	Mi[2] = A[3] * A[7] - A[4] * A[6];

	Mi[3] = A[1] * A[8] - A[2] * A[7];
	Mi[4] = A[0] * A[8] - A[2] * A[6];
	Mi[5] = A[0] * A[7] - A[1] * A[6];

	Mi[6] = A[1] * A[5] - A[2] * A[4];
	Mi[7] = A[0] * A[5] - A[2] * A[3];
	Mi[8] = A[0] * A[4] - A[1] * A[3];

	B[0] = Mi[0] / DET;
	B[1] = -Mi[1] / DET;
	B[2] = Mi[2] / DET;

	B[3] = -Mi[3] / DET;
	B[4] = Mi[4] / DET;
	B[5] = -Mi[5] / DET;

	B[6] = Mi[6] / DET;
	B[7] = -Mi[7] / DET;
	B[8] = Mi[8] / DET;
}

double fT_dotProd3(double *v, double *w)
{
	//Funkcja oblicza wartosc iloczynu wektorowego wektorow v 3x1 i w 3x1
	return v[0]*w[0]+v[1]*w[1]+v[2]*w[2];
}

bool fT_compMatrix3(double *A, double *B)
{
	//Funkcja sluzy do porownywania dwóch macierzy obrotu
	//Jezeli macierze sa rowne to zwraca wartosc true
	//Jezeli macierze sa rozne to zwraca wartosc false
	//Wymiary obu macierzy musza byc 3 x 3
	double dif = 0;
	for (int i=0; i<9; i++)
	{
		dif = dif + fabs(A[i]-B[i]);
	}
	
	if (dif > 1.0E-10)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void fT_matMult3(double *A, double *B, double *C)
{
	//Funkcja oblicza iloczyn macierzy C=A*B
	//Macierze musza miec wymar 3 x 3 czyli musza byc
	//typu double[9]
	C[0] = A[0]*B[0] + A[1]*B[3] + A[2]*B[6];
	C[1] = A[0]*B[1] + A[1]*B[4] + A[2]*B[7];
	C[2] = A[0]*B[2] + A[1]*B[5] + A[2]*B[8];

	C[3] = A[3]*B[0] + A[4]*B[3] + A[5]*B[6];
	C[4] = A[3]*B[1] + A[4]*B[4] + A[5]*B[7];
	C[5] = A[3]*B[2] + A[4]*B[5] + A[5]*B[8];

	C[6] = A[6]*B[0] + A[7]*B[3] + A[8]*B[6];
	C[7] = A[6]*B[1] + A[7]*B[4] + A[8]*B[7];
	C[8] = A[6]*B[2] + A[7]*B[5] + A[8]*B[8];
}

void fT_matMult(double *A, double *B, int* sizeA, int* sizeB, double *C)
{
	//funkcja oblicza iloczyn macierzy A i B
	//sizeA zawiera wymiarzy macierzy A [WIERSZE KOLUMNY]
	//sizeB zawiera wymiarzy macierzy B [WIERSZE KOLUMNY]
	//macierz C musi miec odpowiedni rozmiar: double C[sizeA[0]*sizeB[1]]
	//ponadto sizeA[1] == sizeB[0]
	for (int i = 0; i < sizeA[0]; i++) //dla kazdego wiersza macierzy A
	{
		for (int j = 0; j < sizeB[1]; j++) //dokonuj mnozenia z odpowiednia kolumna macierzy B
		{
			int PositionInC = sizeB[1] * i + j; //obliczenie pozycji w macierzy C
			double ValueInC = 0;
			for (int k = 0; k < sizeA[1]; k++)
			{
				ValueInC += A[i*sizeA[1] + k] * B[k*sizeB[1] + j];
			}
			C[PositionInC] = ValueInC;
		}
	}
}

void fT_dispMatrix3(double *A)
{
	//Funkcja wyswietla macierz 3x3
	for (int i=0; i<9; i++)
	{
		std::cout <<A[i];
		if ((i+1)%3 == 0)
		{
			std::cout <<endl;
		}
		else
		{
			std::cout <<" ";
		}
	}
}





void fT_calcCentroid(vector<TerrainPoint> *T, TerrainPoint *C)
{
	//Funkcja oblicza centroidy wektora punktow (T)
	//Wynik zapisywany jest w C
	double CX = 0;
	double CY = 0;
	double CZ = 0;
	for (int i=0; i<T->size(); i++)
	{
		CX += (*T)[i].X;
		CY += (*T)[i].Y;
		CZ += (*T)[i].Z;
	}
	C->X = CX/T->size();
	C->Y = CY/T->size();
	C->Z = CZ/T->size();
}


double fT_angMean(double A1, double A2)
{
	//Funkcja zwraca srednia 2 katow

	//upewniamy sie ze katy sa dodatnie:
	if (A1<0) A1 = A1+360;
	if (A2<0) A2 = A2+360;

	if (fabs(A1 -A2) > 180)
	{
		return (A1+A2)/2 - 180;
	}
	else
	{
		return (A1+A2)/2;
	}

}

void fT_fundamental(double *LX, double *LY, double *RX, double *RY, double *F)
{
	//Funkcja oblicza macierz fundamentalna algorytmem 9-punktowym z SVD
	CvMat * A = cvCreateMat(9,9,CV_32FC1);
	CvMat * ATA = cvCreateMat(9,9,CV_32FC1);
	CvMat *W =  cvCreateMat(9,9,CV_32FC1);
	CvMat *U = cvCreateMat(9,9,CV_32FC1);
	CvMat *V = cvCreateMat(9,9,CV_32FC1);
	for (int i=0; i<9; i++)
	{
		cvSetReal2D(A,i,0,LX[i]*RX[i]);
		cvSetReal2D(A,i,1,LX[i]*RY[i]);
		cvSetReal2D(A,i,2,LX[i]);
		cvSetReal2D(A,i,3,RX[i]*LY[i]);
		cvSetReal2D(A,i,4,LY[i]*RY[i]);
		cvSetReal2D(A,i,5,LY[i]);
		cvSetReal2D(A,i,6,RX[i]);
		cvSetReal2D(A,i,7,RY[i]);
		cvSetReal2D(A,i,8,1);
	}

	//cvGEMM(A,A,1,NULL,0,ATA,CV_GEMM_A_T);
	//cvSVD(ATA,W,U,V);
	cvSVD(A,W,U,V);
	for (int i=0; i<9; i++)
	{
		F[i] = cvGetReal2D(V,i,8);
	}

	//wypisywanie kontrolne:
//	cout<<"Wyniki obliczen macierzy F:"<<endl;
//	cout<<"Macierz F: "<<endl;
//	fT_dispMatrix3(F);
//	cout<<"Macierz W"<<endl;
//	for (int i=0; i<9; i++)
//	{
//		for (int j=0; j<9; j++)
//		{
//			cout<<cvGetReal2D(W,i,j)<<" ";
//		}
//		cout<<endl;
//	}
//	cout<<"Macierz U"<<endl;
//	for (int i=0; i<9; i++)
//	{
//		for (int j=0; j<9; j++)
//		{
//			cout<<cvGetReal2D(U,i,j)<<" ";
//		}
//		cout<<endl;
//	}
//	cout<<"Macierz V"<<endl;
//	for (int i=0; i<9; i++)
//	{
//		for (int j=0; j<9; j++)
//		{
//			cout<<cvGetReal2D(V,i,j)<<" ";
//		}
//		cout<<endl;
//	}
//	cout<<"Macierz ATA"<<endl;
//	for (int i=0; i<9; i++)
//	{
//		for (int j=0; j<9; j++)
//		{
//			cout<<cvGetReal2D(ATA,i,j)<<" ";
//		}
//		cout<<endl;
//	}

	//kontrola
	CvMat* UW = cvCreateMat(9,9,CV_32FC1);
	CvMat* UWVT = cvCreateMat(9,9,CV_32FC1);
	cvGEMM(U,W,1,NULL,0,UW,0);
	cvGEMM(UW,V,1,NULL,0,UWVT,CV_GEMM_B_T);
//	cout<<"Macierz ATA kontrlola"<<endl;
//	for (int i=0; i<9; i++)
//	{
//		for (int j=0; j<9; j++)
//		{
//			cout<<cvGetReal2D(UWVT,i,j)<<" ";
//		}
//		cout<<endl;
//	}
	cvReleaseMat(&UW);
	cvReleaseMat(&UWVT);
	//koniec kontroli
	//koniec wypisywanie kontrolnego

	cvReleaseMat(&A);
	cvReleaseMat(&U);
	cvReleaseMat(&W);
	cvReleaseMat(&V);

	cvReleaseMat(&ATA);
}

void fT_f2e(double *F, double *E)
{
	CvMat * A = cvCreateMat(3,3,CV_32FC1);
	for (int i=0; i<9; i++)
	{
		cvSetReal2D(A,i/3,i%3,F[i]);
	}
	CvMat * U = cvCreateMat(3,3,CV_32FC1);
	CvMat * W = cvCreateMat(3,3,CV_32FC1);
	CvMat * V = cvCreateMat(3,3,CV_32FC1);

	cvSVD(A,W,U,V);

	double W_Diag_Value = (cvGetReal2D(W,0,0) + cvGetReal2D(W,1,1))/2;
	cvSetReal2D(W,2,2,0);
	//skalowanie macierzy U
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			cvSetReal2D(U,i,j, cvGetReal2D(U,i,j)*W_Diag_Value);
		}
	}

	CvMat* UW = cvCreateMat(3,3,CV_32FC1);
	cvGEMM(U,W,1,NULL,0,UW,0);
	CvMat* _E = cvCreateMat(3,3,CV_32FC1);
	cvGEMM(UW,V,1,NULL,0,_E,CV_GEMM_B_T);

	cout<<"Sprawdzenie wartosci wyznacznika: "<<endl;
	cout<< cvDet(_E)<<endl;;

	int k=0;
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			E[k] = cvGetReal2D(_E,i,j);
			k++;
		}
	}

	cvReleaseMat(&UW);
	cvReleaseMat(&A);
	cvReleaseMat(&U);
	cvReleaseMat(&W);
	cvReleaseMat(&V);
	cvReleaseMat(&_E);

}
