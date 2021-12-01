#pragma once
#include <opencv2/opencv.hpp>

class ComputerVision
{
public:
    ComputerVision();
    void drawFrame(cv::Mat* inputFrame, std::string windowName, bool showPoints);
    void detectPoints(cv::Mat* frame);
    bool trackPoints(cv::Mat* frame, int requiredNbPoints);
    void updateROI();
    std::vector<cv::Point2f> purgePoints(std::vector<cv::Point2f>& points, std::vector<uchar>& status);
    cv::Scalar lineColor;
    cv::Scalar pointColor;
    cv::Scalar roiColor;
    std::vector<cv::Point2f> prevPoints;
    std::vector<cv::Point2f> nextPoints;
    cv::Mat prevInput;
    cv::Rect roi;
    cv::Point start;
private:
};

