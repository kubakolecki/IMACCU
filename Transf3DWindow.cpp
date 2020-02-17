#include "pch.h"
#include "Transf3DWindow.h"

Transf3DWindow::Transf3DWindow(int w, int h, const char* title) :Fl_Window(w, h, title)
{
	begin();
	Chooser_Source = new Fl_File_Chooser(NULL, "*.txt", 1, "Select Source Points");
	Chooser_Target = new Fl_File_Chooser(NULL, "*.txt", 1, "Select Target Points");
	Chooser_Points = new Fl_File_Chooser(NULL, "*.txt", 1, "Select Points to Transform");

	Output_Source = new Fl_Output(160, 10, 180, 25, "Source Points");
	Output_Target = new Fl_Output(160, 45, 180, 25, "Target Points");
	Output_Points = new Fl_Output(160, 80, 180, 25, "Points to Transform");

	Button_Source = new Fl_Button(350, 10, 25, 25, "...");
	Button_Target = new Fl_Button(350, 45, 25, 25, "...");
	Button_Points = new Fl_Button(350, 80, 25, 25, "...");

	Input_Raport = new Fl_Input(160, 115, 180, 25, "Raport");

	Input_Priority = new Fl_Input_Choice(160, 155, 120, 25, "Priority");
	Button_Scale = new Fl_Check_Button(290, 155, 120, 25, "Calc. scale");

	Button_Solve = new Fl_Button(160, 190, 80, 25, "Solve");
	end();

	Input_Priority->add("Symmetric");
	Input_Priority->add("Source");
	Input_Priority->add("Target");

	Input_Priority->value("Symmetric");
	Button_Scale->value(1);

	Button_Source->callback(cb_Button_Source, this);
	Button_Target->callback(cb_Button_Target, this);
	Button_Points->callback(cb_Button_Points, this);
	Chooser_Source->callback(cb_Chooser_Source, this);
	Chooser_Target->callback(cb_Chooser_Target, this);
	Chooser_Points->callback(cb_Chooser_Points, this);
	Button_Solve->callback(cb_Button_Solve, this);
	Button_Scale->callback(cb_Button_Scale, this);
	Input_Priority->callback(cb_Input_Priority, this);

}

Transf3DWindow::~Transf3DWindow(void)
{

}

void Transf3DWindow::cb_Button_Source(Fl_Widget *w, void *v)
{
	((Transf3DWindow*)v)->cb_Button_Source_i();
}

void Transf3DWindow::cb_Button_Source_i()
{
	Chooser_Source->show();
}


void Transf3DWindow::cb_Button_Target(Fl_Widget *w, void *v)
{
	((Transf3DWindow*)v)->cb_Button_Target_i();
}

void Transf3DWindow::cb_Button_Target_i()
{
	Chooser_Target->show();
}

void Transf3DWindow::cb_Button_Points(Fl_Widget *w, void *v)
{
	((Transf3DWindow*)v)->cb_Button_Points_i();
}

void Transf3DWindow::cb_Button_Points_i()
{
	Chooser_Points->show();
}




void Transf3DWindow::cb_Chooser_Source(Fl_File_Chooser *w, void *v)
{
	((Transf3DWindow*)v)->cb_Chooser_Source_i();
}

void Transf3DWindow::cb_Chooser_Source_i()
{
	Output_Source->value(Chooser_Source->value());
}

void Transf3DWindow::cb_Chooser_Target(Fl_File_Chooser *w, void *v)
{
	((Transf3DWindow*)v)->cb_Chooser_Target_i();
}

void Transf3DWindow::cb_Chooser_Target_i()
{
	Output_Target->value(Chooser_Target->value());
	string s = Chooser_Target->value();
	int ShlashPos = s.rfind("/");
	string RapName = s.substr(0, ShlashPos + 1) + "raport.txt";
	Input_Raport->value(RapName.c_str());
}

void Transf3DWindow::cb_Chooser_Points(Fl_File_Chooser *w, void *v)
{
	((Transf3DWindow*)v)->cb_Chooser_Points_i();
}

void Transf3DWindow::cb_Chooser_Points_i()
{
	Output_Points->value(Chooser_Points->value());
}


void Transf3DWindow::cb_Button_Solve(Fl_Widget *w, void *v)
{
	((Transf3DWindow*)v)->cb_Button_Solve_i();
}

