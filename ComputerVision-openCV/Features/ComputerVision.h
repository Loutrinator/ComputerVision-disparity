#pragma once
#include <opencv2/opencv.hpp>

class ComputerVision
{
public:
    ComputerVision();
    void drawWindow(cv::Mat* inputFrame, cv::Mat* targetFrame, std::string windowName);
    void detectComputePoints(cv::Ptr<cv::ORB> orb, cv::Mat* video, cv::Mat* target);
    void computeOutline(cv::Mat* video, cv::Mat* target);
    std::vector<cv::Point2f> purgePoints(std::vector<cv::Point2f>& points, std::vector<uchar>& status);
    cv::Scalar lineColor;
    cv::Scalar pointColor; 
    cv::Scalar roiColor;
    std::vector<cv::KeyPoint> lKeyPts;
    std::vector<cv::KeyPoint> rKeyPts;
    std::vector<cv::DMatch>validFeatures;
    cv::Mat leftDesc;
    cv::Mat rightDesc;
    float distanceValue;
private:
};

