#include "ComputerVision.h"

using namespace cv;

ComputerVision::ComputerVision() : lineColor(255,0,0)
{
}

void ComputerVision::findMatchings(cv::Mat* iA, cv::Mat* iB, std::vector<cv::Point2f>* pA, std::vector<cv::Point2f>* pB) const
{
	std::vector<cv::Point2f> tempPA;
	std::vector<cv::Point2f> tempPB;
	cv::Mat imageLeft;
	cv::Mat imageRight;
	cv::cvtColor(*iA, imageLeft, cv::COLOR_BGR2GRAY);
	cv::cvtColor(*iB, imageRight, cv::COLOR_BGR2GRAY);
	cv::goodFeaturesToTrack(imageLeft, tempPA, 3000, 0.05, 25);
	std::vector<uchar> status;
	std::vector<float> err;
	cv::calcOpticalFlowPyrLK(imageLeft, imageRight, tempPA, tempPB,status, err);
	
	for (int i = 0; i < status.size(); ++i) {
		uchar currentStatus = status[i];
		if (currentStatus == 1) {
			pA->push_back(tempPA[i]);
			pB->push_back(tempPB[i]);
		}
	}

}

void ComputerVision::displayMatchings(cv::Mat* iA, cv::Mat* iB, std::vector<cv::Point2f>* pA, std::vector<cv::Point2f>* pB, bool compare) const
{
	cv::Mat result;
	if (compare) {//Mode de comparaison
		cv::hconcat(*iA, *iB, result);
		std::vector<cv::Point2f> temppB;
		int iAWidth = iA->cols;
		for (int i = 0; i < pB->size(); ++i) {
			temppB.push_back(cv::Point2f((*pB)[i].x + iAWidth, (*pB)[i].y));
		}
		drawlines(&result, pA, &temppB);//tracage des lignes
	}else {//Mode de supperposition sur l'image d'origine
		iA->copyTo(result);
		drawlines(&result, pA, pB);//tracage des lignes
	}

	std::string disparityLines = "Disparity lines";
	cv::namedWindow(disparityLines);
	cv::imshow(disparityLines, result);
}

void ComputerVision::rectify(cv::Mat* iA, cv::Mat* iB, std::vector<cv::Point2f>* pA, std::vector<cv::Point2f>* pB, cv::Mat* outA, cv::Mat* outB) const
{
	cv::Mat fundamentalMatrix = cv::findFundamentalMat(*pA, *pB);//calcul de la matrice fondamentale
	cv::Mat h1;
	cv::Mat h2;
	cv::stereoRectifyUncalibrated(*pA, *pB, fundamentalMatrix, iA->size(), h1, h2);//calcul des matrices de rectification
	cv::warpPerspective(*iA, *outA, h1, iA->size());
	cv::warpPerspective(*iB, *outB, h2, iB->size());
}

void ComputerVision::computeDisparity(cv::Mat* iA, cv::Mat* iB, cv::Mat* output)
{
	cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create();
	cv::Mat disparity;

	cv::Mat gsIa;
	cv::Mat gsIb;
	cv::cvtColor(*iA, gsIa, cv::COLOR_BGR2GRAY);
	cv::cvtColor(*iB, gsIb, cv::COLOR_BGR2GRAY);

	sbm->compute(gsIa, gsIb, disparity);
	//on calcule les values min et max de la carte de disparité
	double minVal;
	double maxVal;
	cv::minMaxLoc(disparity, &minVal, &maxVal);
	double coeff = 255.0 / (maxVal - minVal);
	double offset = -minVal*255.0 / (maxVal - minVal);
	cv::Mat finalDisparity(disparity.size(),CV_8U);
	finalDisparity = offset + (disparity * coeff);
	finalDisparity.convertTo(finalDisparity, CV_8U);

	String disparityImageWindowName = "Disparity";
	namedWindow(disparityImageWindowName);
	imshow(disparityImageWindowName, finalDisparity);
}

void ComputerVision::drawlines(cv::Mat* image, std::vector<cv::Point2f>* const pA, std::vector<cv::Point2f>* const pB) const
{
	for (int i = 0; i < pA->size(); ++i) {
		cv::line(*image, (*pA)[i], (*pB)[i], lineColor, 1);
	}
}
