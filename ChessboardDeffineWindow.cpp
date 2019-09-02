#include "pch.h"
#include "ChessboardDeffineWindow.h"


ChessboardDeffineWindow::ChessboardDeffineWindow(int w, int h, const char * title) :Fl_Window(w, h, title)
{
	begin();
		Input_Rows = new Fl_Int_Input(240, 20, 80, 30, "Number of Intersections: Rows:");
		Input_Cols = new Fl_Int_Input(240, 70, 80, 30, "Number of Intersections: Columns:");
		Input_Dim = new Fl_Float_Input(240, 120, 80, 30, "Size of Square:"); 
		Input_Prefix = new Fl_Input(240, 170, 80, 30, "Prefix of the ID:");
		Button_Generate = new Fl_Button(240, 220, 80, 30, "Generate");
		
	end();
	Button_Generate->callback(cb_Button_Generate, this);
	Input_Rows->value("8");
	Input_Cols->value("8");
	Input_Dim->value("0.040");
	Input_Prefix->value("c");

}

ChessboardDeffineWindow::~ChessboardDeffineWindow()
{
}


void ChessboardDeffineWindow::cb_Button_Generate(Fl_Widget * w, void * v)
{
	((ChessboardDeffineWindow*)v)->cb_Button_Generate_i();
}

inline void ChessboardDeffineWindow::cb_Button_Generate_i()
{
	int rows{ atoi(Input_Rows->value()) };
	int cols{ atoi(Input_Cols->value()) };
	double size{ atof(Input_Dim->value()) };

	if (
		size > 0.0 &&
		rows > 4 &&
		cols > 4
		)
	{

		//rows and cols reffer to the number of chessboard fields - the number of intersections
		//is smaler by 1

		((MainWindow*)win)->GC.clear();

		std::string Id;
		std::stringstream str;

		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				double x = r * size;
				double y = c * size;
				str.clear();
				str << Input_Prefix->value() << std::setfill('0') << std::setw(2) << r+1 << std::setfill('0') << std::setw(2) << c+1;
				str >> Id;
				((MainWindow*)win)->GC.push_back(TerrainPoint(Id, x, y, 0.000, 0.001, 0.001, 0.001, 3));
			}
		}

		std::string gcp_filename = ((MainWindow*)win)->ProjectFolder +
			((MainWindow*)win)->CurrentProjectName.substr(0, ((MainWindow*)win)->CurrentProjectName.length() - 4) + 
			".gcp";

		//setting the size:
		((MainWindow*)win)->OknoChessboardDetect->Input_Rows->value(num2str(rows).c_str());
		((MainWindow*)win)->OknoChessboardDetect->Input_Cols->value(num2str(cols).c_str());
		
		

		writeGCPToFile(gcp_filename, ((MainWindow*)win)->GC);
		this->hide();
	}
}
