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
	CVengine.distanceValue = 0.7;
	CVengine.minPointsSize = 8;

	// Chargement des images
	VideoCapture video("./resources/set1/video.mp4");
	Mat targetFile = imread("./resources/set1/naruto.jpg", 1);

	if (!video.isOpened() && !targetFile.empty()) // On test si notre vidéo est lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	cv::Ptr<cv::ORB> orb = ORB::create(1000);//team classic shit


	cv::Mat lFrame;
	cv::Mat rFrame;

	if (BnWInputs) {
		cv::cvtColor(targetFile, lFrame, COLOR_BGR2GRAY);
	}
	else {
		lFrame = targetFile.clone();
	}


	orb->detect(lFrame, CVengine.lKeyPts);
	orb->compute(lFrame, CVengine.lKeyPts, CVengine.leftDesc);

	Mat currentFrame;
	while (true) {
		if (!video.read(currentFrame))break;
		Mat frame;
		if (BnWInputs) {
			cv::cvtColor(currentFrame, rFrame, COLOR_BGR2GRAY);
		}
		else {
			rFrame = currentFrame.clone();
		}

		CVengine.detectComputePoints(orb, &rFrame, &lFrame);
		CVengine.computeOutline(&lFrame);

		CVengine.drawWindow(&currentFrame, &targetFile, windowName);

		if (cv::waitKey(16) == 27) {
			break;
		}
	}
	cv::waitKey(0);
	return 0;
}