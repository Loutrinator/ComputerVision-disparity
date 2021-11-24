#pragma once
#include <opencv2/opencv.hpp>

class ComputerVision
{
public:
    ComputerVision();
    void findMatchings(cv::Mat* iA, cv::Mat* iB, std::vector<cv::Point2f>* pA, std::vector<cv::Point2f>* pB) const;
    void displayMatchings(cv::Mat* iA, cv::Mat* iB, std::vector<cv::Point2f>* pA, std::vector<cv::Point2f>* pB, bool compare) const;
private:
};

