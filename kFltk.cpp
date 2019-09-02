#include "pch.h"
#include <filesystem>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Input_Choice.H>

using namespace std;

void fillbrowser(const char *filename, const char *mask, Fl_Browser *browser)
{
	//Funkcja sluzy do zapelnienia widgetu Fl_Select_Browser nazwami plikow
	//ze spisu plikow wygenerowanych komenda systemowa.
	//Mask - maska rozszerzen - ktore rozszerzenia maja byc uwzgledniane
	//Przykladowy format: ".tif.jpg.png"
	char linijka[200];
	string Maska(mask);
	int IleRozszerzen = 0;
	for (int i=0; i < Maska.length(); i++) //Liczymy ile rozszerzen wpisanych jest do maski
	{
		if (Maska.at(i) == '.') IleRozszerzen++;
	}
	string *Rozszerzenia;
	Rozszerzenia = new string[IleRozszerzen]; //Dynamiczna tablica rozszerzen
	
	int k=0;
	for (int i=0; i < Maska.length(); i++)
	{
		if (Maska.at(i) == '.') 
		{
			Rozszerzenia[k] = Maska.substr(i,4);
			k++;
		}
	}
	//cout<<"IleRozszerzen:"<<IleRozszerzen<<endl;
	
	//for (int i=0; i < IleRozszerzen; i++) cout<<Rozszerzenia[i]<<endl;
	
	ifstream plik;
	plik.open(filename);
	while(plik.getline(linijka,200,'\n'))
	{
		string Linia(linijka);
		for (int i=0; i < IleRozszerzen; i++)
		{
			int PozycjaRozszerzenia = Linia.find(Rozszerzenia[i],0);
			int PozycjaSpacji = Linia.rfind(" ",Linia.length());
			if (PozycjaRozszerzenia > 0)
			{
				string NazwaPliku = Linia.substr(PozycjaSpacji+1, PozycjaRozszerzenia-PozycjaSpacji+3);
				browser->add(NazwaPliku.c_str());
			}
		}
	}
	
	delete [] Rozszerzenia;
	plik.close();
}

void fillbrowserfs(const string& directory, string&& mask, Fl_Browser * browser)
{
	vector<string> ext;
	for (int i = 0; i < mask.length(); i++)
	{
		if (mask.at(i) == '.')
		{
			ext.push_back(mask.substr(i, 4));
		}
	}

	
	for (auto& f : std::filesystem::directory_iterator(directory))
	{
		std::string s = f.path().string();
		int l = s.length();
		if (l > 5)
		{
			int p = s.rfind("\\");
			if (p != std::string::npos)
			{
				std::string filename = s.substr(p + 1, l - p - 1);
				if (filename.length() > 4)
				{
					std::string extension = filename.substr(filename.length() - 4, 4);
					for (auto& e : ext)
					{
						if (extension == e)
						{
							browser->add(filename.c_str());
							break;
						}
					}
				}
			}
		}
	}

}

void fillinput(const char *filename, const char *mask, Fl_Input_Choice *input)
{
	//Funkcja sluzy do zapelnienia widgetu Fl_Select_Browser nazwami plikow
	//ze spisu plikow wygenerowanych komenda systemowa.
	//Mask - maska rozszerzen - ktore rozszerzenia maja byc uwzgledniane
	//Przykladowy format: ".tif.jpg.png"
	char linijka[200];
	string Maska(mask);
	int IleRozszerzen = 0;
	for (int i = 0; i < Maska.length(); i++) //Liczymy ile rozszerzen wpisanych jest do maski
	{
		if (Maska.at(i) == '.') IleRozszerzen++;
	}
	string *Rozszerzenia;
	Rozszerzenia = new string[IleRozszerzen]; //Dynamiczna tablica rozszerzen

	int k = 0;
	for (int i = 0; i < Maska.length(); i++)
	{
		if (Maska.at(i) == '.') Rozszerzenia[k] = Maska.substr(i, 4);
		k++;
	}

	//for (int i=0; i < IleRozszerzen; i++) cout<<Rozszerzenia[i]<<endl;

	ifstream plik;
	plik.open(filename);
	while (plik.getline(linijka, 200, '\n'))
	{
		string Linia(linijka);
		for (int i = 0; i < IleRozszerzen; i++)
		{
			int PozycjaRozszerzenia = Linia.find(Rozszerzenia[i], 0);
			int PozycjaSpacji = Linia.rfind(" ", Linia.length());
			if (PozycjaRozszerzenia > 0)
			{
				string NazwaPliku = Linia.substr(PozycjaSpacji + 1, PozycjaRozszerzenia - PozycjaSpacji + 3);
				input->add(NazwaPliku.c_str());
			}
		}
	}

	delete[] Rozszerzenia;
	plik.close();
}

