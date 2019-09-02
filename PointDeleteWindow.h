/*
 * PointDeleteWindow.h
 *
 *  Created on: 4 lut 2014
 *      Author: Dell Latitude E6530
 */

#ifndef POINTDELETEWINDOW_H_
#define POINTDELETEWINDOW_H_

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multi_Browser.H>

class PointDeleteWindow  : public Fl_Window
{
public:
	PointDeleteWindow(int w, int h, const char* title);
	~PointDeleteWindow();

	void *win;
	Fl_Multi_Browser * Browser_Images;
	Fl_Button * Button_Delete;


private:
	static void cb_Button_Delete(Fl_Widget *w, void *v);
	inline void cb_Button_Delete_i();

};

#endif /* POINTDELETEWINDOW_H_ */
