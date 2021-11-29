#pragma once
#include <opencv2/opencv.hpp>

class ComputerVision
{
public:
    ComputerVision();
    void findMatchings(cv::Mat* const iA, cv::Mat* const iB, std::vector<cv::Point2f>* pA, std::vector<cv::Point2f>* pB) const;
    void displayMatchings(cv::Mat* const iA, cv::Mat* const iB, std::vector<cv::Point2f>* pA, std::vector<cv::Point2f>* pB, bool compare) const;
    void rectify(cv::Mat* const iA, cv::Mat* const iB, std::vector<cv::Point2f>* const pA, std::vector<cv::Point2f>* const pB, cv::Mat* outA, cv::Mat* outB) const;
    void computeDisparity(cv::Mat* iA, cv::Mat* iB, cv::Mat* output);
    cv::Scalar lineColor;
private:
    void drawlines(cv::Mat* image, std::vector<cv::Point2f>* const  pA, std::vector<cv::Point2f>* const  pB) const;
};

