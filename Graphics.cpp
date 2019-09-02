#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(void)
{

}

Graphics::Graphics(string ProjectFolder)
{
	Filename = ProjectFolder + "graph";
	Circular_Cursor = false; 
	Draw_Epipolar = false;
	Circle_Radius = 6;
	Cross_Size = 4;
	Cursor_Color[0] = 1.0;
	Cursor_Color[1] = 0.0;
	Cursor_Color[2] = 0.0;
	Epipolar_Color[0] = 1.0;
	Epipolar_Color[1] = 1.0;
	Epipolar_Color[2] = 0.0;
	Post_Size = 3;
	Epipolar_Color[3];
}

Graphics::~Graphics(void)
{

}

void Graphics::setProjectFolder(string ProjectFolder)
{
	Filename = ProjectFolder + "graph";
}

void Graphics::write()
{
	ofstream STR;
	STR.open(Filename.c_str(),ios::binary);
	STR.write((char*)&Circular_Cursor, sizeof(Circular_Cursor));
	STR.write((char*)&Draw_Epipolar, sizeof(Draw_Epipolar));
	STR.write((char*)&Circle_Radius, sizeof(Circle_Radius));
	STR.write((char*)&Cross_Size, sizeof(Cross_Size));
	for (int i=0; i<3; i++) STR.write((char*)(Cursor_Color+i), sizeof(Cursor_Color[i]));
	for (int i=0; i<3; i++) STR.write((char*)(Epipolar_Color+i), sizeof(Epipolar_Color[i]));
	STR.write((char*)&Post_Size, sizeof(Post_Size));
	STR.close();
}

void Graphics::read()
{
	ifstream STR;
	STR.open(Filename.c_str(),ios::binary);
	STR.read((char*)&Circular_Cursor, sizeof(Circular_Cursor));
	STR.read((char*)&Draw_Epipolar, sizeof(Draw_Epipolar));
	STR.read((char*)&Circle_Radius, sizeof(Circle_Radius));
	STR.read((char*)&Cross_Size, sizeof(Cross_Size));
	for (int i=0; i<3; i++) STR.read((char*)(Cursor_Color+i), sizeof(Cursor_Color[i]));
	for (int i=0; i<3; i++) STR.read((char*)(Epipolar_Color+i), sizeof(Epipolar_Color[i]));
	STR.read((char*)&Post_Size, sizeof(Post_Size));
	STR.close();
}

void Graphics::wypisz()
{
	cout << "\nCircular_Cursor: " << Circular_Cursor <<endl;
	cout << "Draw_Epipolar: " << Draw_Epipolar <<endl;
	cout << "Circle_Radius: " << Circle_Radius <<endl;
	cout << "Cross_Size: " << Cross_Size <<endl;
	cout <<"Post_Size: "<<Post_Size<<endl;
	for (int i=0; i<3; i++)
	{
		cout << Cursor_Color[i] << " ";
	}
	cout << endl;
	for (int i=0; i<3; i++)
	{
		cout << Epipolar_Color[i] << " ";
	}

}
