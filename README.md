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

## Dependencies

To compile IMACCU You need:
- Eigen 3.3.7 for some math
- fltk-1.3.4-2 for GUI
- opencv 3.4.6 for chessboard corner detection and camera calibration

## Compiling IMACCU

## Using IMACCU
