# IMACCU

Image Measurement And Camera Calibration Utility

## What is IMACCU?

IMACCU is a simple GUI tool that provides You a quick interface for:
- measuring points in the images
- calculating the camera calibration based on the measurements

IMACCU provides the simple interface for the automatic measrurement of the most poplular calibration pattern which is a chessboard.What is very usefull IMACCU allows You to manually delete particular measurements (like outliers or gross errors) or edit them. The above would be quite a cumbersome issue without having a GUI! The detailed calibration report contains outliers of each measurement as well as standard deviations of each calibration parameter.

<img src="https://user-images.githubusercontent.com/54469000/64493207-7d831b80-d27d-11e9-9605-05913396c102.jpg" width="600">

The software provides up to 4 viewports with zoom and pan options for visualization of measurements. IMACCU is extremally lightweight as its GUI is developped using FLTK library.

<img src="https://user-images.githubusercontent.com/54469000/64493210-807e0c00-d27d-11e9-8b87-5f4d092e1a0b.jpg" width="600">

The software code has been developped through many years before sharing it in GitHub so it involves different coding styles. Newest parts are coded using C++17.

## Dependencies

To compile IMACCU You need:
- Eigen 3.3.7 for some math
- fltk-1.3.4-2 for GUI
- Opencv 3.4.6 for chessboard corner detection and camera calibration

However older versions of Eigen and Opencv should also work.

## Compiling IMACCU

I have developped the software in Windows. I haven't tested it under Linux.
This repo contains the vcxproj which You can use to compile IMACCU under Visual Studio. First You need to have FLTK and Opencv build for Visual Studio.

1. Setup FLTK for Visual Studio
Here is the article that will guid You through instalation of FLTK:
https://bumpyroadtocode.com/2017/08/05/how-to-install-and-use-fltk-1-3-4-in-visual-studio-2017-complete-guide/
I only did steps 1 and 2. I didn't copy the .lib and .h files to the Visual Studio file structure. Instead I set the proper directories in the project properties.

2. Download and istall Opencv for Windows from https://opencv.org/releases/

3. Download Eigen from http://eigen.tuxfamily.org/index.php?title=Main_Page. You don't have to install Eigen - it is the header olny library.

4. Open IMACCU.vcxproj in Visual Studio. Open project settings and in Release x64 cofiguration go to the Configuration Properties -> C/C++ -> General and add Eigen, OpenCV and FLTK header locations to the Additional Include Directories.

5. In project settings go to the Configuration Properties -> Liner -> General and add Your locations of .lib files for both OpenCV and FLTK

6. Try to build the project.

7. In order to run the app You will have the opencv dll file: opencv_world346.dll in the same location as the IMACCU.exe or You should include the opencv_world346.dll location to the PATH location.

## Using IMACCU