void fillinputfs(const string & directory, string && mask, Fl_Input_Choice * input_choice)
{
	vector<string> ext;
	for (int i = 0; i < mask.length(); i++)
	{
		if (mask.at(i) == '.')
		{
			ext.push_back(mask.substr(i, 4));
		}
	}


	for (auto& f : std::filesystem::directory_iterator(directory))
	{
		std::string s = f.path().string();
		int l = s.length();
		if (l > 5)
		{
			int p = s.rfind("\\");
			if (p != std::string::npos)
			{
				std::string filename = s.substr(p + 1, l - p - 1);
				if (filename.length() > 4)
				{
					std::string extension = filename.substr(filename.length() - 4, 4);
					for (auto& e : ext)
					{
						if (extension == e)
						{
							input_choice->add(filename.c_str());
							break;
						}
					}
				}
			}
		}
	}

}

void getfilenames(const char* filename, vector <string> *names, const char *mask)
{
	//Funkcja sluzy do budowy wektora (names) z nazwami plikow
	char linijka[200];
	string Maska(mask);
	int IleRozszerzen = 0;
	for (int i=0; i < Maska.length(); i++) //Liczymy ile rozszerzen wpisanych jest do maski
	{
		if (Maska.at(i) == '.') IleRozszerzen++;
	}
	string *Rozszerzenia;
	Rozszerzenia = new string[IleRozszerzen]; //Dynamiczna tablica rozszerzen

	int k=0;
	for (int i=0; i < Maska.length(); i++)
	{
		if (Maska.at(i) == '.')
		{
			Rozszerzenia[k] = Maska.substr(i,4);
			k++;
		}
	}


	ifstream plik;
	plik.open(filename);
	while(plik.getline(linijka,200,'\n'))
	{
		string Linia(linijka);
		for (int i=0; i < IleRozszerzen; i++)
		{
			int PozycjaRozszerzenia = Linia.find(Rozszerzenia[i],0);
			int PozycjaSpacji = Linia.rfind(" ",Linia.length());
			if (PozycjaRozszerzenia > 0)
			{
				string NazwaPliku = Linia.substr(PozycjaSpacji+1, PozycjaRozszerzenia-PozycjaSpacji+3);
				names->push_back(NazwaPliku);
			}
		}
	}

	delete [] Rozszerzenia;
	plik.close();
}

void fillbrowser_Killian(const char *filename, Fl_Browser *browser, int Limit)
{
	//Funkcja sluzy do zapelnienia widgetu Fl_Select_Browser w oknie Killian_Window
	//ze spisu plikow wygenerowanych komenda systemowa.
	//Mask - maska rozszerzen - ktore rozszerzenia maja byc uwzgledniane
	//Stosujemy ".ida"
	//Limit - minimalna ilosc fotopunktow jaka ma byc na zdjeciach
	char linijka[200];
	string Maska = ".ida";
	Image * Obraz;
	string NazwaPliku(filename);
	int PozycjaBackslesha = NazwaPliku.rfind("\\");
	string NazwaFolderu = NazwaPliku.substr(0,PozycjaBackslesha+1);
	
	ifstream plik;
	plik.open(filename); //chodzi o plik prj.txt
	while(plik.getline(linijka,200,'\n'))
	{
		string Linia(linijka);
		int PozycjaRozszerzenia = Linia.find(Maska,0);
		int PozycjaSpacji = Linia.rfind(" ",Linia.length());
		if (PozycjaRozszerzenia > 0)
		{
			string NazwaPlikuIDA = Linia.substr(PozycjaSpacji+1, PozycjaRozszerzenia-PozycjaSpacji+3);
			string PelnaNazwaPlikuIDA = NazwaFolderu + NazwaPlikuIDA;
			Obraz = new Image;
			Obraz->ReadFromFile(PelnaNazwaPlikuIDA.c_str());
			int NGCP = 0;
			if (Obraz->NPoints >= Limit)
			{
				string s1 = NazwaPlikuIDA.substr(0,NazwaPlikuIDA.length() - 4);
				string s2 = ".pix";
				string PelnaNazwaPlikuPIX = NazwaFolderu + s1 + s2;
				ImagePoint P;
				P.StrumienDoOdczytu_L.open(PelnaNazwaPlikuPIX.c_str());
				for (int i=0; i<Obraz->NPoints + 1; i++)
				{
					P.load_L(i+1);
					if (P.Type == 3) NGCP++;
				}
				P.StrumienDoOdczytu_L.close();
			}

			delete Obraz;
			
			if (NGCP >=Limit)
			{
				browser->add(NazwaPlikuIDA.c_str());
			}
		}

	}
	
	plik.close();	
}

