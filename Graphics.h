#pragma once

class Graphics
{
public:
	std::string Filename;
	bool Circular_Cursor;
	bool Draw_Epipolar;
	int Circle_Radius;
	int Cross_Size;
	float Cursor_Color[3];
	float Epipolar_Color[3];
	float Post_Size;
	Graphics(void);
	Graphics(std::string ProjectFolder);
	~Graphics(void);
	void write();
	void read();
	void wypisz();
	void setProjectFolder(std::string ProjectFolder);
};
