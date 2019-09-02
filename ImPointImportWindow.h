#pragma once
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_File_Chooser.H>

class ImPointImportWindow  : public Fl_Window
{
public:
	ImPointImportWindow(int w, int h, const char* title);
	~ImPointImportWindow(void);
	void * win; // Wskaznik do okna glownego
	Fl_Button *Button_Browse;
	Fl_Output *Output_File;
	Fl_Check_Button *Check_Accuracy;
	Fl_Float_Input *Input_AccuracyX;
	Fl_Float_Input *Input_AccuracyY;
	Fl_Button *Button_Execute;
	Fl_File_Chooser *Chooser_InputFile;

private:
	static void cb_Button_Browse(Fl_Widget *w, void *v);
	inline void cb_Button_Browse_i();

	static void cb_Check_Accuracy(Fl_Widget *w, void *v);
	inline void cb_Check_Accuracy_i();

	static void cb_Chooser_InputFile(Fl_File_Chooser *w, void *v);
	inline void cb_Chooser_InputFile_i();

	static void cb_Button_Execute(Fl_Widget *w, void *v);
	inline void cb_Button_Execute_i();
};
