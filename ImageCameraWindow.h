#pragma once
#include <FL/Fl_Window.H>

using namespace std;

class ImageCameraWindow : public Fl_Window
{
public:
	Fl_Multi_Browser *Browser_Images;
	Fl_Input_Choice *Input_Camera;
	Fl_Button *Button_Change;
	Fl_Button *Button_Cancel;
	void *win;
	ImageCameraWindow(int w, int h, const char* title);
	~ImageCameraWindow(void);

private:
	static void cb_Input_Camera (Fl_Widget *w, void *v);
	inline void cb_Input_Camera_i();

	static void cb_Button_Cancel (Fl_Widget *w, void *v);
	inline void cb_Button_Cancel_i();

	static void cb_Button_Change (Fl_Widget *w, void *v);
	inline void cb_Button_Change_i();
};
