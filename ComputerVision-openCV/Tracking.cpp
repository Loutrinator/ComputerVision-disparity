// Disparity.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/opencv.hpp>
//#include "ComputerVision.h"

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	/*ComputerVision CVengine;

	
	// Chargement des images
	cv::VideoCapture video("./resources/tracking-2.mp4");

	if (!video.isOpened()) // On test si notre vidéo est lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	String VideoWindow = "Video";
	namedWindow(VideoWindow);
	cv::Mat currentFrame;

	while (video.read(currentFrame)) {
		imshow(VideoWindow, currentFrame);
	}
	*/
	std::cout << "Hello Ronan!" << std::endl;
	return 0;
}