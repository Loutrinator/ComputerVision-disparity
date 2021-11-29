#include "ComputerVision.h"

using namespace cv;

ComputerVision::ComputerVision() : lineColor(0,0,255), pointColor(0,255,255)
{
}

void ComputerVision::drawFrame(cv::Mat* inputFrame, bool showPoints)
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
	imshow("Video", frame);

}

void ComputerVision::detectPoints(cv::Mat* frame)
{
	cv::goodFeaturesToTrack(*frame, nextPoints, 3000, 0.05, 5);
}

std::vector<cv::Point2f> purgePoints(std::vector<cv::Point2f>& points, std::vector<uchar>& status) {
	std::vector<cv::Point2f> result;
	for (int i = 0; i < points.size(); ++i) {
		if (status[i] > 0)result.push_back(points[i]);
	} return result;
}


bool ComputerVision::trackPoints(cv::Mat* frame, int requiredNbPoints)
{
	bool pointsFound;
	if (!prevInput.empty()) {
		prevPoints = nextPoints; //je stoque les pr�c�dents points
		if (prevPoints.size() < requiredNbPoints) {//je n'ai pas assez de points � tracker
			detectPoints(&prevInput); //je calcul les nouveaux points
			if (prevPoints.size() < requiredNbPoints) return false; //on n'a toujours pas assez de points. On quitte la fonction
		}
		pointsFound = true; // on a trouv� des points (dit � drawFrame de tracer des points sur l'image)
		std::vector<uchar> status;
		std::vector<float> err;
		cv::calcOpticalFlowPyrLK(prevInput, *frame, prevPoints, nextPoints, status, err);
		nextPoints = purgePoints(nextPoints, status);
		//on retire les points qui ne sont pas track�s
	}
	prevInput = (*frame).clone();//on copie la frame actuelle dans prevInput pour en garder une r�f�rence
	prevPoints = nextPoints;
	return pointsFound;
}