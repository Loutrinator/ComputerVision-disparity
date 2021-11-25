#include "ComputerVision.h"

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
	cv::goodFeaturesToTrack(imageLeft, tempPA, 10, 0.05, 10);
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

void ComputerVision::drawlines(cv::Mat* image, std::vector<cv::Point2f>* const pA, std::vector<cv::Point2f>* const pB) const
{
	for (int i = 0; i < pA->size(); ++i) {
		cv::line(*image, (*pA)[i], (*pB)[i], lineColor, 1);
	}
}
