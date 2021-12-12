// Disparity.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/opencv.hpp>
#include "ComputerVision.h"

#include <iostream>
#include <string>
#include "Sym.h"

using namespace cv;
using namespace std;

ComputerVision CVengine;

int main(int argc, char** argv)
{
	std::string windowName = "Symmetry points";
	std::string windowMatchesName = "Matches";
	bool BnWInputs = true;
	CVengine.distanceValue = 0.75;
	CVengine.minPointsSize = 1;

	// Chargement des images
	Mat image = imread("./resources/f.jpg", 1);

	if (image.empty()) // On test si notre vidéo est lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	cv::Ptr<cv::ORB> orb = ORB::create(1500);//team classic shit

	cv::Mat lFrame;

	if (BnWInputs) {
		cv::cvtColor(image, lFrame, COLOR_BGR2GRAY);
	}
	else {
		lFrame = image.clone();
	}
	CVengine.computeSymmetry(orb, &lFrame, X_AXIS);
	//CVengine.computeSymmetry(orb, &lFrame, Y_AXIS);

	CVengine.drawMatchesSingleFrame(&lFrame, windowMatchesName);

	cv::waitKey(0);
	return 0;
}