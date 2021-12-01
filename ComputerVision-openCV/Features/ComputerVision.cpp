#include "ComputerVision.h"

using namespace cv;

ComputerVision::ComputerVision() : lineColor(0,0,255), pointColor(0, 255, 255), roiColor(0, 255, 0), distanceValue(20)
{
}

void ComputerVision::drawWindow(cv::Mat* inputFrame, cv::Mat* targetFrame, std::string windowName)
{
	cv::Mat iFrame = inputFrame->clone();
	cv::Mat tFrame = targetFrame->clone();;
	std::vector<Point2f> lPoints;
	cv::KeyPoint::convert(lKeyPts, lPoints);
	std::vector<Point2f> rPoints;
	cv::KeyPoint::convert(rKeyPts, rPoints);

	for (int i = 0; i < lPoints.size(); ++i) {
		cv::Point2f point = lPoints[i];
		cv::circle(iFrame, point, 3, pointColor, 1);
	}
	for (int i = 0; i < rPoints.size(); ++i) {
		cv::Point2f point = rPoints[i];
		cv::circle(tFrame, point, 3, pointColor, 1);
	}
	cv::imshow("video", iFrame);
	cv::imshow("object", tFrame);
	
	//std::vector<DMatch> good;
	//cv::Mat resultImage;
    //cv:drawMatches(*targetFrame, rKeyPts, *inputFrame, lKeyPts, validFeatures, resultImage);

	//cv::imshow("resultImage", resultImage);

}

void ComputerVision::detectComputePoints(cv::Ptr<cv::ORB> orb, cv::Mat* video, cv::Mat* target)
{
	orb->detect(*video, lKeyPts);
	orb->compute(*video, lKeyPts, leftDesc);
	orb->detect(*target, rKeyPts);
	orb->compute(*target, rKeyPts, rightDesc);

	cv::Ptr <cv::BFMatcher> bfmMatcher = cv::BFMatcher::create();
	std::vector<std::vector<cv::DMatch>> matrixOfMatches;
	bfmMatcher->knnMatch(leftDesc, rightDesc, matrixOfMatches, 2);

	for (int i = 0; i < matrixOfMatches.size(); ++i) {
		std::vector<cv::DMatch> match = matrixOfMatches[i];
		if (match[0].distance < distanceValue * match[1].distance) {
			validFeatures.push_back(match[0]);
		}
	}


}

