// IMACCU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "MainWindow.h"
#include "WarningWindow.h"
#include <stdlib.h>
using namespace std;
string CurrentProject = "";
MainWindow OknoGlowne(600, 100);

int main(int argc, _TCHAR* argv[])
{
		OknoGlowne.show();
		return Fl::run();
}