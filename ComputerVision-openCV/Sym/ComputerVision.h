#pragma once
#include <opencv2/opencv.hpp>


enum SymmetryAxe { X_AXIS = 0, Y_AXIS = 1 };
class ComputerVision
{
public:
    ComputerVision();
    void drawMatchesSingleFrame(cv::Mat* frame, std::string windowName);
    void drawBarrycenters(cv::Mat* frame, std::string windowName);
    void computeSymmetry(cv::Ptr<cv::ORB> orb, cv::Mat* img, SymmetryAxe axe);
    std::vector<cv::Point2f> purgePoints(std::vector<cv::Point2f>& points, std::vector<uchar>& status);
    cv::Scalar lineColor;
    cv::Scalar pointColor; 
    cv::Scalar roiColor;
    float distanceValue;

    std::vector<cv::KeyPoint> normalKeyPts;
    std::vector<cv::KeyPoint> flippedKeyPts;

    std::vector<cv::Point2f> normalValidPts;
    std::vector<cv::Point2f> flippedValidPts;

    std::vector<cv::Point2f> xBarryCenters;
    std::vector<cv::Point2f> yBarryCenters;
    int minPointsSize;
private:
};

