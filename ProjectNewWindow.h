#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <string>
#include <FL/Fl_Native_File_Chooser.H>
#include "WarningWindow.h"
using namespace std;

class ProjectNewWindow :
	public Fl_Window
{
public:
	ProjectNewWindow(int w, int h, const char* title);
	~ProjectNewWindow(void);
	Fl_Input *Input_ProjectName;
	Fl_Input *Input_ImageFolder;
	Fl_Button *Button_Ok;
	Fl_Button *Button_Select_Folder;
	Fl_Button *Button_Cancel;
	WarningWindow *Window_Warning_Bad_Folder;
	string NazwaProjektu;
	string FolderObrazow;
	Fl_Native_File_Chooser *Chooser_Folder;
	void *win; //Bardzo wazny wskaznik - okno glowne moze przez niego przekazac swoj wlasny adres


private:
	static void cb_Button_Cancel (Fl_Widget *w, void *v);
	inline void cb_Button_Cancel_i();

	static void cb_Button_Ok (Fl_Widget *w, void *v);
	inline void cb_Button_Ok_i();

	static void cb_Button_Select_Folder(Fl_Widget *w, void *v);
	inline void cb_Button_Select_Folder_i();

	static void cb_Input_ProjectName (Fl_Widget *w, void *v);
	inline void cb_Input_ProjectName_i();

	static void cb_Input_ImageFolder (Fl_Widget *w, void *v);
	inline void cb_Input_ImageFolder_i();

};
