#include "pch.h"
#include "Transf3DData.h"

Transf3DData::Transf3DData(void)
{
	s = 1;
	EstScale = true;
	Priority = 0;
	for (int i = 0; i < 3; i++) T[i] = 0;
	for (int i = 0; i < 9; i++) R[i] = 0;
	R[0] = 1;
	R[4] = 1;
	R[8] = 1;
	Mx = 0;
	My = 0;
	Mz = 0;
	Mp = 0;
	FixedPointsN = 0;
}

Transf3DData::~Transf3DData(void)
{

}

void Transf3DData::wypisz(void)
{
	cout << "Dane transformacji 3D:\n";
	cout << "Estymacja skali: " << EstScale << "\n";
	cout << "Skala: " << fixed << setprecision(4) << s << "\n";
	cout << "Wektor translacji:\n";
	for (int i = 0; i < 3; i++)
	{
		cout << fixed << setprecision(4) << T[i] << " ";
	}
	cout << endl;
	cout << "Macierz obrotu:\n";
	for (int i = 0; i < 9; i++)
	{
		cout << fixed << setprecision(7) << R[i] << " ";
		if (i % 3 == 2)
		{
			cout << endl;
		}
	}
}

void Transf3DData::solve(vector<TerrainPoint> *LeftPoints, vector<TerrainPoint> *RightPoints)
{
	//Nomenklatura "prawe" i "lewe" jest zaczerpnieta z artykulu zrodlowego algorytmu Horna
	//*LeftPoints - punkty dostosowania w ukladzie lewym
	//*RightPoints - punkty dostosowania w ukladzie prawym
	Names.clear();
	if (LeftPoints->size() == RightPoints->size() && LeftPoints->size() >= 3)
	{
		if (FixedPointsN < 2)
		{
			//Obliczenie centroidow
			TerrainPoint CentroidL("CL", 0, 0, 0);
			TerrainPoint CentroidR("CR", 0, 0, 0);

			if (FixedPointsN == 0)
			{
				fT_calcCentroid(LeftPoints, &CentroidL);
				fT_calcCentroid(RightPoints, &CentroidR);
			}
			else //czyli FixedPointsN == 1
			{
				//cout<<"FixedPointsN: "<<FixedPointsN<<endl;
				CentroidL.X = (*LeftPoints)[FixedPointsIdx[0]].X;
				CentroidL.Y = (*LeftPoints)[FixedPointsIdx[0]].Y;
				CentroidL.Z = (*LeftPoints)[FixedPointsIdx[0]].Z;
				CentroidR.X = (*RightPoints)[FixedPointsIdx[0]].X;
				CentroidR.Y = (*RightPoints)[FixedPointsIdx[0]].Y;
				CentroidR.Z = (*RightPoints)[FixedPointsIdx[0]].Z;
			}

			vector<TerrainPoint> CL; //Punkty ze zbioru lewego centrowane
			vector<TerrainPoint> CR; //Punkty ze zboiru prawego centrowane

			TerrainPoint Temp;
			//Redukcja do centroidow
			for (int i = 0; i < LeftPoints->size(); i++)
			{
				if ((FixedPointsN == 0) || ((FixedPointsN == 1) && (FixedPointsIdx[0] != i)))
					//aby nie uwzgledniac tego jednego wspolnego punku przy estymacji
				{
					Temp.X = (*LeftPoints)[i].X - CentroidL.X;
					Temp.Y = (*LeftPoints)[i].Y - CentroidL.Y;
					Temp.Z = (*LeftPoints)[i].Z - CentroidL.Z;
					CL.push_back(Temp);
					Temp.X = (*RightPoints)[i].X - CentroidR.X;
					Temp.Y = (*RightPoints)[i].Y - CentroidR.Y;
					Temp.Z = (*RightPoints)[i].Z - CentroidR.Z;
					CR.push_back(Temp);
				}
				Names.push_back((*LeftPoints)[i].Name);
			}

			//Obliczenie wspolczynnikow macierzy M
			double M[9];
			for (int i = 0; i < 9; i++) M[i] = 0; //inicjalizacja

			for (int i = 0; i < CL.size(); i++)
			{
				M[0] += CL[i].X * CR[i].X;
				M[1] += CL[i].X * CR[i].Y;
				M[2] += CL[i].X * CR[i].Z;

				M[3] += CL[i].Y * CR[i].X;
				M[4] += CL[i].Y * CR[i].Y;
				M[5] += CL[i].Y * CR[i].Z;

				M[6] += CL[i].Z * CR[i].X;
				M[7] += CL[i].Z * CR[i].Y;
				M[8] += CL[i].Z * CR[i].Z;
			}

			//Obliczenie wspolczynnikow macierzy N
			CvMat *N = cvCreateMat(4, 4, CV_32FC1);

			cvSetReal2D(N, 0, 0, M[0] + M[4] + M[8]);
			cvSetReal2D(N, 0, 1, M[5] - M[7]);
			cvSetReal2D(N, 0, 2, M[6] - M[2]);
			cvSetReal2D(N, 0, 3, M[1] - M[3]);

			cvSetReal2D(N, 1, 0, M[5] - M[7]);
			cvSetReal2D(N, 1, 1, M[0] - M[4] - M[8]);
			cvSetReal2D(N, 1, 2, M[1] + M[3]);
			cvSetReal2D(N, 1, 3, M[6] + M[2]);

			cvSetReal2D(N, 2, 0, M[6] - M[2]);
			cvSetReal2D(N, 2, 1, M[1] + M[3]);
			cvSetReal2D(N, 2, 2, -M[0] + M[4] - M[8]);
			cvSetReal2D(N, 2, 3, M[5] + M[7]);

			cvSetReal2D(N, 3, 0, M[1] - M[3]);
			cvSetReal2D(N, 3, 1, M[6] + M[2]);
			cvSetReal2D(N, 3, 2, M[5] + M[7]);
			cvSetReal2D(N, 3, 3, -M[0] - M[4] + M[8]);

			//Obliczenie wektorow wlasnych
			CvMat *EVects = cvCreateMat(4, 4, CV_32FC1);
			CvMat *EVals = cvCreateMat(4, 1, CV_32FC1);
			cvEigenVV(N, EVects, EVals, 1E-15);

			double MinValue, MaxValue;
			CvPoint P;
			cvMinMaxLoc(EVals, &MinValue, &MaxValue, NULL, &P, NULL);

			//Wyznaczenie quaternionu
			double Q[4];
			for (int i = 0; i < 4; i++) Q[i] = cvGetReal2D(EVects, P.x, i);

			//cout <<"\nQuaternion: "<<endl;
			//for (int i=0; i<4; i++) cout<<Q[i]<<endl;
			//double s=0;
			//for (int i=0; i<4; i++) s+=Q[i]*Q[i];
			//cout << s <<endl;

			//Wyznaczenie macierzy obrotu
			fT_quaternions2rot(Q, R);

			double SumDL = 0;
			double SumDR = 0;

			//Sumy odleglosci od centroidow
			for (int i = 0; i < CL.size(); i++)
			{
				SumDL += CL[i].X*CL[i].X + CL[i].Y*CL[i].Y + CL[i].Z*CL[i].Z;
				SumDR += CR[i].X*CR[i].X + CR[i].Y*CR[i].Y + CR[i].Z*CR[i].Z;
			}

			double SumCrossLR = 0;
			double SumCrossRL = 0;
			TerrainPoint CL_Rot; //wektor po obrocie lewy
			TerrainPoint CR_Rot; //wektor po obrocie prawy

			double RT[9];
			fT_transpose3(R, RT);

			//Sumy iloczynow wektorowych z uwzglednieniem obrotu
			for (int i = 0; i < CL.size(); i++)
			{
				fT_vectorTrans3(R, &(CL[i]), &CL_Rot);
				fT_vectorTrans3(RT, &(CR[i]), &CR_Rot);
				SumCrossLR += CL[i].X * CR_Rot.X + CL[i].Y * CR_Rot.Y + CL[i].Z * CR_Rot.Z;
				SumCrossRL += CR[i].X * CL_Rot.X + CR[i].Y * CL_Rot.Y + CR[i].Z * CL_Rot.Z;
			}


			if (EstScale == true)
			{
				//Wyznaczenie skali
				switch (Priority)
				{
				case 0: // Priorytet: rownodokladne
					s = sqrt(SumDR / SumDL);
					break;
				case 1: // Priorytet: lewy dokladniejszy
					s = SumDR / SumCrossLR;
					break;
				case 2: // Priorytet: prawy dokladniejszy
					s = SumCrossRL / SumDL;
					break;
				}
			}
			else
			{
				s = 1;
			}

			//Wyznaczenie wektora translacji
			TerrainPoint SRLC; //scaled and rotated left centroid
			fT_vectorTrans3(R, &CentroidL, &SRLC);
			SRLC.X = s * SRLC.X;
			SRLC.Y = s * SRLC.Y;
			SRLC.Z = s * SRLC.Z;
			T[0] = CentroidR.X - SRLC.X;
			T[1] = CentroidR.Y - SRLC.Y;
			T[2] = CentroidR.Z - SRLC.Z;
		}
		calcErrors(LeftPoints, RightPoints);
	}
}

