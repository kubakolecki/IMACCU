#include "pch.h"
#include "Image.h"

using namespace std;

Image::Image(void)
{
	ImageFileName = "";
	NPoints = 0; //Ile punktow do tej pory pomierzono na zdjeciu
	CamFileName = ""; //Nazwa pliku kamery
	Name = "";
	HasDist = 0;
	W = 0;
	H = 0;

	for (int i=0; i<4; i++)
	{
		Transf_x[i]=0;
		Transf_y[i]=0;
	}

	for (int i=0; i<3; i++)
	{
		InteriorAdjust[i] = 2;
		Interior[i] = 1000;
		Apriori_IErr[i] = 1;
		Aposteriori_IErr[i] = 1;
		kAdjust[i] = 2;
		k[i]=0;
		Apriori_kErr[i] = 1;
		Aposteriori_kErr[i] = 1;
	}

	for (int i=0; i<2; i++)
	{
		pAdjust[i] = 2;
		p[i] = 0;
		Apriori_pErr[i] = 1;
		Aposteriori_pErr[i] = 1;
		AddAdjust[i] = 2;
		Add[i] = 0;
		Apriori_AddErr[i] = 1;
		Aposteriori_AddErr[i] = 1;
	}

	AngularSystem = "al-ni-ka";
	for (int i=0; i<3; i++)
	{
		AnglesAdjust[i] = 0;
		AnglesApr[i] = 0; // Katy poczatkowe - podane
		AnglesObs[i] = 0; // Katy obserwowane
		AnglesAdj[i] = 0; // Katy wyliczone
		AnglesAprErr[i] = 1; //Bledy katow przyblizonych
		AnglesObsErr[i] = 1; //Bledy katow obserwowanych
		AnglesAdjErr[i] = 1; //Bledy aposteriori (po wyrownaniu)

		CoordsAdjust[i] = 0; // Wektor okreslajacy charakter wspolrzednych // unknown, approximated, fixed
		CoordsApr[i] = 100; // Wspolrzedne podane
		CoordsObs[i] = 100;
		CoordsAdj[i] = 100; // Wspolrzedne wyliczone
		CoordsAprErr[i] = 0.01; //Bledy wspolrzednych
		CoordsObsErr[i] = 0.01;
		CoordsAdjErr[i] = 0.01;
	}
	for (int i=0; i<9; i++) R[i] = 0;
	R[0] = 1;
	R[4] = 1;
	R[8] = 1;

	for (int i=0; i<4; i++) Quaternions[i] = 0;
	fT_rot2quaternions(R, Quaternions);

	for (int i=0; i<11; i++) DLT[i] = 0;
}

