// Disparity.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// Chargement des images
	Mat lImage = imread("./resources/disparity-01-left.jpg");
	Mat rImage = imread("./resources/disparity-01-right.jpg");

	if (lImage.empty() || rImage.empty()) // On test si nos deux images ont bien été trouvées et lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}


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