void Transf3DData::transform(vector<TerrainPoint> *Source, vector<TerrainPoint>* Target)
{
	//Transformacja zbioru punktow
	TerrainPoint TarPoint;
	for (int i = 0; i < Source->size(); i++)
	{
		fT_vectorTrans3(R, &(*Source)[i], &TarPoint);
		TarPoint.X = s * TarPoint.X + T[0];
		TarPoint.Y = s * TarPoint.Y + T[1];
		TarPoint.Z = s * TarPoint.Z + T[2];
		Target->push_back(TarPoint);
	}
}

void Transf3DData::transform(double *Source, double *Target)
{
	//Transformacja pojedynczego punktu
	fT_vectorTrans3(R, Source, Target);
	Target[0] = s * Target[0] + T[0];
	Target[1] = s * Target[1] + T[1];
	Target[2] = s * Target[2] + T[2];

}

void Transf3DData::transform(vector<double> *Source, vector<double> *Target)
{
	double So[3];
	double Ta[3];
	So[0] = Source->at(0);
	So[1] = Source->at(1);
	So[2] = Source->at(2);
	fT_vectorTrans3(R, So, Ta);
	(*Target).resize(3);
	(*Target)[0] = s * Ta[0] + T[0];
	(*Target)[1] = s * Ta[1] + T[1];
	(*Target)[2] = s * Ta[2] + T[2];
}


