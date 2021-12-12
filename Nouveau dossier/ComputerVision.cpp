#include "ComputerVision.h"

using namespace cv;

ComputerVision::ComputerVision() : lineColor(0,0,255), pointColor(0, 255, 255), roiColor(0, 255, 0), distanceValue(0.6), minPointsSize(4)
{
}

void ComputerVision::drawWindow(cv::Mat* lFrame, cv::Mat* rFrame, std::string windowName)
{
	cv::Mat resultImage;
	//std::cout << "lKeyPts : " << lKeyPts.size() << " rKeyPts : " << rKeyPts.size() << std::endl;
	//std::cout << "targetFrame : " << (*targetFrame).size() << " inputFrame : " << (*inputFrame).size() << std::endl;
	//std::cout << "validFeatures : " << validFeatures.size() << std::endl;
	cv::drawMatches(*lFrame, lKeyPts, *rFrame, rKeyPts, validFeatures, resultImage);
	cv::imshow(windowName, resultImage);
}

void ComputerVision::detectComputePoints(cv::Ptr<cv::ORB> orb, cv::Mat* lFrame, cv::Mat* rFrame)
{

	orb->detect(*lFrame, lKeyPts);
	orb->compute(*lFrame, lKeyPts, leftDesc);
	orb->detect(*rFrame, rKeyPts);
	orb->compute(*rFrame, rKeyPts, rightDesc);

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
