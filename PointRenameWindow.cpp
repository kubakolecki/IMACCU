#include "pch.h"
#include "PointRenameWindow.h"


PointRenameWindow::PointRenameWindow(int w, int h, const char* title) :Fl_Window(w, h, title)
{
	begin();
		Output_Point_Name_Old = new Fl_Output(60,20,180,25,"Point ID:");
		Output_Point_Type_Old = new Fl_Output(320,20,80, 25, "Point Type:");
		Input_Point_Name_New = new Fl_Input(60,65,180,25, "New ID:");
		Output_Point_Type_New = new Fl_Output(320,65,80,25, "New Type:");
		Button_Ok = new Fl_Button(60,110,120,30, "OK");
		Button_Cancel = new Fl_Button(60,155, 120, 30, "Cancel");
	end();

	//callbacks:
	Button_Ok->callback(cb_Button_Ok, this);
	Button_Cancel->callback(cb_Button_Cancel, this);
	Input_Point_Name_New->callback(cb_Button_Input_Point_Name_New, this);

	Input_Point_Name_New->when(1);

}


PointRenameWindow::~PointRenameWindow()
{
}

void PointRenameWindow::cb_Button_Cancel(Fl_Widget * w, void * v)
{
	((PointRenameWindow*)v)->cb_Button_Cancel_i();
}

inline void PointRenameWindow::cb_Button_Cancel_i()
{
	this->hide();
	((ImagesShowWindow*)win)->StereoWindow->set_DrawingMode(((ImagesShowWindow*)win)->StereoWindow->PreviousDrawingMode);
}

void PointRenameWindow::cb_Button_Ok(Fl_Widget * w, void * v)
{
	((PointRenameWindow*)v)->cb_Button_Ok_i();
}

inline void PointRenameWindow::cb_Button_Ok_i()
{
	string NewPointName(Input_Point_Name_New->value());
	if (NewPointName != "")
	{
		if (((ImagesShowWindow*)win)->Toggle_ButtonPomiarLewy->value() == 1)
		{
			((ImagesShowWindow*)win)->StereoWindow->LeftPoints.at(Idx_Point_To_Rename).Name = NewPointName;

			if (IsGCP == true)
			{
				((ImagesShowWindow*)win)->StereoWindow->LeftPoints.at(Idx_Point_To_Rename).Type = 3;
			}
			else
			{
				((ImagesShowWindow*)win)->StereoWindow->LeftPoints.at(Idx_Point_To_Rename).Type = 0;
			}

			ImagePoint::StrumienDoZapisu_L.close();
			ImagePoint::StrumienDoZapisu_L.open(((ImagesShowWindow*)win)->PixFileName_Left.c_str(), ios::out);
			for (int i = 0; i<((ImagesShowWindow*)win)->LeftImage.NPoints; i++)
			{
				((ImagesShowWindow*)win)->StereoWindow->LeftPoints[i].write_L();
			}
			ImagePoint::StrumienDoZapisu_L.close();
			ImagePoint::StrumienDoZapisu_L.open(((ImagesShowWindow*)win)->PixFileName_Left.c_str(), ios::app);

		}

		if (((ImagesShowWindow*)win)->Toggle_ButtonPomiarPrawy->value() == 1)
		{
			((ImagesShowWindow*)win)->StereoWindow->RightPoints.at(Idx_Point_To_Rename).Name = NewPointName;

			if (IsGCP == true)
			{
				((ImagesShowWindow*)win)->StereoWindow->RightPoints.at(Idx_Point_To_Rename).Type = 3;
			}
			else
			{
				((ImagesShowWindow*)win)->StereoWindow->RightPoints.at(Idx_Point_To_Rename).Type = 0;
			}

			ImagePoint::StrumienDoZapisu_P.close();
			ImagePoint::StrumienDoZapisu_P.open(((ImagesShowWindow*)win)->PixFileName_Right.c_str(), ios::out);
			for (int i = 0; i<((ImagesShowWindow*)win)->RightImage.NPoints; i++)
			{
				((ImagesShowWindow*)win)->StereoWindow->RightPoints[i].write_P();
			}
			ImagePoint::StrumienDoZapisu_P.close();
			ImagePoint::StrumienDoZapisu_P.open(((ImagesShowWindow*)win)->PixFileName_Right.c_str(), ios::app);
		}

	}

	((ImagesShowWindow*)win)->StereoWindow->redraw();
	this->hide();
	((ImagesShowWindow*)win)->StereoWindow->set_DrawingMode(((ImagesShowWindow*)win)->StereoWindow->PreviousDrawingMode);



}

void PointRenameWindow::cb_Button_Input_Point_Name_New(Fl_Widget * w, void * v)
{
	((PointRenameWindow*)v)->cb_Button_Input_Point_Name_New_i();
}

inline void PointRenameWindow::cb_Button_Input_Point_Name_New_i()
{

		string Id(Input_Point_Name_New->value());
		if (Id != "")
		{
			int NGCP = ((MainWindow*)((ImagesShowWindow*)win)->win)->NGCP; //Podstawienie za zmienna lokalna dla czytelnosci
			IsGCP = false;

			for (int i = 0; i < NGCP; i++)
			{
				if (((MainWindow*)((ImagesShowWindow*)win)->win)->GC.at(i).Name == Id)
				{
					IsGCP = true;
					break;
				}
			}

			if (IsGCP)
			{
				Output_Point_Type_New->value("GCP");
			}
			else
			{
				Output_Point_Type_New->value("Tie");
			}

			Button_Ok->activate();
		}
		else
		{
			Output_Point_Type_New->value("");
			Button_Ok->deactivate();
		}

}


int PointRenameWindow::handle(int e)
{
	int ret = Fl_Window::handle(e);
	switch (e)
	{
	case FL_KEYBOARD:
		switch (Fl::event_key())
		{
		case FL_Enter:
			if (Button_Ok->active())
			{
				Button_Ok->do_callback();
			}
			ret = 1;
			break;
		}
		break;
	}
	return ret;
}