void Transf3DData::calcErrors(vector<TerrainPoint> *LeftPoints, vector<TerrainPoint> *RightPoints)
{
	//Obliczenie wsplrzednych punktow lewych po transformacji celem oceny dokladnosci
	TerrainPoint SRLC;
	vector<TerrainPoint> TransfLeftPoints;
	for (int i = 0; i < LeftPoints->size(); i++)
	{
		fT_vectorTrans3(R, &(*LeftPoints)[i], &SRLC);
		TransfLeftPoints.push_back(SRLC);
		TransfLeftPoints[i].X = s * TransfLeftPoints[i].X + T[0];
		TransfLeftPoints[i].Y = s * TransfLeftPoints[i].Y + T[1];
		TransfLeftPoints[i].Z = s * TransfLeftPoints[i].Z + T[2];
	}

	//Obliczenie poprawek
	VX.clear();
	VY.clear();
	VZ.clear();
	VX.resize(TransfLeftPoints.size());
	VY.resize(TransfLeftPoints.size());
	VZ.resize(TransfLeftPoints.size());
	for (int i = 0; i < TransfLeftPoints.size(); i++)
	{
		VX[i] = (*RightPoints)[i].X - TransfLeftPoints[i].X;
		VY[i] = (*RightPoints)[i].Y - TransfLeftPoints[i].Y;
		VZ[i] = (*RightPoints)[i].Z - TransfLeftPoints[i].Z;
	}

	Mx = 0;
	My = 0;
	Mz = 0;
	//Obliczenie bledow srednich
	for (int i = 0; i < VX.size(); i++)
	{
		Mx += VX[i] * VX[i];
		My += VY[i] * VY[i];
		Mz += VZ[i] * VZ[i];
	}

	if (s == 0) //pary spostrzezen rownodokladnych
	{
		Mx = pow(Mx / (VX.size() - 1), 0.5);
		My = pow(My / (VY.size() - 1), 0.5);
		Mz = pow(Mz / (VZ.size() - 1), 0.5);
	}
	else //pary spostrzezen roznodokladnych
	{
		Mx = pow(Mx / VX.size(), 0.5);
		My = pow(My / VY.size(), 0.5);
		Mz = pow(Mz / VZ.size(), 0.5);
	}

	Mp = pow(Mx*Mx + My * My + Mz * Mz, 0.5);
}