Image::Image(Camera kamera)
{
	CamFileName = kamera.Name;
	NPoints = 0;
	for (int i=0; i < 4; i++)
	{
		if (i == 0) {Transf_x[i] = 1;}
		else {Transf_x[i] = 0;}
	}
	for (int i=0; i < 4; i++)
	{
		if (i == 1) {Transf_y[i] = 1;}
		else {Transf_y[i] = 0;}
	}
	
	//Size
	W = kamera.W;
	H = kamera.H;

	//Interior
	for (int i=0; i < 3; i++) InteriorAdjust[i] = 2;	
	Interior[0] = kamera.ck;
	Interior[1] = kamera.x0;
	Interior[2] = kamera.y0;
	for (int i=0; i < 3; i++) Apriori_IErr[i] = 1;
	for (int i=0; i < 3; i++) Aposteriori_IErr[i] =1;

    //k
	for (int i=0; i <3; i++) kAdjust[i] = 2;
	k[0] = kamera.k1;
	k[1] = kamera.k2;
	k[2] = kamera.k3;
	for (int i=0; i < 3; i++) Apriori_kErr[i] = 1;
	for (int i=0; i < 3; i++) Aposteriori_kErr[i] =1;	

	//p
	for (int i=0; i < 2; i++) pAdjust[i] = 2;
	p[0] = kamera.p1;
	p[1] = kamera.p2;
	for (int i=0; i < 2; i++) Apriori_pErr[i] = 1;
	for (int i=0; i < 2; i++) Aposteriori_pErr[i] =1;	

	//Add
	for (int i=0; i < 2; i++) AddAdjust[i] = 2;
	Add[0] = kamera.y_scaling;
	Add[1] = kamera.skewness;	
	for (int i=0; i < 2; i++) Apriori_AddErr[i] = 1;
	for (int i=0; i < 2; i++) Aposteriori_AddErr[i] =1;
	
	double Angles[3];
	for (int i=0; i < 3; i++) Angles[i] = 0;

	AngularSystem = "al-ni-ka";
	for (int i=0; i < 3; i++) AnglesAdjust[i] = 0;
	setAnglesApr(Angles);
	setAnglesObs(Angles);
	setAnglesAdj(Angles);
	//cout <<AngularSystem <<endl;
	for (int i=0; i < 3; i++) AnglesAprErr[i] = 1;
	for (int i=0; i < 3; i++) AnglesObsErr[i] = 1;
	for (int i=0; i < 3; i++) AnglesAdjErr[i] = 1;
	for (int i=0; i < 3; i++) CoordsAdjust[i] = 0;
	for (int i=0; i < 3; i++) CoordsApr[i] = 100;
	for (int i=0; i < 3; i++) CoordsObs[i] = 100;
	for (int i=0; i < 3; i++) CoordsAdj[i] = 100;
	for (int i=0; i < 3; i++) CoordsAprErr[i] = 0.01;
	for (int i=0; i < 3; i++) CoordsObsErr[i] = 0.01;
	for (int i=0; i < 3; i++) CoordsAdjErr[i] = 0.01;
	setDLT();
	HasDist = checkDist();

}

Image::~Image(void)
{
}

