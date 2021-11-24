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
	Mat lImage = imread("./resources/disparity-02-left.png", 1);
	Mat rImage = imread("./resources/disparity-02-right.png", 1);

	if (lImage.empty() || rImage.empty()) // On test si nos deux images ont bien été trouvées et lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	std::vector<cv::Point2f> pointsInLeftImage;
	std::vector<cv::Point2f> pointsInRightImage;
	CVengine.findMatchings(&lImage, &rImage, &pointsInLeftImage, &pointsInRightImage);
	CVengine.displayMatchings(&lImage, &rImage, &pointsInLeftImage, &pointsInRightImage,true);

	//Affichage des images
	String LeftImageWindowName = "Left image";
	namedWindow(LeftImageWindowName); 
	imshow(LeftImageWindowName, lImage); 

	String RightImageWindowName = "Right image";
	namedWindow(RightImageWindowName); 
	imshow(RightImageWindowName, rImage); 


	waitKey(0);

	//Fin du programme
	destroyWindow(LeftImageWindowName);
	destroyWindow(RightImageWindowName);

	return 0;
}