void Transf3DData::applyToModel(Model3D *InputModel, Image *LeftImage, Image *RightImage, bool SetApr, bool SetObs, bool SetAdj)
{
	//Funkcja sluzy do wyznaczania elementow orientacji zewnetrznej zdjec na podstawie elemento orientacji bezwzglednej i wzajemnej
	//Obroty obliczenia
	double LeftAngles[3];
	double RightAngles[3];
	double RR[9]; //Macierz obrotu dla zdjecia prawego
	fT_rot2angles(R, LeftAngles, LeftImage->AngularSystem);
	fT_matMult3(R, InputModel->r, RR); //Obliczamy macierz obrotu dla zdjecia prawego
	fT_rot2angles(RR, RightAngles, RightImage->AngularSystem);

	//Wspolrzedne srodkow rzutu - obliczenia
	double LeftPC[3];
	double RightPC[3];
	double LeftModel[3];
	LeftModel[0] = 0;
	LeftModel[1] = 0;
	LeftModel[2] = 0;
	transform(LeftModel, LeftPC);
	transform(InputModel->b, RightPC);

	//Ustawienie EO
	if (SetApr == true)
	{
		LeftImage->setAnglesApr(LeftAngles);
		RightImage->setAnglesApr(RightAngles);
		LeftImage->setCoordsApr(LeftPC);
		RightImage->setCoordsApr(RightPC);
	}

	if (SetObs == true)
	{
		LeftImage->setAnglesObs(LeftAngles);
		RightImage->setAnglesObs(RightAngles);
		LeftImage->setCoordsObs(LeftPC);
		RightImage->setCoordsObs(RightPC);
	}

	if (SetAdj == true)
	{
		LeftImage->setRotationMatrix(R);
		RightImage->setRotationMatrix(RR);
		LeftImage->setCoordsAdj(LeftPC);
		RightImage->setCoordsAdj(RightPC);
	}
}



void Transf3DData::createcomb(vector <vector<int> > * Comb, int N)
{
	int i1 = 0;
	int i2 = 1;
	int i3 = 2;
	for (i1 = 0; i1 <= N - 3; i1++)
	{
		for (i2 = i1 + 1; i2 <= N - 2; i2++)
		{
			for (i3 = i2 + 1; i3 <= N - 1; i3++)
			{
				vector<int> COMB;
				COMB.push_back(i1);
				COMB.push_back(i2);
				COMB.push_back(i3);
				(*Comb).push_back(COMB);
			}
		}
	}
}

void Transf3DData::create3PointSamples(vector <vector<int> > * Samples, vector<TerrainPoint> *P, int MaxSamples)
{
	//Funkcja generuje 3 punktowe probki dla procedury RANSAC
	//MaxSamples oznacza maksymalna ilosc probek
	//Samples to wynik dzialania programu
	//P to wskaznik do wektora elementow TerrainPoint
	vector <vector<int> > AllSamples; //Pusty wektor z 5 punktami
	if (P->size() <= 6) //Przypadek malej ilosci punktow
	{
		//Sprawdzamy wszystkie kombinacje	
		createcomb(&AllSamples, P->size());
	}
	else
	{
		for (int los = 0; los < MaxSamples; los++) //Losowanie kolejnych probek 
		{

			vector<int> AvailablePoints; //Budujemy sobie wektor z liczbami od 0 do P->size()-1
			for (int i = 0; i < P->size(); i++)
			{
				bool CzyMogeDopisac = true;
				for (int j = 0; j < FixedPointsN; j++)
				{
					if (i == FixedPointsIdx[j])
					{
						CzyMogeDopisac = false;
						break;
					}
				}
				if (CzyMogeDopisac == true) AvailablePoints.push_back(i);
			}

			vector<int> Samp;
			for (int i = 0; i < 3; i++)//Proces losowania 3 punktow do probki
			{
				int PointNr = rand() % (AvailablePoints.size() - 1);
				Samp.push_back(AvailablePoints.at(PointNr));
				AvailablePoints.erase(AvailablePoints.begin() + PointNr);
			}
			AllSamples.push_back(Samp);
		}
	}
	*Samples = AllSamples;
}