void fillvector(const char *filename, const char *mask, vector<string> *v)
{
	v->clear();
	char linijka[200];
	string Maska(mask);
	int IleRozszerzen = 0;
	for (int i=0; i < Maska.length(); i++) //Liczymy ile rozszerzen wpisanych jest do maski
	{
		if (Maska.at(i) == '.') IleRozszerzen++;
	}
	string *Rozszerzenia;
	Rozszerzenia = new string[IleRozszerzen]; //Dynamiczna tablica rozszerzen

	int k=0;
	for (int i=0; i < Maska.length(); i++)
	{
		if (Maska.at(i) == '.') Rozszerzenia[k] = Maska.substr(i,4);
		k++;
	}

	//for (int i=0; i < IleRozszerzen; i++) cout<<Rozszerzenia[i]<<endl;

	ifstream plik;
	plik.open(filename);
	while(plik.getline(linijka,200,'\n'))
	{
		string Linia(linijka);
		for (int i=0; i < IleRozszerzen; i++)
		{
			int PozycjaRozszerzenia = Linia.find(Rozszerzenia[i],0);
			int PozycjaSpacji = Linia.rfind(" ",Linia.length());
			if (PozycjaRozszerzenia > 0)
			{
				string NazwaPliku = Linia.substr(PozycjaSpacji+1, PozycjaRozszerzenia-PozycjaSpacji+3);
				v->push_back(NazwaPliku);
			}
		}
	}

	delete [] Rozszerzenia;
	plik.close();
}


void deactivateItem (Fl_Menu_Bar* menu,const char *Itemname)
{
	const Fl_Menu_Item *Item = menu->find_item(Itemname);
	Fl_Menu_Item *It = (Fl_Menu_Item*)Item;
	It->deactivate();
}
void activateItem (Fl_Menu_Bar* menu,const char *Itemname)
{
	const Fl_Menu_Item *Item = menu->find_item(Itemname);
	Fl_Menu_Item *It = (Fl_Menu_Item*)Item;
	It->activate();
}

void writeGCPToFile (string& filename, vector <TerrainPoint>& GC)
{
	ofstream StrToFile;
	StrToFile.open(filename);
	if (StrToFile)
	{
		for (auto  &p : GC)
		{
			StrToFile << p.Name <<" "<<fixed<< setprecision(4) <<p.X<<" "<<p.Y<<" "<<p.Z<<" ";
			StrToFile <<fixed<< setprecision(4) << p.Mx <<" "<<p.My<<" "<<p.Mz<<" "<<setprecision(0)<<p.Type<<" ";
			StrToFile << p.TypParametru<<" " <<p.Use <<endl;
		}
	}
	StrToFile.close();
}

int countGCP (const char *filename)
{
	ifstream StreamFromGCP;
	StreamFromGCP.open(filename);
	if (!StreamFromGCP)
	{
		//cout<<"Brak pliku z fotopunktami lub plik bledny" <<endl;
		StreamFromGCP.close();
		return 0;
	}
	else
	{
		string linia;
		int GCPNumber=0;
		while ( getline(StreamFromGCP,linia,'\n') ) //Analiza pliku z fotopunktami
		{
			if ( linia.length() > 10) //Zabezpieczenie przed pustymi liniami na koncu pliku
			{
				GCPNumber++;
			}
		}
		StreamFromGCP.close();
		return GCPNumber;
	}
}

int importGC(const char *filename, vector<TerrainPoint>* GC, int N)
{
	ifstream StreamFromGCP;
	StreamFromGCP.open(filename);
	if (!StreamFromGCP)
	{
		//cout<<"Brak pliku z fotopunktami lub plik bledny" <<endl;
		StreamFromGCP.close();
		return 1;
	}
	else
	{
		string linia;
		for (int i=0; i<N; i++)
		{
			getline(StreamFromGCP,linia,'\n');
			string GCPId; //Id fotopunktu
			double X, Y, Z; //Wspolrzedne fotopunktu
			double Mx, My, Mz; //Dokladnosc a'priori
			short Code; //Kod
			istringstream StreamFromLine(linia);
			StreamFromLine>>GCPId >> X >> Y >> Z >> Mx >> My >> Mz >> Code;
			TerrainPoint T(GCPId,X,Y,Z,Mx,My,Mz,Code);
			GC->push_back(T);
			StreamFromLine.clear();				
		}
		StreamFromGCP.close();
		return 0;
	}
}