short Image::WriteToFile(const char * Filename)
{
	ofstream IDA;
	IDA.open(Filename);
	if (IDA)
	{
		IDA << ImageFileName << "\n";
		IDA << Name << "\n";
		IDA << CamFileName <<"\n";
		IDA << "size: "<<this->W<<" "<<this->H<<"\n";
		IDA << "number_of_image_points "<<this->NPoints<<"\n";

		IDA << "transf_x: "<<fixed<<setprecision(8)<<this->Transf_x[0]<<" "<<this->Transf_x[1]<<" "<<this->Transf_x[2]<<" "<<this->Transf_x[3]<<"\n";
		IDA << "transf_y: "<<fixed<<setprecision(8)<<this->Transf_y[0]<<" "<<this->Transf_y[1]<<" "<<this->Transf_y[2]<<" "<<this->Transf_y[3]<<"\n";

		IDA << "interior_adjust " <<InteriorAdjust[0]<<" "<<InteriorAdjust[1]<<" "<<InteriorAdjust[2]<<"\n";
		IDA << "interior "<<fixed<<setprecision(3)<<this->Interior[0] <<" "<<this->Interior[1]<<" "<<this->Interior[2] <<"\n";
		IDA << "interior_apriori_errors "<<fixed<<setprecision(3)<<this->Apriori_IErr[0] <<" "<<this->Apriori_IErr[1] <<" "<<this->Apriori_IErr[2] <<"\n";
		IDA << "interior_aposteriori_errors "<<fixed<<setprecision(3)<<this->Aposteriori_IErr[0] <<" "<<this->Aposteriori_IErr[1] <<" "<<this->Aposteriori_IErr[2] <<"\n";

		IDA << "radial_distortion_adjust "<<kAdjust[0]<<" "<<kAdjust[1]<<" "<<kAdjust[2]<<"\n";
		IDA << "radial_distortion " <<scientific<<setprecision(4)<<this->k[0] <<" " <<this->k[1] <<" " <<this->k[2] <<" " << "\n";
		IDA << "radial_distortion_apriori_errors "<< scientific<<setprecision(4)<<this->Apriori_kErr[0] <<" " <<this->Apriori_kErr[1] <<" " <<this->Apriori_kErr[2] <<" " <<"\n";
		IDA << "radial_distortion_aposteriori_errors "<< scientific<<setprecision(4)<<this->Aposteriori_kErr[0] <<" " <<this->Aposteriori_kErr[1] <<" " <<this->Aposteriori_kErr[2] <<" " <<"\n";

		IDA << "tangential_distortion_adjust " <<pAdjust[0]<<" "<<pAdjust[1]<<"\n";
		IDA << "tangential_distortion " << scientific<<setprecision(4)<<this->p[0] <<" "<<this->p[1] <<"\n";
		IDA << "tangential_distortion_apriori_errors " << scientific<<setprecision(4)<<this->Apriori_pErr[0] <<" "<<this->Apriori_pErr[1] <<"\n";
		IDA << "tangential_distortion_aposteriori_errors " << scientific<<setprecision(4)<<this->Aposteriori_pErr[0] <<" "<<this->Aposteriori_pErr[1] <<"\n";

		IDA << "add_param_adjust " <<AddAdjust[0]<<" "<<AddAdjust[1]<<"\n";
		IDA << "add_param " <<scientific<<setprecision(4)<<this->Add[0] <<" "<<this->Add[1] <<"\n";
		IDA << "add_param_apriori_errors "<<scientific<<setprecision(4)<<this->Apriori_AddErr[0]<<" " <<this->Apriori_AddErr[1]<<"\n";
		IDA << "add_param_aposteriori_errors "<<scientific<<setprecision(4)<<this->Aposteriori_AddErr[0]<<" " <<this->Aposteriori_AddErr[1]<<"\n";

		IDA << "rotation " << this->AngularSystem <<"\n";
		IDA << "angles_adjust " <<AnglesAdjust[0] <<" "<<AnglesAdjust[1] <<" "<<AnglesAdjust[1]<< "\n";
		IDA << "angles_apr " <<fixed<<setprecision(4)<<this->AnglesApr[0] << " " <<this->AnglesApr[1] << " " <<this->AnglesApr[2] << "\n";
		IDA << "angles_obs " <<fixed<<setprecision(4)<<this->AnglesObs[0] << " " <<this->AnglesObs[1] << " " <<this->AnglesObs[2] << "\n";
		IDA << "angles_adj " <<fixed<<setprecision(4)<<this->AnglesAdj[0] << " " <<this->AnglesAdj[1] << " " <<this->AnglesAdj[2] << "\n";
		IDA << "angles_apr_err " <<fixed<<setprecision(4)<<this->AnglesAprErr[0]<< " " <<this->AnglesAprErr[1] << " " <<this->AnglesAprErr[2] << "\n";
		IDA << "angles_obs_err " <<fixed<<setprecision(4)<<this->AnglesObsErr[0]<< " " <<this->AnglesObsErr[1] << " " <<this->AnglesObsErr[2] << "\n";
		IDA << "angles_adj_err "<<fixed<<setprecision(4)<<this->AnglesAdjErr[0]<<" "<<this->AnglesAdjErr[1]<<" "<<this->AnglesAdjErr[2]<<"\n";

		IDA << "coords_adjust " <<CoordsAdjust[0] << " " <<CoordsAdjust[1] << " " <<CoordsAdjust[2] << "\n";
		IDA << "coords_apr " <<fixed<<setprecision(4)<<this->CoordsApr[0] << " " <<this->CoordsApr[1] << " " <<this->CoordsApr[2] << "\n";
		IDA << "coords_obs " <<fixed<<setprecision(4)<<this->CoordsObs[0] << " " <<this->CoordsObs[1] << " " <<this->CoordsObs[2] << "\n";
		IDA << "coords_adj " <<fixed<<setprecision(4)<<this->CoordsAdj[0] << " " <<this->CoordsAdj[1] << " " <<this->CoordsAdj[2] << "\n";
		IDA << "coords_apr_err "<<fixed<<setprecision(4)<<this->CoordsAprErr[0]<< " "<<this->CoordsAprErr[1] << " " <<this->CoordsAprErr[2] << "\n";
		IDA << "coords_obs_err "<<fixed<<setprecision(4)<<this->CoordsObsErr[0]<<" "<<this->CoordsObsErr[1]<<" "<<this->CoordsObsErr[2]<<"\n";
		IDA << "coords_adj_err "<<fixed<<setprecision(4)<<this->CoordsAdjErr[0]<<" "<<this->CoordsAdjErr[1]<<" "<<this->CoordsAdjErr[2]<<"\n";

		IDA << "quaternion " <<fixed<<setprecision(12)<< this->Quaternions[0] << " " <<this->Quaternions[1] << " " <<this->Quaternions[2] << " "<< this->Quaternions[3] <<"\n";
		IDA << "rotation_matrix ";
		for (int i=0; i<8; i++)
		{
			IDA <<fixed<<setprecision(12)<<this->R[i] << " ";
		}
		IDA <<fixed<<setprecision(12)<<this->R[8] << "\n";
		IDA << "DLT ";
		for (int i=0; i<10; i++)
		{
			IDA <<fixed<<setprecision(10)<<this->DLT[i] << " ";
		}
		IDA <<fixed<<setprecision(10)<<this->DLT[10] << "\n";
		IDA.close();
		return 1;
	}
	else
	{
		IDA.close();
		return 0;
	}
}

