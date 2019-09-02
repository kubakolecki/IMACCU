#pragma once
#include <FL/Fl_Browser.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Menu_Bar.H>
#include "TerrainPoint.h"
#include "ImagePoint.h"
#include <vector>
#include <string>
using namespace std;
void fillbrowser(const char *filename, const char *mask, Fl_Browser *browser);
void fillbrowserfs(const string& directory,string&& mask, Fl_Browser *browser);
void fillbrowser_Killian(const char *filename, Fl_Browser *browser, int Limit);
void fillbrowser_Absolute(const char *filename, Fl_Browser *browser, std::string *ProjectFolder, vector <TerrainPoint> *T, int NGCP);
void fillbrowser_Absolute_Model(const char *filename, Fl_Browser *browser, std::string *ProjectFolder,vector <TerrainPoint> *T, int NGCP);
void fillinput(const char *filename, const char *mask, Fl_Input_Choice *iput);
void fillinputfs(const string& directory, string&& mask, Fl_Input_Choice *browser);
void fillvector(const char *filename, const char *mask, vector<std::string> *v);
void getfilenames(const char* filename, vector <std::string> *names, const char *mask);
void deactivateItem (Fl_Menu_Bar* menu, const char *Itemname);
void activateItem (Fl_Menu_Bar* menu, const char *Itemname);
int countGCP (const char *filename); // Funkcja do importu fotopunktow z pliku do tablicy GCP
void writeGCPToFile (string& filename, vector <TerrainPoint>& GC); // Funkcja zapisujaca fotopunkty z tablicy GCP do pliku
//void addGCP(TerrainPoint ***GCP, int *N, TerrainPoint *T);
std::string getTime(void);
int importGC(const char *filename, vector<TerrainPoint>* GC, int N);
bool isEnySelected(Fl_Browser *browser); //Funkcja sprawdza czy na liscie jest jeszcze jakis element zaznaczony
std::string num2str(int num);
std::string num2str_d(double num, int prec);
int str2num(std::string str);
double str2num_d(std::string str);
void getImNames(std::string prjstr, vector<std::string>* Names);
void loadImagePointVector(std::string PixName, vector<ImagePoint>* PointVector);
void writeImagePointVector(std::string PixName, vector<ImagePoint>* PointVector);
void sortPositiveVector(vector<double>* V, vector <int>* Idx);
