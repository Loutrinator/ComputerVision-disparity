#include "ComputerVision.h"

using namespace cv;

ComputerVision::ComputerVision() : lineColor(0,0,255), pointColor(0, 255, 255), roiColor(0, 255, 0), distanceValue(0.6), minPointsSize(4)
{
}

void ComputerVision::drawWindow(cv::Mat* inputFrame, cv::Mat* targetFrame, std::string windowName)
{
	//tracing ROI
	cv::Mat videoFrame = inputFrame->clone();
	if (validFeatures.size() > minPointsSize) {
		cv::line(videoFrame, roiPoints[0], roiPoints[1], roiColor, 2);
		cv::line(videoFrame, roiPoints[1], roiPoints[2], roiColor, 2);
		cv::line(videoFrame, roiPoints[2], roiPoints[3], roiColor, 2);
		cv::line(videoFrame, roiPoints[3], roiPoints[0], roiColor, 2);
	}
	cv::Mat resultImage;
	//std::cout << "lKeyPts : " << lKeyPts.size() << " rKeyPts : " << rKeyPts.size() << std::endl;
	//std::cout << "targetFrame : " << (*targetFrame).size() << " inputFrame : " << (*inputFrame).size() << std::endl;
	//std::cout << "validFeatures : " << validFeatures.size() << std::endl;
	cv::drawMatches(*targetFrame, lKeyPts, videoFrame, rKeyPts, validFeatures, resultImage);
	cv::imshow(windowName, resultImage);
}

void ComputerVision::detectComputePoints(cv::Ptr<cv::ORB> orb, cv::Mat* video, cv::Mat* target)
{
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
}

void ComputerVision::computeOutline(cv::Mat* target)
{
	if (validFeatures.size() > minPointsSize) {
		std::vector<cv::Point2f> lValidPts;
		std::vector<cv::Point2f> rValidPts;
		for (int i = 0; i < validFeatures.size(); ++i) {
			cv::DMatch feature = validFeatures[i];
			lValidPts.push_back(lKeyPts[feature.queryIdx].pt);
			rValidPts.push_back(rKeyPts[feature.trainIdx].pt);
		}
		cv::Mat transMat = findHomography(lValidPts, rValidPts, cv::RANSAC);
		std::vector<Point2f> rectShape;
		int width = (*target).cols;
		int height = (*target).rows;
		rectShape.push_back(Point2f(0, 0));
		rectShape.push_back(Point2f(width, 0));
		rectShape.push_back(Point2f(width, height));
		rectShape.push_back(Point2f(0, height));
		std::vector<Point2f> xformedRectShape;
		if (transMat.dims > 0) {//il a trouvé une matrice pouvant résoudre la déformation des points
			roiPoints.clear();
			cv::perspectiveTransform(rectShape, roiPoints, transMat);
		}
	}
	
}

