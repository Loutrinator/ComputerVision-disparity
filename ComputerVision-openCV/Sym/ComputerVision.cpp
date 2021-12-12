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
		for (int i = 0; i < flippedValidPts.size(); ++i) {
			Point2f px = flippedValidPts[i];
			px.x = frame->cols - px.x;
			xFlippedRPoints.push_back(px);
		}

		//cv::flip(rValidPts, rValidPts, -1);
		//je draw les lignes
		for (int i = 0; i < flippedValidPts.size(); ++i) {
			Point2f lP = normalValidPts[i];
			Point2f xrP = xFlippedRPoints[i];
			cv::line(resultImage, lP, xrP, lineColor);
			//cv::line(resultImage, lP, yrP, lineColor);
		}
		//je draw les barycentres
		for (int i = 0; i < normalValidPts.size(); ++i) {
			Point2f lP = normalValidPts[i];
			Point2f xrP = xFlippedRPoints[i];
			Point2f xBC;
			xBC.x = (xrP.x - lP.x) / 2 + lP.x;
			xBC.y = (xrP.y - lP.y) / 2 + lP.y;
			xBarryCenters.push_back(xBC);
			cv::circle(resultImage, xBC, 2, pointColor);
		}
	}
	cv::imshow(windowName, resultImage);
}

void ComputerVision::drawBarrycenters(cv::Mat* frame, std::string windowName)
{
	cv::Mat bCenter = Mat::zeros(frame->size(),CV_8U);
	for (int i = 0; i < xBarryCenters.size(); ++i) {
		Point2f bc = xBarryCenters[i];
		cv::circle(bCenter, bc, 2, Scalar(255, 255, 255));
	}
	cv::imshow(windowName, bCenter);
}

void ComputerVision::computeSymmetry(cv::Ptr<cv::ORB> orb, cv::Mat* img, SymmetryAxe axe)
{
	cv::Mat flipped;
	cv::flip(*img, flipped, 1);
	cv::imshow("Flipped images", flipped);
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

void ComputerVision::computeHough(cv::Mat* frame, std::string windowName)
{
	cv::Mat res;
	std::vector<Vec2f> lines; // will hold the results of the detection
	HoughLines(xBarryCenters, lines, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection
	// Draw the lines
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(res, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
	}
	imshow(windowName, res);
}