string getTime()
{
	//Funkcja zwraca string z czasem w formacie: RRRRMMDD_GGMMSS
	time_t Czas;
	struct tm ts;
    char buf[80];
	Czas = time(NULL);
	//buf = asctime(ts&);
	//strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", ts);
	//string Wynik(asctime_s(localtime(&Czas)));
	ctime_s(buf, sizeof buf, &Czas );
	string Wynik = string(buf);
	return Wynik;
}

bool isEnySelected(Fl_Browser *browser)
{
	bool ret = false;
	for (int i=1; i<=browser->size(); i++)
	{
		if (browser->selected(i)) ret = true;
	}
	return ret;
}

string num2str(int num)
{
	stringstream ss;
    string str;
    ss << num;
    ss >> str;
	return str;
}


string num2str_d(double num, int prec)
{
	stringstream ss;
    string str;
    ss <<fixed<<setprecision(prec)<<num;
    ss >> str;
	return str;
}

int str2num(string str)
{
	stringstream ss;
    int num;
    ss << str;
    ss >> num;
	return num;
}

double str2num_d(string str)
{
	stringstream ss;
    double num;
    ss << str;
    ss >> num;
	return num;
}

void getImNames(string prjstr, vector<string>* Names)
{
	ifstream STR;
	STR.open(prjstr.c_str());
	string Linestr;
	while (STR)
	{
		getline(STR, Linestr,'\n');
		int PozycjaRozszerzenia = Linestr.find(".ida",0);
		int PozycjaSpacji = Linestr.rfind(" ",Linestr.length());
		if (PozycjaRozszerzenia > 0)
		{
			string NazwaPliku = Linestr.substr(PozycjaSpacji+1, PozycjaRozszerzenia-PozycjaSpacji+3);
			Names->push_back(NazwaPliku);
		}
	}
	STR.close();
}

void loadImagePointVector(string PixName, vector<ImagePoint>* PointVector)
{
	string N;
	string ImId;
	string LnId;
	double WspX;
	double WspY;
	double XErr;
	double YErr;
	short T;
	int x;
	ifstream PIXSTR;
	PIXSTR.open(PixName.c_str());
	while (PIXSTR)
	{
		PIXSTR >> N >>ImId >> LnId >> WspX >> WspY >> XErr >> YErr >> T >>x;
		if (PIXSTR)
		{
			ImagePoint P(N,ImId,LnId,WspX,WspY,XErr,YErr,T);
			PointVector->push_back(P);
			//cout <<"Wczytuje punkt: "<<N<<endl;
		}
	}
	PIXSTR.close();
}

void writeImagePointVector(string PixName, vector<ImagePoint>* P)
{
	//cout <<"Zapis do pliku: "<<PixName<<endl;
	ofstream PIXSTR;
	PIXSTR.open(PixName.c_str());
	for (int i=0; i<P->size(); i++)
	{
		PIXSTR <<setw(7)<<P->at(i).Name<<" "<<setw(15)<<P->at(i).ImageId<<" "<<setw(15)<<P->at(i).LineId<<" "<<fixed<< setprecision(2)<<setw(9)<<P->at(i).X<<" "<<setw(9)<<P->at(i).Y<<" "<<setw(5)<<P->at(i).Mx<<" "<<setw(5)<<P->at(i).My<<" "<<setw(3)<<P->at(i).Type << setw(2) << P->at(i).Active << endl;
	}
	PIXSTR.close();
}

void sortPositiveVector(vector<double> *V, vector<int> *Idx)
{

	//Idx->resize(V->size());
	vector<bool> WasSorted;
	WasSorted.resize(V->size(),false);
	while (Idx->size()<V->size())
	{
		double MaxValue = 0;
		int MinIdx = 0;
		for (int i=0; i<V->size(); i++)
		{
			if (((*V)[i]>MaxValue) && WasSorted[i]==false)
			{
				MaxValue = (*V)[i];
				MinIdx = i;
			}
		}
		Idx->push_back(MinIdx);
		WasSorted[MinIdx] = true;
	}


}
