#include "pch.h"
#include "Model3D.h"

Model3D::Model3D(void)
{
}

Model3D::~Model3D(void)
{
}

void Model3D::readFromFile(string Filename)
{
	string s;
	ifstream STR;
	//cout<<Filename.c_str()<<endl;
	STR.open(Filename.c_str());
	PL.clear();
	PR.clear();
	TerPoints.clear();
	STR >> s;
	STR >> FirstImageName >> SecondImageName;
	STR >> s;
	STR >> ckL >> ckR;
	STR >> s >> NPoints;
	STR >> s;
	for (int i = 0; i < NPoints; i++)
	{
		string Name;
		double xL, yL;
		double xR, yR;
		STR >> Name >> xL >> yL >> xR >> yR;
		SimplePoint<string> PunktLewy(xL, yL, Name);
		SimplePoint<string> PunktPrawy(xR, yR, Name);
		PL.push_back(PunktLewy);
		PR.push_back(PunktPrawy);
	}
	STR >> s;
	for (int i = 0; i < NPoints; i++)
	{
		string Name;
		double X, Y, Z;
		STR >> Name >> X >> Y >> Z;
		TerrainPoint T(PL[i].Nr, X, Y, Z);
		TerPoints.push_back(T);
	}
	STR >> s;
	STR >> r[0] >> r[1] >> r[2];
	STR >> r[3] >> r[4] >> r[5];
	STR >> r[6] >> r[7] >> r[8];
	STR >> b[0] >> b[1] >> b[2];
	STR.close();

}

void Model3D::writeToFile(string Filename)
{
	ofstream STR;
	STR.open(Filename.c_str());
	STR << "Images:\n";
	STR << FirstImageName << " " << SecondImageName << "\n";
	STR << "Cameras:\n";
	STR << fixed << setprecision(4) << ckL << " " << ckR << "\n";
	STR << "#Points: " << NPoints << "\n";
	STR << "Fiducial_distortion_free_coordinates:\n";
	for (int i = 0; i < NPoints; i++)
	{
		STR << PL[i].Nr << " " << fixed << setprecision(3) << PL[i].x << " " << PL[i].y << " " << PR[i].x << " " << PR[i].y << "\n";
	}
	STR << "Model_Coordinates:\n";
	for (int i = 0; i < NPoints; i++)
	{
		STR << fixed << setprecision(5) << PL[i].Nr << " " << TerPoints[i].X << " " << TerPoints[i].Y << " " << TerPoints[i].Z << "\n";
	}
	STR << "Orientation:\n";
	STR << fixed << setprecision(18) << r[0] << " " << r[1] << " " << r[2] << "\n";
	STR << fixed << setprecision(18) << r[3] << " " << r[4] << " " << r[5] << "\n";
	STR << fixed << setprecision(18) << r[6] << " " << r[7] << " " << r[8] << "\n";
	STR << fixed << setprecision(10) << b[0] << " " << b[1] << " " << b[2] << "\n";
	STR.close();

}

void Model3D::wypisz(void)
{
	cout << "Wypisywanie modelu:" << endl;
	cout << "#Points: " << NPoints << endl;
	cout << "Left ck: " << ckL << endl;
	cout << "Right ck: " << ckR << endl;
	cout << "Image point coordinates:" << endl;
	for (int i = 0; i < NPoints; i++)
	{
		cout << fixed << setprecision(3) << PL[i].Nr << " " << PL[i].x << " " << PL[i].y << " " << PR[i].x << " " << PR[i].y << endl;
	}
	cout << "Model_Coordinates:" << endl;
	for (int i = 0; i < NPoints; i++)
	{
		cout << fixed << setprecision(5) << TerPoints[i].X << " " << TerPoints[i].Y << " " << TerPoints[i].Z << endl;
	}
	cout << "Orientation:\n";
	cout << fixed << setprecision(12) << r[0] << " " << r[1] << " " << r[2] << endl;
	cout << fixed << setprecision(12) << r[3] << " " << r[4] << " " << r[5] << endl;
	cout << fixed << setprecision(12) << r[6] << " " << r[7] << " " << r[8] << endl;
	cout << fixed << setprecision(5) << b[0] << " " << b[1] << " " << b[2] << endl;

}


void Model3D::calcModelCoords(void)
{
	//Funkcja oblicza wspolrzedne w ukladzie modelu
	//na podstawie wspolrzednych tlowych, EOW i elementow orientacji wzajemnej
	for (int i = 0; i < NPoints; i++)
	{
		double ImVecL[3];
		double ImVecR[3];
		ImVecL[0] = PL[i].x;
		ImVecL[1] = PL[i].y;
		ImVecL[2] = ckL;
		ImVecR[0] = PR[i].x;
		ImVecR[1] = PR[i].y;
		ImVecR[2] = ckR;
		double Result[3];
		double V[4];
		fT_fastModelInteresection(ImVecL, ImVecR, r, b, Result, V);
		TerPoints[i].X = Result[0];
		TerPoints[i].Y = Result[1];
		TerPoints[i].Z = Result[2];
		cout << "V[0] = " << V[0] << " V[1] = " << V[1] << endl;
	}
}
