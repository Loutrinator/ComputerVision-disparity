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
	std::string windowName = "prout";
	bool BnWInputs = true;

	// Chargement des images
	VideoCapture video("./resources/set1/video.mp4");
	Mat targeFile = imread("./resources/set1/naruto.jpg", 1);
	Mat imageToTrack;
	cv::cvtColor(targeFile, imageToTrack, COLOR_BGR2GRAY);

	if (!video.isOpened() && !imageToTrack.empty()) // On test si notre vidéo est lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	cv::Ptr<cv::ORB> orb = ORB::create();//team classic shit

	Mat currentFrame;

	while (true) {
		if (!video.read(currentFrame))break;
		Mat frame;
		cv::cvtColor(currentFrame, frame, COLOR_BGR2GRAY);
		if (BnWInputs) {
			CVengine.detectComputePoints(orb, &frame, &imageToTrack);
			CVengine.drawWindow(&frame, &imageToTrack, windowName);
		}
		else {
			CVengine.detectComputePoints(orb, &currentFrame, &targeFile);
			CVengine.drawWindow(&currentFrame, &targeFile, windowName);
		}
		if (cv::waitKey(16) == 27) {
			break;
		}
	}
	cv::waitKey(0);
	return 0;
}