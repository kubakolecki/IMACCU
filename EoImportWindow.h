#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Float_Input.H>

class EoImportWindow : public Fl_Window
{
public:
	void * win; // Wskaznik do okna glownego
	Fl_Multi_Browser *Browser_Images;
	Fl_Output *Output_File;
	Fl_Button *Button_Import;
	Fl_Button *Button_Browse;
	Fl_File_Chooser *Chooser_Plik;
	Fl_Check_Button *ChB_Approximated;
	Fl_Check_Button *ChB_Observed;
	Fl_Check_Button *ChB_Adjusted;
	Fl_Check_Button *ChB_X;
	Fl_Check_Button *ChB_Y;
	Fl_Check_Button *ChB_Z;
	Fl_Check_Button *ChB_Ang1;
	Fl_Check_Button *ChB_Ang2;
	Fl_Check_Button *ChB_Ang3;
	Fl_Check_Button *ChB_Error;
	Fl_Box *Box_Alignment;
	Fl_Box *Box_LeverArm;
	Fl_Input_Choice *Input_RotSystem;
	Fl_Float_Input *Input_R0;
	Fl_Float_Input *Input_R1;
	Fl_Float_Input *Input_R2;
	Fl_Float_Input *Input_R3;
	Fl_Float_Input *Input_R4;
	Fl_Float_Input *Input_R5;
	Fl_Float_Input *Input_R6;
	Fl_Float_Input *Input_R7;
	Fl_Float_Input *Input_R8;
	Fl_Float_Input *Input_lx;
	Fl_Float_Input *Input_ly;
	Fl_Float_Input *Input_lz;
	EoImportWindow(int w, int h, const char* title);
	~EoImportWindow(void);
private:
	static void cb_Button_Browse(Fl_Widget *w, void *v);
	inline void cb_Button_Browse_i();

	static void cb_Button_Import(Fl_Widget *w, void *v);
	inline void cb_Button_Import_i();

	static void cb_Chooser_Plik(Fl_File_Chooser *w, void *v);
	inline void cb_Chooser_Plik_i();

	static void cb_Input_RotSystem(Fl_Widget *w, void *v);
	inline void cb_Input_RotSystem_i();
};
