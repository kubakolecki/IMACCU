#pragma once
#include "Camera.h"
#include "Image.h"
#include "SimplePoint.h"

int fT_findColsestImpoint(ImagePoint *P,int N, double X, double Y, double * D);
int fT_findColsestImpoint(vector <ImagePoint> *P, double X, double Y, double * D);
int fT_findClosesPoint(vector <TerrainPoint>*T, double X, double Y, double Z, double *D);
double fT_imdist(ImagePoint *P1, ImagePoint *P2);
double fT_terrainDist(TerrainPoint *T1, TerrainPoint *T2);
double fT_terrainDist(double *T1, double *T2);
double fT_terrainDist(TerrainPoint *T1, double *T2);
void fT_cor2pix(ImagePoint *Source, ImagePoint *Target, int N, int Width, int Height);
void fT_cor2pix(vector<ImagePoint> &Source, vector<ImagePoint> &Target, int Width, int Height);
void fT_pix2fid(ImagePoint *Source, ImagePoint *Target, int N, Camera *Cam);
void fT_pix2fid(ImagePoint *Source, ImagePoint *Target, int N, Image *Im);
void fT_pix2fid(vector<ImagePoint> *Source, vector<ImagePoint> *Target, Camera* Cam);
//void fT_pix2fid(ImagePoint *Source, ImagePoint *Target, int N, Image *Im);
void fT_pix2fid(vector<ImagePoint> &Source, vector<ImagePoint> &Target, Image& Im);
void fT_fid2pix(ImagePoint *Source, ImagePoint *Target, int N, Image* Im);
void fT_fid2uds(ImagePoint *Source, ImagePoint *Target, int N, Camera *Cam);
void fT_fid2uds(ImagePoint *Source, ImagePoint *Target, int N, Image *Im);
void fT_fid2uds(vector<ImagePoint> *Source, vector<ImagePoint> *Target, Camera* Cam);
void fT_uds2fid(ImagePoint *Source, ImagePoint *Target, int N,Image *Im);
double fT_xDistCorrection (Image *Obraz, double x, double y);
double fT_yDistCorrection (Image *Obraz, double x, double y);
double fT_findFurthestImpoints (ImagePoint *P, int N, int * Index, int Type);
double fT_distFromLine(ImagePoint *P1, ImagePoint* P2, ImagePoint* P3);
double fT_distFromLine(TerrainPoint *T1, TerrainPoint *T2, TerrainPoint *T3, double* ClP);
void fT_sel4Points4Resection(ImagePoint *P, int N, int* Index);
void fT_transform2first(TerrainPoint *Source, TerrainPoint *Target,CvMat* R);
void fT_biasCoords(TerrainPoint *Source, TerrainPoint *Dest, double DX, double DY, double DZ);
void fT_biasCoords(TerrainPoint *Source,  double DX, double DY, double DZ);
short fT_trilateration(TerrainPoint *T, double *Dist, double *Result1, double *Result2);
double fT_angleInCamera(ImagePoint *P1, ImagePoint*P2, double ck);
double fT_distInCamera(ImagePoint *P, double ck);
void fT_tetrahedreon2rot(TerrainPoint *GCP, double *C, ImagePoint *P, double ck, CvMat* R);
double fT_checkEO(double *Coords, CvMat * R, TerrainPoint * T, ImagePoint *P, double ck);
void fT_rot2angles(CvMat* MacierzObrotu, double* Angles, string System);
void fT_rot2angles(double* R, double* Angles, string System);
void fT_rot2quaternions(CvMat* MacierzObrotu, double* Q);
void fT_rot2quaternions(double* R, double* Q);
void fT_rot2vector(CvMat * Rot, double *R);
void fT_quaternions2rot(double * Q, CvMat *R);
void fT_quaternions2rot(double * Q, double *R);
void fT_angles2rot(double *Angles, CvMat *R, string System);
void fT_angles2rot(double *Angles, double *R, string System);
void fT_findEpiR(Image* Obraz1, Image* Obraz2, double *R);
void fT_orientation2dlt(double *IO, double *Coords, CvMat* R, double *DLT);
void fT_orientation2dlt(double *Io, double *Coords, double *Angles, string System, double *DLT);
void fT_rotavg(CvMat*Source1, CvMat*Source2, CvMat *Res);
short fT_killianResection(ImagePoint* P, TerrainPoint * T, double ck, double (*ProjCenter)[3], CvMat *RotationMatrix[3]);
void fT_ground2ImageByDLT(TerrainPoint *T,double *DLT, int N, ImagePoint* P);
void fT_ground2Image(TerrainPoint* T, Image* Obraz, int N, ImagePoint *P);

