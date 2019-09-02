#pragma once
using namespace std;
class Camera
{
public:
	std::string Name;
	std::string Description;
	int W;
	int H;
	float ck;
	float x0 = 0.0;
	float y0 = 0.0;
	double k1 = 0.0;
	double k2 = 0.0;
	double k3 = 0.0;
	double p1 = 0.0;
	double p2 = 0.0;
	double y_scaling = 0.0;
	double skewness = 0.0;
	short Valid;
	Camera(void);
	Camera (const char * Filename);
	Camera (double* io, double *k, double *p, double *a);
	~Camera(void);
	void wypisz(void);
	void writeToFile(string& filename) const;
};
