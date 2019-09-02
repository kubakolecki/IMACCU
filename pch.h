// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <FL/Fl.H> 
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
//#include <opencv2\core.hpp>
//#include <opencv2\opencv.hpp>
//#include <opencv2\highgui.hpp>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\cxcore.h>
#include <opencv\cvaux.h>
#include <ctime>
#include <vector>
#include <iterator>
#include "kFltk.h"
#include "MainWindow.h"
#include "TerrainPoint.h"
#include "ImagePoint.h"
#include "photogrammetry.h"
#include "Camera.h"
#include "Image.h"

#endif //PCH_H