short Image::ReadFromFile(const char * Filename)
{
	ifstream Plik;
	Plik.open(Filename);
	if (Plik)
	{
		char Linia[400];
		char slowo[200];
		for (int i=1; i<=40; i++) //40 linijek jest w pliku
		{
			Plik.getline(Linia,400,'\n');
			string wiersz(Linia);
			istringstream LiniaPliku(wiersz);
			switch (i)
			{
			case 1:
				this->ImageFileName = Linia;
				break;
			case 2:
				this->Name = Linia;
				break;
			case 3:
				this->CamFileName = Linia;
				break;
			case 4:
				LiniaPliku >> slowo >> W >> H;
				break;
			case 5:
				LiniaPliku >> slowo >> this->NPoints;				
				break;
			case 6:
				LiniaPliku >> slowo >> Transf_x[0] >> Transf_x[1] >>Transf_x[2] >>Transf_x[3];				
				break;
			case 7:
				LiniaPliku >> slowo >> Transf_y[0] >> Transf_y[1] >>Transf_y[2] >>Transf_y[3];				
				break;
			case 8:
				LiniaPliku >> slowo >> InteriorAdjust[0] >>InteriorAdjust[1] >>InteriorAdjust[2];
				break;
			case 9:
				LiniaPliku >> slowo >> Interior[0] >>Interior[1] >>Interior[2];
				break;
			case 10:
				LiniaPliku >> slowo >> Apriori_IErr[0] >>Apriori_IErr[1] >>Apriori_IErr[2];
				break;
			case 11:
				LiniaPliku >> slowo >> Aposteriori_IErr[0] >>Aposteriori_IErr[1] >>Aposteriori_IErr[2];
				break;
			case 12:
				LiniaPliku >> slowo >> kAdjust[0] >>kAdjust[1] >>kAdjust[2];
				break;
			case 13:
				LiniaPliku >> slowo >> k[0] >>k[1] >>k[2];
				break;			
			case 14:
				LiniaPliku >> slowo >> Apriori_kErr[0] >>Apriori_kErr[1]>>Apriori_kErr[2];
				break;				
			case 15:
				LiniaPliku >> slowo >> Aposteriori_kErr[0] >>Aposteriori_kErr[1]>>Aposteriori_kErr[2];
				break;
			case 16:
				LiniaPliku >> slowo >> pAdjust[0] >>pAdjust[1];
				break;	
			case 17:
				LiniaPliku >> slowo >> p[0] >>p[1];
				break;	
			case 18:
				LiniaPliku >> slowo >> Apriori_pErr[0] >>Apriori_pErr[1];
				break;				
			case 19:
				LiniaPliku >> slowo >> Aposteriori_pErr[0] >>Aposteriori_pErr[1];
				break;
			case 20:
				LiniaPliku >> slowo >> AddAdjust[0] >>AddAdjust[1];
				break;
			case 21:
				LiniaPliku >> slowo >> Add[0] >>Add[1];
				break;
			case 22:
				LiniaPliku >> slowo >> Apriori_AddErr[0] >>Apriori_AddErr[1];
				break;				
			case 23:
				LiniaPliku >> slowo >> Aposteriori_AddErr[0] >>Aposteriori_AddErr[1];
				break;
			case 24:
				LiniaPliku >> slowo >> this->AngularSystem;
				break;
			case 25:
				LiniaPliku >> slowo >> AnglesAdjust[0] >> AnglesAdjust[1] >> AnglesAdjust[2];
				break;
			case 26:
				LiniaPliku >> slowo >> AnglesApr[0] >> AnglesApr[1] >> AnglesApr[2];
				break;
			case 27:
				LiniaPliku >> slowo >> AnglesObs[0] >> AnglesObs[1] >> AnglesObs[2];
				break;
			case 28:
				LiniaPliku >> slowo >> AnglesAdj[0] >> AnglesAdj[1] >> AnglesAdj[2];
				break;
			case 29:
				LiniaPliku >> slowo >> AnglesAprErr[0] >> AnglesAprErr[1] >> AnglesAprErr[2];
				break;
			case 30:
				LiniaPliku >> slowo >> AnglesObsErr[0] >> AnglesObsErr[1] >> AnglesObsErr[2];
				break;
			case 31:
				LiniaPliku >> slowo >> AnglesAdjErr[0] >> AnglesAdjErr[1] >> AnglesAdjErr[2];
				break;
			case 32:
				LiniaPliku >> slowo >> CoordsAdjust[0] >> CoordsAdjust[1] >> CoordsAdjust[2];
				break;
			case 33:
				LiniaPliku >> slowo >> CoordsApr[0] >> CoordsApr[1] >> CoordsApr[2];
				break;
			case 34:
				LiniaPliku >> slowo >> CoordsObs[0] >> CoordsObs[1] >> CoordsObs[2];
				break;
			case 35:
				LiniaPliku >> slowo >> CoordsAdj[0] >> CoordsAdj[1] >> CoordsAdj[2];
				break;
			case 36:
				LiniaPliku >> slowo >> CoordsAprErr[0] >> CoordsAprErr[1] >> CoordsAprErr[2];
				break;
			case 37:
				LiniaPliku >> slowo >> CoordsObsErr[0] >> CoordsObsErr[1] >> CoordsObsErr[2];
				break;
			case 38:
				LiniaPliku >> slowo >> CoordsAdjErr[0] >> CoordsAdjErr[1] >> CoordsAdjErr[2];
				break;
			case 39:
				LiniaPliku >> slowo >> Quaternions[0] >> Quaternions[1] >> Quaternions[2] >> Quaternions[3];
				break;
			case 40:
				LiniaPliku >> slowo >> R[0] >> R[1] >> R[2] >> R[3] >> R[4] >> R[5] >> R[6] >> R[7] >> R[8];
				break;
			case 41:
				LiniaPliku >> slowo >> DLT[0] >> DLT[1] >> DLT[2] >> DLT[3] >> DLT[4] >> DLT[5] >> DLT[6] >> DLT[7] >> DLT[8] >> DLT[9] >> DLT[10];
				break;
			}
		}
		HasDist = checkDist(); //Sprawdzenie czy obraz ma dystorsje
		Plik.close();
		//wypisz();
		return 1;

	}
	else
	{
		Plik.close();		
		return 0;

	}

}

