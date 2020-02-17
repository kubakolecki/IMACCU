#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Menu_Item.H>
#include "ProjectNewWindow.h"
#include "ProjectOpenWindow.h"
#include "ProjectEditWindow.h"
#include "ImagesImportWindow.h"
#include "ImagesShowWindow.h"
#include "ImageCameraWindow.h"
#include "PointImportWindow.h"
#include "ChessboardDeffineWindow.h"
#include "ImPointImportWindow.h"
#include "PointDeleteWindow.h"
#include "PointExpBATWindow.h"
#include "TerrainPoint.h"
#include "EoImportWindow.h"
#include "EoExportWindow.h"
#include "GraphicsWindow.h"
#include "StereoModeWindow.h"
#include "OpenCVCalibWindow.h"
#include "Transf3DWindow.h"
#include "ChessboardDetectWindow.h"
#include "HelpWindow.h"
#include <string>
#include <vector>
using namespace std;
class MainWindow :
	public Fl_Window
{
public:
	string Version;
	Fl_Menu_Bar *menu;
	ProjectNewWindow * OknoNowyProjekt;
	ProjectOpenWindow * OknoOtworzProjekt;
	ProjectEditWindow * OknoEdycjiProjektu;
	ImagesImportWindow * OknoImportObrazow;
	ImagesShowWindow * OknoWyswietlaniaObrazow;
	ImagesShowWindow * OknoWyswietlaniaObrazow_2;
	ImageCameraWindow * OknoZmianyKamery;
	PointImportWindow *OknoImportuPunktow;
	ChessboardDeffineWindow *OknoDefinicjiSzachownicy;
	PointExpBATWindow *OknoExportuBAT;
	ImPointImportWindow *OknoImportuImPoint;
	PointDeleteWindow *OknoUsuwaniaPunktow;
	EoImportWindow *OknoImportuEO;
	EoExportWindow *OknoExportuEO;
	GraphicsWindow *OknoGrafiki;
	OpenCVCalibWindow *OknoKalibOpenCV;
	Transf3DWindow *OknoTransformacji3D;
	StereoModeWindow * OknoUstawien3D;
	ChessboardDetectWindow *OknoChessboardDetect;
	HelpWindow *OknoPomocy;
	WarningWindow *LicenceWindow;
	WarningWindow* RedundantPointsWindow;
	WarningWindow* SinglePointsWindow;
	WarningWindow* DeleteSinglePointsWindow;
	vector <TerrainPoint> GC;
	int NGCP;
	//vector< vector<DrawingNode> >::iterator DispDrw_it; //Wspolrzedne 3D rysowanych linii rysunkow wyswietlanych
	string CurrentProjectName;
	string ImageFolderName;
	string ProjectFolder;
	string ActiveDrawing; //nazwa aktywnego rysunku
	int ActiveDrawingNum; //numer aktywnego rysunku
	vector <int> DispDrawingNums; //numery aktywnego rysunku

	MainWindow(int w, int h, const char* title = "IMACCU");

	~MainWindow(void);
private:
	static void cb_MenuProjectNew(Fl_Widget *w, void *v);
	inline void cb_MenuProjectNew_i();
	
	static void cb_MenuProjectOpen(Fl_Widget *w, void *v);
	inline void cb_MenuProjectOpen_i();

	static void cb_MenuProjectEdit(Fl_Widget *w, void *v);
	inline void cb_MenuProjectEdit_i();

	static void cb_MenuImagesImport(Fl_Widget *w, void *v);
	inline void cb_MenuImagesImport_i();

	static void cb_MenuImagesCamera(Fl_Widget *w, void *v);
	inline void cb_MenuImagesCamera_i();

	static void cb_MenuImagesDisplayStereo(Fl_Widget *w, void *v);
	inline void cb_MenuImagesDisplayStereo_i();

	static void cb_MenuImagesDisplayStereo_2(Fl_Widget *w, void *v);
	inline void cb_MenuImagesDisplayStereo_2_i();

	static void cb_MenuPointsImportGCP(Fl_Widget *w, void *v);
	inline void cb_MenuPointsImportGCP_i();

	static void cb_MenuDeffineChessboard(Fl_Widget *w, void *v);
	inline void cb_MenuDeffineChessboard_i();
	
	static void cb_MenuPointsImportImPoints(Fl_Widget *w, void *v);
	inline void cb_MenuPointsImportImPoints_i();
	
	static void cb_MenuPointsExportBAT(Fl_Widget *w, void *v);
	inline void cb_MenuPointsExportBAT_i();

	static void cb_MenuPointsCheckRedundant(Fl_Widget *w, void *v);
	inline void cb_MenuPointsCheckRedundant_i();

	static void cb_MenuPointsDelete(Fl_Widget *w, void *v);
	inline void cb_MenuPointsDelete_i();

	static void cb_MenuImagesExportEO(Fl_Widget *w, void *v);
	inline void cb_MenuImagesExportEO_i();

	static void cb_MenuSolveOpenCVCalibration(Fl_Widget *w, void *v);
	inline void cb_MenuSolveOpenCVCalibration_i();

	static void cb_MenuSolve3DTransf(Fl_Widget *w, void *v);
	inline void cb_MenuSolve3DTransf_i();

	static void cb_MenuToolsChessboardDetect(Fl_Widget *w, void *v);
	inline void cb_MenuToolsChessboardDetect_i();

	static void cb_MenuSettingsGraphics(Fl_Widget *w, void *v);
	inline void cb_MenuSettingsGraphics_i();

	static void cb_MenuSettingsStereo(Fl_Widget *w, void *v);
	inline void cb_MenuSettingsStereo_i();

	static void cb_MenuHelpAbout(Fl_Widget *w, void *v);
	inline void cb_MenuHelpAbout_i();

	static void cb_MenuHelpLicense(Fl_Widget *w, void *v);
	inline void cb_MenuHelpLicense_i();
};
