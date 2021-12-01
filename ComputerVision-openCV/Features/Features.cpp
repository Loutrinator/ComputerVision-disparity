// Disparity.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/opencv.hpp>
#include "ComputerVision.h"

#include <iostream>
#include <string>
#include "Features.h"

using namespace cv;
using namespace std;

ComputerVision CVengine;

int main(int argc, char** argv)
{
	std::string windowName = "Video";
	cv::namedWindow(windowName);

	// Chargement des images
	VideoCapture video("./resources/set1/video.mp4");
	Mat imageToTrack = imread("./resources/set1/naruto.jpg", 1);

	if (!video.isOpened() && !imageToTrack.empty()) // On test si notre vidéo est lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	cv::Ptr<cv::ORB> orb = ORB::create();
	Mat currentFrame;

	while (true) {
		if (!video.read(currentFrame))break;
		CVengine.detectComputePoints(orb, &currentFrame, &imageToTrack);
		CVengine.drawWindow(&currentFrame, &imageToTrack, windowName);
		if (cv::waitKey(16) == 27) {
			break;
		}
	}
	cv::waitKey(0);
	return 0;
}