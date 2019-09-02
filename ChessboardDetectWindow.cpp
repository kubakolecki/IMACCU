#include "pch.h"
#include "ChessboardDetectWindow.h"
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\calib3d.hpp>


ChessboardDetectWindow::ChessboardDetectWindow(const char * title) :Fl_Window(440, 700, title)
{
	begin();
		Browser_Images = new Fl_Multi_Browser(20, 20, 220, 620, "Images");
		Browser_Control_Points = new Fl_Multi_Browser(250, 20, 60, 620, "#GCP");
		Input_Rows = new Fl_Int_Input(370, 20, 60, 30, "Rows:");
		Input_Cols = new Fl_Int_Input(370, 70, 60, 30, "Cols:");
		Button_Run = new Fl_Button(370, 120, 60, 30, "Run");
		Progress_Detection = new Fl_Progress(20, 660, 220, 20);
	end();
	Window_Warning_GCP = new WarningWindow(220, 180, "Warning!", "#GCP != rows X cols");
	Button_Run->callback(cb_Button_Run, this);
	Input_Rows->value("8");
	Input_Cols->value("8");
	Progress_Detection->minimum(0);
	Progress_Detection->maximum(1);
	Progress_Detection->selection_color(FL_GREEN);

}


ChessboardDetectWindow::~ChessboardDetectWindow()
{
}

void ChessboardDetectWindow::cb_Button_Run(Fl_Widget * w, void * v)
{
	((ChessboardDetectWindow*)v)->cb_Button_Run_i();
}

inline void ChessboardDetectWindow::cb_Button_Run_i()
{
	Progress_Detection->value(0);
	
	Rows = atoi(Input_Rows->value());
	Cols = atoi(Input_Cols->value());

	Browser_Control_Points->clear();

	if (((MainWindow*)win)->GC.size() != Rows * Cols)
	{
		
		Window_Warning_GCP->show();
		Window_Warning_GCP->set_modal();
		return;
	}

	float progress_max = static_cast<float>(2 * Browser_Images->size());
	float progress{ 0.0 };

	Button_Run->deactivate();
	for (int i = 1; i <= Browser_Images->size(); i++)
	{
		string s = Browser_Images->text(i);
		string ida_file_name = ((MainWindow*)win)->ProjectFolder + s;
		string pix_file_name = ((MainWindow*)win)->ProjectFolder + s.substr(0, s.length() - 4) + ".pix";
		Image Obraz;
		Obraz.ReadFromFile(ida_file_name.c_str());
		progress += 1.0;
		Progress_Detection->value(progress / progress_max);
		Fl::check();

		cv::Mat ChessboardImage = cv::imread(Obraz.ImageFileName);
		std::vector<cv::Point2f> corners;
		cv::findChessboardCorners(ChessboardImage, cv::Size(Cols, Rows), corners);
		Browser_Control_Points->add(num2str(corners.size()).c_str());

		vector<ImagePoint> ImagePoints;
		ImagePoints.reserve(corners.size());

		int gcp_id = 0;
		for (auto &c : corners)
		{
			string id = ((MainWindow*)win)->GC.at(gcp_id++).Name;
			ImagePoints.emplace_back(id,
				Obraz.Name,
				c.x - 0.5*ChessboardImage.cols + 0.5,
				0.5*ChessboardImage.rows - c.y - 0.5,
				3);
			
		}

		ImagePoints.shrink_to_fit();
		ImagePoint::StrumienDoZapisu.close();
		if (!ImagePoint::StrumienDoZapisu)
		{
			ImagePoint::StrumienDoZapisu_L.clear(ImagePoint::StrumienDoZapisu_L.rdstate() & ~ios::failbit);
		}
		ImagePoint::StrumienDoZapisu.open(pix_file_name, ios::app);
		for (auto &p : ImagePoints)
		{
			p.write();
		}

		Obraz.NPoints = corners.size();
		Obraz.WriteToFile(ida_file_name.c_str());

		//image reload:
		if (corners.size() > 0)
		{
			if (((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->LoadedL)
			{
				((MainWindow*)win)->OknoWyswietlaniaObrazow->InputObrazLewy->do_callback();
			}

			if (((MainWindow*)win)->OknoWyswietlaniaObrazow->StereoWindow->LoadedR)
			{
				((MainWindow*)win)->OknoWyswietlaniaObrazow->InputObrazPrawy->do_callback();
			}

			if (((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->LoadedL)
			{
				((MainWindow*)win)->OknoWyswietlaniaObrazow_2->InputObrazLewy->do_callback();
			}

			if (((MainWindow*)win)->OknoWyswietlaniaObrazow_2->StereoWindow->LoadedR)
			{
				((MainWindow*)win)->OknoWyswietlaniaObrazow_2->InputObrazPrawy->do_callback();
			}
		}

		progress += 1.0;
		Progress_Detection->value(progress / progress_max);
		Fl::check();
	}

	Button_Run->activate();
}