void Image::wypisz()
{
	cout << ImageFileName << "\n";
	cout << Name << "\n";
	cout << CamFileName <<"\n";
	cout << "size: "<<W<<" "<<H<<"\n";
	cout << "number_of_image_points "<<this->NPoints<<"\n";
	
	cout << "transf_x: "<<fixed<<setprecision(8)<<this->Transf_x[0]<<" "<<this->Transf_x[1]<<" "<<this->Transf_x[2]<<" "<<this->Transf_x[3]<<"\n";
	cout << "transf_y: "<<fixed<<setprecision(8)<<this->Transf_y[0]<<" "<<this->Transf_y[1]<<" "<<this->Transf_y[2]<<" "<<this->Transf_y[3]<<"\n";

	cout << "interiorAdjust "<<InteriorAdjust[0]<<" "<<InteriorAdjust[1]<<" "<<InteriorAdjust[2]<<"\n";
	cout << "interior "<<fixed<<setprecision(3)<<this->Interior[0] <<" "<<this->Interior[1]<<" "<<this->Interior[2] <<"\n";
	cout << "interior_apriori_errors "<<fixed<<setprecision(3)<<this->Apriori_IErr[0] <<" "<<this->Apriori_IErr[1] <<" "<<this->Apriori_IErr[2] <<"\n";
	cout << "interior_aposteriori_errors "<<fixed<<setprecision(3)<<this->Aposteriori_IErr[0] <<" "<<this->Aposteriori_IErr[1] <<" "<<this->Aposteriori_IErr[2] <<"\n";	
	
	cout << "radial_distortion_adjust "<<kAdjust[0]<<" "<<kAdjust[1]<<" "<<kAdjust[2]<<"\n";
	cout << "radial_distortion " <<scientific<<setprecision(4)<<this->k[0] <<" " <<this->k[1] <<" " <<this->k[2] <<" " << "\n";
	cout << "radial_distortion_apriori_errors "<<scientific<<setprecision(4)<<this->Apriori_kErr[0] <<" " <<this->Apriori_kErr[1] <<" " <<this->Apriori_kErr[2] <<" " <<"\n";
	cout << "radial_distortion_aposteriori_errors "<<scientific<<setprecision(4)<<this->Aposteriori_kErr[0] <<" " <<this->Aposteriori_kErr[1] <<" " <<this->Aposteriori_kErr[2] <<" " <<"\n";	
	
	cout << "tangential_distortion_adjust "<<pAdjust[0]<<" "<<pAdjust[1]<<"\n";
	cout << "tangential_distortion " <<scientific<<setprecision(4)<<this->p[0] <<" "<<this->p[1] <<"\n";
	cout << "tangential_distortion_apriori_errors " <<scientific<<setprecision(4)<<this->Apriori_pErr[0] <<" "<<this->Apriori_pErr[1] <<"\n";
	cout << "tangential_distortion_aposteriori_errors " <<scientific<<setprecision(4)<<this->Aposteriori_pErr[0] <<" "<<this->Aposteriori_pErr[1] <<"\n";

	cout << "add_param_adjust "<<AddAdjust[0]<<" "<<AddAdjust[1]<<"\n";
	cout << "add_param " <<scientific<<setprecision(4)<<this->Add[0] <<" "<<this->Add[1] <<"\n";
	cout << "add_param_apriori_errors "<<scientific<<setprecision(4)<<this->Apriori_AddErr[0]<<" " <<this->Apriori_AddErr[1]<<"\n";
	cout << "add_param_aposteriori_errors "<<scientific<<setprecision(4)<<this->Aposteriori_AddErr[0]<<" " <<this->Aposteriori_AddErr[1]<<"\n";

	cout << "rotation " << this->AngularSystem <<"\n";
	cout << "angles_adjust " <<AnglesAdjust[0] << " " <<AnglesAdjust[1] << " " <<AnglesAdjust[2] << "\n";
	cout << "angles_apr " <<fixed<<setprecision(4)<<this->AnglesApr[0] << " " <<this->AnglesApr[1] << " " <<this->AnglesApr[2] << "\n";
	cout << "angles_obs " <<fixed<<setprecision(4)<<this->AnglesObs[0] << " " <<this->AnglesObs[1] << " " <<this->AnglesObs[2] << "\n";	
	cout << "angles_adj " <<fixed<<setprecision(4)<<this->AnglesAdj[0] << " " <<this->AnglesAdj[1] << " " <<this->AnglesAdj[2] << "\n";
	cout << "angles_apr_err " <<fixed<<setprecision(4)<<this->AnglesAprErr[0] << " " <<this->AnglesAprErr[1] << " " <<this->AnglesAprErr[2] << "\n";
	cout << "angles_obs_err " <<fixed<<setprecision(4)<<this->AnglesObsErr[0] << " " <<this->AnglesObsErr[1] << " " <<this->AnglesObsErr[2] << "\n";	
	cout << "angles_adj_err "<<fixed<<setprecision(4)<<this->AnglesAdjErr[0]<<" "<<this->AnglesAdjErr[1]<<" "<<this->AnglesAdjErr[2]<<"\n";
	
	cout << "coords_adjust " <<CoordsAdjust[0]<<" "<<CoordsAdjust[1] <<" "<<CoordsAdjust[2] << "\n";
	cout << "coords_apr " <<fixed<<setprecision(4)<<this->CoordsApr[0] << " " <<this->CoordsApr[1] << " " <<this->CoordsApr[2] << "\n";
	cout << "coords_obs " <<fixed<<setprecision(4)<<this->CoordsObs[0] << " " <<this->CoordsObs[1] << " " <<this->CoordsObs[2] << "\n";	
	cout << "coords_adj " <<fixed<<setprecision(4)<<this->CoordsAdj[0] << " " <<this->CoordsAdj[1] << " " <<this->CoordsAdj[2] << "\n";
	cout << "coords_apr_err " <<fixed<<setprecision(4)<<this->CoordsAprErr[0] << " " <<this->CoordsAprErr[1] << " " <<this->CoordsAprErr[2] << "\n";
	cout << "coords_obs_err " <<fixed<<setprecision(4)<<this->CoordsObsErr[0] << " " <<this->CoordsObsErr[1] << " " <<this->CoordsObsErr[2] << "\n";	
	cout << "coords_adj_err "<<fixed<<setprecision(4)<<this->CoordsAdjErr[0]<<" "<<this->CoordsAdjErr[1]<<" "<<this->CoordsAdjErr[2]<<"\n";
	
	cout << "quaternion " <<fixed<<setprecision(7)<< this->Quaternions[0] << " " <<this->Quaternions[1] << " " <<this->Quaternions[2] << " "<< this->Quaternions[3] <<"\n";
	
	cout << "rotation_matrix ";
	for (int i=0; i<8; i++)
	{
		cout <<fixed<<setprecision(8)<<this->R[i] << " ";
	}
	cout <<fixed<<setprecision(8)<<this->R[8] << "\n";
	
	cout << "DLT ";
	for (int i=0; i<10; i++)
	{
		cout <<fixed<<setprecision(10)<<this->DLT[i] << " ";
	}
	cout <<fixed<<setprecision(10)<<this->DLT[10] << "\n";
}

