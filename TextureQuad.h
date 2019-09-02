#pragma once

class TextureQuad
{
public:
	int X1, Y1, X2, Y2; //Narozniki - lewy gorny i prawy dolny
	int Row; //Wiersz liczac od gory
	int Col; //Kolumna liczac od lewej
	TextureQuad(int _X1, int _Y1, int _X2, int _Y2, int _Row, int _Col);
	TextureQuad(void);
	~TextureQuad(void);
};
