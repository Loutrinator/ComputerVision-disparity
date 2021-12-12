#include "ComputerVision.h"

using namespace cv;

ComputerVision::ComputerVision() : lineColor(0,255,0), pointColor(0, 0, 255), roiColor(0, 255, 0), distanceValue(0.6), minPointsSize(4)
{
}


void ComputerVision::drawMatchesSingleFrame(cv::Mat* frame, std::string windowName)
{
	cv::Mat resultImage = frame->clone();
	cv::cvtColor(resultImage, resultImage, COLOR_GRAY2BGR);
	if (flippedValidPts.size() > minPointsSize) {

		//je flip les points valides de l'image de droite sur X
		std::vector<Point2f> xFlippedRPoints;
		std::vector<Point2f> yFlippedRPoints;
		for (int i = 0; i < flippedValidPts.size(); ++i) {
			Point2f px = flippedValidPts[i];
			px.x = frame->cols - px.x;
			xFlippedRPoints.push_back(px);

			Point2f py = flippedValidPts[i];
			py.y = frame->rows - py.y;
			yFlippedRPoints.push_back(py);
		}

		//cv::flip(rValidPts, rValidPts, -1);
		//je draw les lignes
		for (int i = 0; i < flippedValidPts.size(); ++i) {
			Point2f lP = normalValidPts[i];
			Point2f xrP = xFlippedRPoints[i];
			Point2f yrP = yFlippedRPoints[i];
			//cv::line(resultImage, lP, xrP, lineColor);
			cv::line(resultImage, lP, yrP, lineColor);
		}
		//je draw les barycentres
		for (int i = 0; i < normalValidPts.size(); ++i) {
			Point2f lP = normalValidPts[i];
			Point2f xrP = xFlippedRPoints[i];
			Point2f yrP = yFlippedRPoints[i];
			Point2f xBC;
			Point2f yBC;
			xBC.x = (xrP.x - lP.x) / 2 + lP.x;
			xBC.y = (xrP.y - lP.y) / 2 + lP.y;
			yBC.x = (yrP.x - lP.x) / 2 + lP.x;
			yBC.y = (yrP.y - lP.y) / 2 + lP.y;
			xBarryCenters.push_back(xBC);
			yBarryCenters.push_back(yBC);
			cv::circle(resultImage, xBC, 2, pointColor);
			cv::circle(resultImage, yBC, 2, pointColor);// (resultImage, lP, rP, lineColor);
		}
	}
	cv::imshow(windowName, resultImage);
}

void ComputerVision::computeSymmetry(cv::Ptr<cv::ORB> orb, cv::Mat* img, SymmetryAxe axe)
{
	cv::Mat flipped;
	cv::flip(*img, flipped, axe);
	cv::Mat normalDesc;
	cv::Mat flippedDesc;

	normalKeyPts.clear();
	orb->detect(*img, normalKeyPts);
	orb->compute(*img, normalKeyPts, normalDesc);

	orb->detect(flipped, flippedKeyPts);
	orb->compute(flipped, flippedKeyPts, flippedDesc);
	normalValidPts.clear();
	flippedValidPts.clear();
	xBarryCenters.clear();
	yBarryCenters.clear();

	cv::Ptr<cv::BFMatcher> bfmMatcher = cv::BFMatcher::create();
	std::vector<std::vector<cv::DMatch>> matrixOfMatches;
	bfmMatcher->knnMatch(normalDesc, flippedDesc, matrixOfMatches, 2);

	for (int i = 0; i < matrixOfMatches.size(); ++i) {
		std::vector<cv::DMatch> match = matrixOfMatches[i];
		if (match[0].distance < distanceValue * match[1].distance) {

			normalValidPts.push_back(normalKeyPts[match[0].queryIdx].pt);
			flippedValidPts.push_back(flippedKeyPts[match[0].trainIdx].pt);

		}
	}

}


