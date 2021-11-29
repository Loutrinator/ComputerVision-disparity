// Disparity.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/opencv.hpp>
#include "ComputerVision.h"

#include <iostream>
#include <string>
#include "Tracking.h"

using namespace cv;
using namespace std;

ComputerVision CVengine;

void mouseCallback(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		CVengine.start = cv::Point(x, y);
		CVengine.roi = cv::Rect();
		CVengine.prevPoints.clear();
		CVengine.nextPoints.clear();
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		if (CVengine.start.x >= 0) {
			cv::Point end(x, y);
			CVengine.roi = cv::Rect(CVengine.start, end);
		}
	}
	else if (event == cv::EVENT_LBUTTONUP) {
		cv::Point end(x, y);
		CVengine.roi = cv::Rect(CVengine.start, end);
		CVengine.start = cv::Point(-1, -1);
	}
}
int main(int argc, char** argv)
{
	std::string windowName = "Video";
	cv::namedWindow(windowName);
	setMouseCallback(windowName, mouseCallback, NULL);

	// Chargement des images
	VideoCapture video("./resources/tracking_3.mp4");

	if (!video.isOpened()) // On test si notre vid�o est lue correctement
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	Mat currentFrame;
	bool endReached = false;
	while (!endReached) {

		bool pointsWereFound = false;
		if (CVengine.start.x < 0) {
			if (!video.read(currentFrame))endReached = true;
			cv::Mat bwFrame;
			cv::cvtColor(currentFrame, bwFrame, cv::COLOR_BGR2GRAY);
			if (CVengine.roi.area() >= 3) {
				pointsWereFound = CVengine.trackPoints(&bwFrame, 3);
			}

		}
		CVengine.drawFrame(&currentFrame, windowName, pointsWereFound);

		if (cv::waitKey(16) == 27) {
			break;
		}
	}
	cv::waitKey(0);
	return 0;
}