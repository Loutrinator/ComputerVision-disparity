#include "ComputerVision.h"

using namespace cv;

ComputerVision::ComputerVision() : lineColor(0,0,255), pointColor(0, 255, 255), roiColor(0, 255, 0), start(-1, -1)
{
}

void ComputerVision::drawFrame(cv::Mat* inputFrame, std::string windowName, bool showPoints)
{
	cv::Mat frame;
	frame = inputFrame->clone();
	if (showPoints) {
		for (int i = 0; i < nextPoints.size(); ++i) {
			cv::Point2f currentPosition = nextPoints[i];
			cv::Point2f prevPosition = prevPoints[i];
			cv::circle(frame, currentPosition, 3, pointColor, 1);
			cv::line(frame, currentPosition, prevPosition, lineColor, 1);
		}
	}
	cv::rectangle(frame, roi.tl(), roi.br(), roiColor, 2);
	imshow(windowName, frame);

}

void ComputerVision::detectPoints(cv::Mat* frame)
{
	cv::goodFeaturesToTrack(*frame, prevPoints, 100, 0.05, 10);
}

std::vector<cv::Point2f> ComputerVision::purgePoints(std::vector<cv::Point2f>& points, std::vector<uchar>& status) {
	std::vector<cv::Point2f> result;
	for (int i = 0; i < points.size(); ++i) {
		cv::Point2f currentPoint = points[i];
		if (status[i] > 0) {
			result.push_back(currentPoint);
		}
	} return result;
}


bool ComputerVision::trackPoints(cv::Mat* frame, int requiredNbPoints)
{
	//if (currentPoint.x >= roi.tl().x && currentPoint.x <= roi.br().x && currentPoint.y <= roi.tl().y && currentPoint.y >= roi.br().y) {
	bool pointsFound = false;

	if (!prevInput.empty()) {

		cv::Mat mask = cv::Mat::zeros(frame->size(), CV_8U);
		cv::rectangle(mask, roi.br(), roi.tl(), cv::Scalar(255, 255, 255), -1);
		cv::Mat widerMask = cv::Mat::zeros(frame->size(), CV_8U);
		int offset = 0;
		Point2f minXY(roi.tl().x - offset, roi.br().y + offset);
		Point2f maxXY(roi.br().x + offset, roi.tl().y - offset);
		cv::rectangle(widerMask, minXY, maxXY, cv::Scalar(255, 255, 255), -1);
		//je show les deux masks les uns sur les autres
		cv::Mat showMasks = mask.clone(); 
		cv::rectangle(showMasks, minXY, maxXY, cv::Scalar(0, 0, 255), 1);
		imshow("mask", showMasks);

		cv::Mat maskedPrevious;
		cv::bitwise_and(prevInput, mask, maskedPrevious);
		imshow("masked previous", maskedPrevious);
		cv::Mat maskedNext;
		cv::bitwise_and(*frame, widerMask, maskedNext);

		prevPoints = nextPoints; //je stoque les précédents points
		if (prevPoints.size() < requiredNbPoints) {//je n'ai pas assez de points à tracker
			detectPoints(&maskedPrevious); //je calcul les nouveaux points
			if (prevPoints.size() < requiredNbPoints) return false; //on n'a toujours pas assez de points. On quitte la fonction
		}
		pointsFound = true; // on a trouvé des points (dit à drawFrame de tracer des points sur l'image)
		std::vector<uchar> status;
		std::vector<float> err;
		cv::calcOpticalFlowPyrLK(maskedPrevious, maskedNext, prevPoints, nextPoints, status, err);
		prevPoints = purgePoints(prevPoints, status);
		nextPoints = purgePoints(nextPoints, status);
		//on retire les points qui ne sont pas trackés
	}
	prevInput = (*frame).clone();//on copie la frame actuelle dans prevInput pour en garder une référence
	//prevPoints = nextPoints;
	return pointsFound;
}

void ComputerVision::updateROI()
{
	if (roi.empty()) return;
	int minX = INT_MAX;
	int maxX = INT_MIN;
	int minY = INT_MAX;
	int maxY = INT_MIN;

	for (int i = 0; i < nextPoints.size(); ++i) {
		Point2f p = nextPoints[i];
		if (minX > p.x) minX = p.x;
		if (maxX < p.x) maxX = p.x;
		if (minY > p.y) minY = p.y;
		if (maxY < p.y) maxY = p.y;
	}
	roi = Rect(Point2f(minX, minY), Point2f(maxX, maxY));
}


