#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Input_Choice.H>

class EoExportWindow : public Fl_Window
{
public:
	void * win; // Wskaznik do okna glownego
	Fl_Multi_Browser * Browser_Images;
	Fl_Input *Input_File;
	Fl_Button *Button_Export;
	Fl_Round_Button *RB_Export_Apr;
	Fl_Round_Button *RB_Export_Obs;
	Fl_Round_Button *RB_Export_Adj;
	Fl_Input_Choice *Input_Rotation;
	Fl_Check_Button *ChB_Export_Errors;
	Fl_Check_Button *ChB_Bat_Export;
	EoExportWindow(int w, int h, const char* title);
	~EoExportWindow(void);

private:

static void cb_Button_Export(Fl_Widget *w, void *v);
inline void cb_Button_Export_i();

static void cb_RB_Export_Apr(Fl_Widget *w, void *v);
inline void cb_RB_Export_Apr_i();

static void cb_RB_Export_Obs(Fl_Widget *w, void *v);
inline void cb_RB_Export_Obs_i();

static void cb_RB_Export_Adj(Fl_Widget *w, void *v);
inline void cb_RB_Export_Adj_i();

static void cb_Input_Rotation(Fl_Widget *w, void *v);
inline void cb_Input_Rotation_i();

static void cb_ChB_Export_Errors(Fl_Widget *w, void *v);
inline void cb_ChB_Export_Errors_i();

};
