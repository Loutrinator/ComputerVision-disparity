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
	Mat lImage = imread("./resources/disparity-01-left.png", 1);
	Mat rImage = imread("./resources/disparity-01-right.png", 1);

	if (lImage.empty() || rImage.empty()) // On test si nos deux images ont bien été trouvées et lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	//on cherche les points en commun
	std::vector<cv::Point2f> pointsInLeftImage;
	std::vector<cv::Point2f> pointsInRightImage;
	CVengine.findMatchings(&lImage, &rImage, &pointsInLeftImage, &pointsInRightImage);

	//on affiche ces points
	CVengine.displayMatchings(&lImage, &rImage, &pointsInLeftImage, &pointsInRightImage,true);

	//A l'aide de ces points on corrige la perspective de ces images
	Mat rectifiedA;
	Mat rectifiedB;
	CVengine.rectify(&lImage, &rImage, &pointsInLeftImage, &pointsInRightImage, &rectifiedA, &rectifiedB);

	//Affichage des images
	String LeftImageWindowName = "Rectified left image";
	namedWindow(LeftImageWindowName); 
	imshow(LeftImageWindowName, rectifiedA);

	String RightImageWindowName = "Rectified right image";
	namedWindow(RightImageWindowName); 
	imshow(RightImageWindowName, rectifiedB);


	waitKey(0);

	//Fin du programme
	destroyWindow(LeftImageWindowName);
	destroyWindow(RightImageWindowName);

	return 0;
}