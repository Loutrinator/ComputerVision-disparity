// Disparity.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/opencv.hpp>
#include "ComputerVision.h"

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	ComputerVision CVengine;


	// Chargement des images
	VideoCapture video("./resources/tracking_3.mp4");

	if (!video.isOpened()) // On test si notre vidéo est lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	Mat currentFrame;

	while (video.read(currentFrame)) {
		cv::Mat bwFrame;
		cv::cvtColor(currentFrame, bwFrame, cv::COLOR_BGR2GRAY);
		bool pointsWereFound = CVengine.trackPoints(&bwFrame,100);
		CVengine.drawFrame(&currentFrame, pointsWereFound);

		if (cv::waitKey(16) == 27) {
			break;
		}
	}
	cv::waitKey(0);
	return 0;
}