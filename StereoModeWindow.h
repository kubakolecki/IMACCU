#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Round_Button.H>

class StereoModeWindow: public Fl_Window
{
public:
	void *win;
	Fl_Round_Button * RB_Two_Views;
	Fl_Round_Button * RB_Anaglyph;
	Fl_Round_Button * RB_3D_Glasses;
	StereoModeWindow(int w, int h, const char* title);
	~StereoModeWindow(void);
private:
	static void cb_RB_Two_Views(Fl_Widget *w, void *v);
	inline void cb_RB_Two_Views_i();

	static void cb_RB_Anaglyph(Fl_Widget *w, void *v);
	inline void cb_RB_Anaglyph_i();

	static void cb_RB_3D_Glasses(Fl_Widget *w, void *v);
	inline void cb_RB_3D_Glasses_i();

};