void Image::setCamera(Camera kamera)
{
	W = kamera.W;
	H = kamera.H;
	Interior[0] = kamera.ck;
	Interior[1] = kamera.x0;
	Interior[2] = kamera.y0;
	k[0] = kamera.k1;
	k[1] = kamera.k2;
	k[2] = kamera.k3;
	p[0] = kamera.p1;
	p[1] = kamera.p2;
	Add[0] = kamera.y_scaling;
	Add[1] = kamera.skewness;
	HasDist = checkDist();
}

void Image::setRotationMatrix(CvMat *Rot, int flag, string Type)
{
	if (flag==0)
	{
		fT_rot2angles(Rot, AnglesAdj, AngularSystem);
	}
	else
	{
		fT_rot2angles(Rot, AnglesApr, AngularSystem);
	}
	fT_rot2quaternions(Rot, Quaternions);
	fT_rot2vector(Rot,R);
	if (Type!="")
	{
		for (int i=0; i<3; i++)
		{
			AnglesAdjust[i] = true;
		}
	}
}

void Image::setQuaternions(double *Q, int flag, string Type)
{
	CvMat* Rot = cvCreateMat(3,3,CV_64FC1);
	fT_quaternions2rot(Q, Rot);
	if (flag==0)
	{
		fT_rot2angles(Rot, AnglesAdj, AngularSystem);
	}
	else
	{
		fT_rot2angles(Rot, AnglesApr, AngularSystem);
	}
	fT_rot2vector(Rot,R);
	for(int i=0; i<4; i++)
	{
		Quaternions[i] = Q[i];
	}
	if (Type!="")
	{
		for (int i=0; i<3; i++)
		{
			AnglesAdjust[i] = true;
		}
	}
	cvReleaseMat(&Rot);
}