void fT_ground2IdealImage(TerrainPoint* T, Image* Obraz, int N, ImagePoint *P);
void fT_ground2IdealImage(vector <TerrainPoint> *T, Image* Obraz, vector <ImagePoint> *P);
void fT_ground2IdealImage(double **T , Image* Obraz, int N, ImagePoint *P, bool PCZero = false);
void fT_ground2IdealImage(double *T , Image* Obraz, ImagePoint *P, bool PCZero = false);
void fT_ground2IdealImage(double *T , Image* Obraz, double *P, bool PCZero = false);
int fT_countByType(ImagePoint *P,int N, int T);
int fT_countByType(TerrainPoint *T, int N, int Typ);
void fT_selectPointByType(ImagePoint *Input, int N, int Type, ImagePoint *Output);
void fT_selectPointByType(TerrainPoint *Input, int N, int Type, TerrainPoint *Output);
bool fT_isImPointInArray(ImagePoint *Array, int N, string Name);
short ft_solveQuadraticEquation(double *Coef, double* Results);
short ft_solveQuatricsReal(long double *Coef, long double *Results);
long double ft_findV(long double *V1, long double *V2);
double ft_findV(double *V1, double *V2);
void fT_findRelative(double *RLeft, double *RRight, double *CoordsL, double *CoordsR, CvMat *R, CvMat *B);
void fT_multiImageIntersection(vector<TerrainPoint>& P, vector <vector<double> >& R, vector <float>& cks, vector<cv::Point2f> &Measurements, TerrainPoint &Result);
double fT_fastModelInteresection(double *ImVecL, double *ImVecR, double *r, double* b, double *Result, double *V);
double fT_fastModelInteresection(double *ImVecL, double *ImVecR, vector<double> *r, vector<double>* b, double *Result, double *V);
bool fT_testCheriality(double *r, double *b, double* XL, double* YL, double* XP, double* YP, double ckL, double ckP, int N);
void fT_vectorTrans3(double *A, double* v, double *vt);
void fT_vectorTrans3(double *A, double* b, double* v, double *vt);
void fT_vectorTrans3(double *A, TerrainPoint *v, TerrainPoint *vt);
void fT_vectorTrans3(double *A, vector<double> *v, vector<double> *vt);
double fT_dotProd3(double *v, double *w);
void fT_transpose3(double *A, double *AT);
void fT_matTranspose(double *A, int *sizeA, double *AT);
void fT_matInv3(double*A, double *B);
void fT_matMult3(double *A, double *B, double *C);
void fT_matMult(double *A, double *B, int* sizeA, int* sizeB, double *C);
bool fT_compMatrix3(double *A, double *B);
void fT_dispMatrix3(double *A); 
bool fT_testRotationMatrix(double *r, double DIFF);
void fT_calcNonorthonormality(double *R, double *Test);
void fT_orthonormalize(double *R, double *RN);
void fT_calcCentroid(vector<TerrainPoint> *T, TerrainPoint *C);
double fT_angMean(double A1, double A2);
void fT_fundamental(double *LX, double *LY, double *RX, double *RY, double *F);
void fT_f2e(double *F, double *E);





