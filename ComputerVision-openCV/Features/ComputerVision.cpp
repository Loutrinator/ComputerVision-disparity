#include "ComputerVision.h"

using namespace cv;

ComputerVision::ComputerVision() : lineColor(0,0,255), pointColor(0, 255, 255), roiColor(0, 255, 0), distanceValue(0.65)
{
}

void ComputerVision::drawWindow(cv::Mat* inputFrame, cv::Mat* targetFrame, std::string windowName)
{
	cv::Mat iFrame = inputFrame->clone();
	cv::Mat tFrame = targetFrame->clone();

	/*
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
	cv::imshow(windowName, iFrame);
	cv::imshow("object", tFrame);
	*/

	//std::vector<DMatch> good;
	cv::Mat resultImage;
	//std::cout << "lKeyPts : " << lKeyPts.size() << " rKeyPts : " << rKeyPts.size() << std::endl;
	//std::cout << "targetFrame : " << (*targetFrame).size() << " inputFrame : " << (*inputFrame).size() << std::endl;
	//std::cout << "validFeatures : " << validFeatures.size() << std::endl;
	cv::drawMatches(*targetFrame, lKeyPts, *inputFrame, rKeyPts, validFeatures, resultImage);
	cv::imshow(windowName, resultImage);
}

void ComputerVision::detectComputePoints(cv::Ptr<cv::ORB> orb, cv::Mat* video, cv::Mat* target)
{

	orb->detect(*target, lKeyPts);
	orb->compute(*target, lKeyPts, leftDesc);
	orb->detect(*video, rKeyPts);
	orb->compute(*video, rKeyPts, rightDesc);

	cv::Ptr<cv::BFMatcher> bfmMatcher = cv::BFMatcher::create();
	std::vector<std::vector<cv::DMatch>> matrixOfMatches;
	bfmMatcher->knnMatch(leftDesc, rightDesc, matrixOfMatches, 2);

	validFeatures.clear();//IMPORTANT :'(

	for (int i = 0; i < matrixOfMatches.size(); ++i) {
		std::vector<cv::DMatch> match = matrixOfMatches[i];
		if (match[0].distance < distanceValue * match[1].distance) {
			validFeatures.push_back(match[0]);
		}
	}
	//cv::Mat resultImage;
	//cv::drawMatches(*target, lKeyPts, *video, rKeyPts, validFeatures, resultImage);
	//cv::imshow("test", resultImage);
}