void Transf3DData::checkByRansac(vector<TerrainPoint> *LeftPoints, vector<TerrainPoint> *RightPoints, int NSamples, double Thresh)
{
	//short TempScalePriority = ScalePriority;
	//ScalePriority = 2;
	Outliers.clear();
	vector <vector<int> > Samples;
	create3PointSamples(&Samples, LeftPoints, NSamples);

	//	cout<<"Utworzone probki do RANSAC: "<<endl;
	//	for (int i=0; i<Samples.size(); i++)
	//	{
	//		cout << i<<": " <<Samples[i][0]<<" "<< Samples[i][1] <<" "<< Samples[i][2] <<endl;
	//	}

		//Wybieramy punkty
	vector<TerrainPoint> LP;
	vector<TerrainPoint> RP;
	vector<TerrainPoint> BestPoints; //Najlepsza probka
	int BestSample[3];
	for (int i = 0; i < 3; i++) BestSample[i] = 0;
	double BestRank = 0;
	double SampRank = 0;
	int TempFixedPointsIdx;
	for (int samp = 0; samp < Samples.size(); samp++) //Dla kazdej probki
	{
		LP.clear();
		RP.clear();
		if (FixedPointsN == 1) //Przypadek gdy 1 punkt jest staly
		{
			LP.push_back((*LeftPoints)[FixedPointsIdx[0]]);
			RP.push_back((*RightPoints)[FixedPointsIdx[0]]);
		}

		for (int i = 0; i < 3; i++)
		{
			LP.push_back((*LeftPoints)[Samples[samp][i]]);
			RP.push_back((*RightPoints)[Samples[samp][i]]);
		}


		if (FixedPointsN == 1)
		{
			TempFixedPointsIdx = FixedPointsIdx[0];
			FixedPointsIdx[0] = 0;
		}

		solve(&LP, &RP);
		calcErrors(LeftPoints, RightPoints);
		FixedPointsIdx[0] = TempFixedPointsIdx;
		//Tutaj trzeba by poprawic sposob obliczania SampRank
		SampRank = calcRank(Thresh);

		if (SampRank > BestRank)
		{
			BestRank = SampRank;
			BestSample[0] = Samples[samp][0];
			BestSample[1] = Samples[samp][1];
			BestSample[2] = Samples[samp][2];
		}
	}
	LP.clear();
	RP.clear();

	if (FixedPointsN == 1) //Przypadek gdy 1 punkt jest staly
	{
		LP.push_back((*LeftPoints)[FixedPointsIdx[0]]);
		RP.push_back((*RightPoints)[FixedPointsIdx[0]]);
	}

	for (int i = 0; i < 3; i++)
	{
		LP.push_back((*LeftPoints)[BestSample[i]]);
		RP.push_back((*RightPoints)[BestSample[i]]);
	}

	if (FixedPointsN == 1)
	{
		TempFixedPointsIdx = FixedPointsIdx[0];
		FixedPointsIdx[0] = 0;
	}
	solve(&LP, &RP);
	calcErrors(LeftPoints, RightPoints);
	FixedPointsIdx[0] = TempFixedPointsIdx;
	//cout <<"Best sample:\n";
	//cout <<(*LeftPoints)[BestSample[0]].Name<<" "<<(*LeftPoints)[BestSample[1]].Name<<" "<<(*LeftPoints)[BestSample[2]].Name<<endl;
	//cout <<(*RightPoints)[BestSample[0]].Name<<" "<<(*RightPoints)[BestSample[1]].Name<<" "<<(*RightPoints)[BestSample[2]].Name<<endl; 
	//cout<<"Corrections for best sample:\n";
	for (int i = 0; i < (*LeftPoints).size(); i++)
	{
		double V = sqrt(VX[i] * VX[i] + VY[i] * VY[i] + VZ[i] * VZ[i]);
		if (V > Thresh)
		{
			Outliers.push_back(i);
		}
		//cout << (*LeftPoints)[i].Name <<" "<<VX[i] <<" "<<VY[i]<< " "<<VZ[i]<<endl;
	}
}

double Transf3DData::calcRank(double Thresh)
{
	double R = 0;
	for (int i = 0; i < VX.size(); i++)
	{
		double V = sqrt(VX[i] * VX[i] + VY[i] * VY[i] + VZ[i] * VZ[i]);
		if (V < Thresh)
		{
			R = R + 1 - V / Thresh;
		}
	}
	return R;
}