void Transf3DWindow::cb_Button_Solve_i()
{

	ifstream STRSource;
	ifstream STRTarget;
	STRSource.open(Output_Source->value());
	STRTarget.open(Output_Target->value());
	string Name;
	double X, Y, Z;
	vector<TerrainPoint> Source;
	vector<TerrainPoint> Target;
	vector<TerrainPoint> Points;

	//Wczytanie punktow do wektorow

	while (STRSource)
	{
		STRSource >> Name >> X >> Y >> Z;
		if (STRSource)
		{
			TerrainPoint T(Name, X, Y, Z);
			Source.push_back(T);
		}
	}

	while (STRTarget)
	{
		STRTarget >> Name >> X >> Y >> Z;
		if (STRTarget)
		{
			TerrainPoint T(Name, X, Y, Z);
			Target.push_back(T);
		}
	}

	if (Source.size() == Target.size())
	{
		TransfData.solve(&Source, &Target);
		TransfData.calcErrors(&Source, &Target);

		//Transformacja:
		ifstream STRPoints;
		STRPoints.open(Output_Points->value());
		while (STRPoints)
		{
			STRPoints >> Name >> X >> Y >> Z;
			if (STRPoints)
			{
				TerrainPoint T(Name, X, Y, Z);
				Points.push_back(T);
			}
		}
		STRPoints.close();

		vector<TerrainPoint> OutputPoints;
		TransfData.transform(&Points, &OutputPoints);

		ofstream STRRaport;
		STRRaport.open(Input_Raport->value());

		STRRaport << "3D Transform - raport" << endl;
		STRRaport << "Estimate scale: ";
		if (TransfData.EstScale == true)
		{
			STRRaport << "yes\n";
		}
		else
		{
			STRRaport << "no\n";
		}
		STRRaport << "Priority: ";
		switch (TransfData.Priority)
		{
		case 0:
			STRRaport << "symmetric\n";
			break;
		case 1:
			STRRaport << "source\n";
			break;
		case 2:
			STRRaport << "target\n";
			break;
		}

		STRRaport << "\nResults:\n";
		STRRaport << "Scale: " << fixed << setprecision(9) << TransfData.s << "\n";
		STRRaport << "Translation:\n";
		STRRaport << fixed << setprecision(4) << TransfData.T[0] << " " << TransfData.T[1] << " " << TransfData.T[2] << "\n";
		STRRaport << "Rotation:\n";
		for (int i = 0; i < 9; i++)
		{
			STRRaport << fixed << setprecision(12) << TransfData.R[i];
			if (i % 3 == 2)
			{
				STRRaport << "\n";
			}
			else
			{
				STRRaport << " ";
			}
		}

		STRRaport << "\nCorrections:\n";
		for (int i = 0; i < Source.size(); i++)
		{
			STRRaport << setw(10) << Source[i].Name << " " << fixed << setw(12) << setprecision(5) << TransfData.VX[i] << " " << setw(12) << TransfData.VY[i] << " " << setw(12) << TransfData.VZ[i] << "\n";
		}

		STRRaport << "\nErrors (Mx,My,Mz,Mp):\n";
		STRRaport << TransfData.Mx << " " << TransfData.My << " " << TransfData.Mz << " " << TransfData.Mp << "\n";

		STRRaport << "\nTransformed points:\n";
		for (int i = 0; i < OutputPoints.size(); i++)
		{
			STRRaport << setw(10) << OutputPoints[i].Name << " " << fixed << setprecision(4) << setw(20) << OutputPoints[i].X << " " << setw(20) << OutputPoints[i].Y << " " << setw(20) << OutputPoints[i].Z << "\n";
		}

		STRRaport.close();

	}

	STRTarget.close();
	STRSource.close();
}

void Transf3DWindow::cb_Button_Scale(Fl_Widget *w, void *v)
{
	((Transf3DWindow*)v)->cb_Button_Scale_i();
}

void Transf3DWindow::cb_Button_Scale_i()
{
	if (Button_Scale->value() == 1)
	{
		TransfData.EstScale = true;
	}
	else
	{
		TransfData.EstScale = false;
		TransfData.s = 1;
	}
}

void Transf3DWindow::cb_Input_Priority(Fl_Widget *w, void *v)
{
	((Transf3DWindow*)v)->cb_Input_Priority_i();
}

void Transf3DWindow::cb_Input_Priority_i()
{
	string s = Input_Priority->value();
	if (s == "Symmetric") TransfData.Priority = 0;
	if (s == "Source") TransfData.Priority = 1;
	if (s == "Target") TransfData.Priority = 2;

}