void Image::setRotationMatrix(double *_R)
{
	for(int i=0; i<9; i++)
	{
		R[i] = _R[i];
	}
	fT_rot2angles(_R, AnglesAdj, AngularSystem);
	fT_rot2quaternions(_R,Quaternions);
}

void Image::setRotationMatrix(vector <double> *_R)
{
	for(int i=0; i<9; i++)
	{
		R[i] = (*_R)[i];
	}
	fT_rot2angles(R, AnglesAdj, AngularSystem);
	fT_rot2quaternions(R,Quaternions);
}

void Image::setQuaternions(double *Q)
{
	for(int i=0; i<4; i++)
	{
		Quaternions[i] = Q[i];
	}
	fT_quaternions2rot(Quaternions,R);
	fT_rot2angles(R, AnglesAdj, AngularSystem);
}

void Image::setAnglesApr(double *Angles)
{
	// Ustawiamy wartosci katow
	for (int i=0; i<3; i++)
	{
		AnglesApr[i] = Angles[i];
	}
}

void Image::setAnglesObs(double *Angles)
{
	// Ustawiamy wartosci katow
	for (int i=0; i<3; i++)
	{
		AnglesObs[i] = Angles[i];
	}
}

void Image::setAnglesAdj(double *Angles)
{
	// Ustawiamy wartosci katow
	for (int i=0; i<3; i++)
	{
		AnglesAdj[i] = Angles[i];
	}
	//Poniewaz sa to wartosci traktowane jako wyrownane
	//Zapisujemy rowniez kwaternion, macierz R i DLT
	fT_angles2rot(Angles,R,AngularSystem);
	fT_rot2quaternions(R,Quaternions);
	setDLT();
}


