#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Button.H>

class ImagesImportWindow : public Fl_Window
{
public:
	Fl_Multi_Browser *Browser_Images;
	Fl_Input_Choice *Input_Camera;
	Fl_Input_Choice *Input_Orientation;
	Fl_Button *Button_Import;
	Fl_Button *Button_Cancel;
	void *win;

	ImagesImportWindow(int w, int h, const char* title);
	~ImagesImportWindow(void);
private:
	static void cb_Input_Camera (Fl_Widget *w, void *v);
	inline void cb_Input_Camera_i();
	
	static void cb_Button_Cancel (Fl_Widget *w, void *v);
	inline void cb_Button_Cancel_i();

	static void cb_Button_Import (Fl_Widget *w, void *v);
	inline void cb_Button_Import_i();
};
