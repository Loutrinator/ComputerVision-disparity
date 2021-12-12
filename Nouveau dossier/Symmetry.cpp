// Disparity.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/opencv.hpp>
#include "ComputerVision.h"

#include <iostream>
#include <string>
#include "Symmetry.h"

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
	Mat image = imread("./resources/f.jpg", 1);

	if (image.empty()) // On test si notre vidéo est lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	cv::Ptr<cv::ORB> orb = ORB::create(1000);//team classic shit


	cv::Mat lFrame;
	cv::Mat rFrame;

	if (BnWInputs) {
		cv::cvtColor(image, lFrame, COLOR_BGR2GRAY);
	}
	else {
		lFrame = image.clone();
	}

	cv::flip(lFrame, rFrame, 1);

	CVengine.detectComputePoints(orb, &lFrame, &rFrame);

	CVengine.drawWindow(&lFrame, &rFrame, windowName);

	cv::waitKey(0);
	return 0;
}