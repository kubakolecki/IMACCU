#include "pch.h"
#include "CameraSaveWindow.h"
#include "OpenCVCalibWindow.h"


CameraSaveWindow::CameraSaveWindow(int w, int h, const char* title) :Fl_Window(w, h, title)
{
	begin();
		Input_Name = new Fl_Input(120, 20, 240, 30, "Camera Name:");
		Input_Description = new Fl_Input(120, 70, 240, 30, "Description:");
		Button_Save = new Fl_Button(120, 120, 80, 30, "Save");
		Button_Cancel = new Fl_Button(220, 120, 80, 30, "Cancel");
		Chooser_File = new Fl_Native_File_Chooser();
	end();

	//callbacks
	Button_Save->callback(cb_Button_Save, this);
	Button_Cancel->callback(cb_Button_Cancel, this);

	//properties:
	Chooser_File->title("Save Camera");
	Chooser_File->type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	Chooser_File->filter("Camera\t*.cam");
	Chooser_File->options(Fl_Native_File_Chooser::SAVEAS_CONFIRM);
	this->set_modal();
}


CameraSaveWindow::~CameraSaveWindow()
{
}

void CameraSaveWindow::cb_Button_Save(Fl_Widget * w, void * v)
{
	((CameraSaveWindow*)v)->cb_Button_Save_i();
}

inline void CameraSaveWindow::cb_Button_Save_i()
{
	string name(Input_Name->value());
	string description(Input_Name->value());
	string filename;
	
	if (name == "" || description == "") return;

	string presentfile =  name + ".cam";
	Chooser_File->preset_file(presentfile.c_str());

	switch (Chooser_File->show())
	{
	case -1:
		break;
	case 1:
		break;
	default:
		filename = Chooser_File->filename();
		break;
	}

	((OpenCVCalibWindow*)win)->CalibratedCamera.Description = description;
	((OpenCVCalibWindow*)win)->CalibratedCamera.Name = name;
	((OpenCVCalibWindow*)win)->CalibratedCamera.writeToFile(filename);

	this->hide();
}

void CameraSaveWindow::cb_Button_Cancel(Fl_Widget * w, void * v)
{
	((CameraSaveWindow*)v)->cb_Button_Cancel_i();
}

inline void CameraSaveWindow::cb_Button_Cancel_i()
{
	this->hide();
}


	