void Image::setCoordsApr(double *Coords)
{
	for (int i=0; i<3; i++)
	{
		CoordsApr[i] = Coords[i];
	}
}

void Image::setCoordsObs(double *Coords)
{
	for (int i=0; i<3; i++)
	{
		CoordsObs[i] = Coords[i];
	}
}

void Image::setCoordsAdj(double *Coords)
{
	for (int i=0; i<3; i++)
	{
		CoordsAdj[i] = Coords[i];
	}
	setDLT();
}

void Image::setCoordsAdj(TerrainPoint *Coords)
{
	CoordsAdj[0] = Coords->X;
	CoordsAdj[1] = Coords->Y;
	CoordsAdj[2] = Coords->Z;
	setDLT();
}

//Settery do bledow

void Image::setAnglesAprErr(double *AnglesErr)
{
	for (int i=0; i<3; i++)
	{
		AnglesAprErr[i] = AnglesErr[i];
	}
}

void Image::setAnglesObsErr(double *AnglesErr)
{
	for (int i=0; i<3; i++)
	{
		AnglesObsErr[i] = AnglesErr[i];
	}
}

void Image::setAnglesAdjErr(double *AnglesErr)
{
	for (int i=0; i<3; i++)
	{
		AnglesAdjErr[i] = AnglesErr[i];
	}
	
}


void Image::setCoordsAprErr(double *CoordsErr)
{
	for (int i=0; i<3; i++)
	{
		CoordsAprErr[i] = CoordsErr[i];
	}
}

void Image::setCoordsObsErr(double *CoordsErr)
{
	for (int i=0; i<3; i++)
	{
		CoordsObsErr[i] = CoordsErr[i];
	}
}

void Image::setCoordsAdjErr(double *CoordsErr)
{
	for (int i=0; i<3; i++)
	{
		CoordsAdjErr[i] = CoordsErr[i];
	}
}


void Image::setDLT(void)
{
	fT_orientation2dlt(Interior, CoordsAdj, AnglesAdj, AngularSystem, DLT);
}

short Image::checkDist(void)
{
	double sum = k[0]*k[0] + k[1]*k[1] + k[2]*k[2] + p[0]*p[0] + p[1]*p[1] + Add[0]*Add[0] + Add[1]*Add[1];
	if (sum == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
