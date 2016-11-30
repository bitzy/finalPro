#ifndef PLANIMETRY_TOOLS_H
#define PLANIMETRY_TOOLS_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

double getEucliDist(double p1x, double p1y, double p2x, double p2y);
double getEucliDist(cv::Point   p1, cv::Point   p2);
double getEucliDist(cv::Point3d p1, cv::Point3d p2);
double getEucliDist(cv::Vec3d   p1, cv::Vec3d   p2);

void getDotsOnLine(cv::Point p1, cv::Point p2, std::vector<cv::Point>& dots);
int  getDotsOnLine(cv::Point p1, cv::Point p2, cv::Point p3, std::vector<cv::Point>& dots);

double getDistFromP2L(cv::Point p1, cv::Point p2, cv::Point p3);

void normalnizeVector(std::vector<double>& v);

//histogram tool:
class HistoGram1D {
private:
    int   histSize[1];
    float hranges[2];
    const float * ranges[1];
    int channels[1];
public:
    HistoGram1D(int binNum, int minV, int maxV);
    cv::MatND getHistogram1D(const cv::Mat& img);
    cv::Mat getHistogramImage(const cv::Mat& img);
};

void outColors2IMG(const cv::Mat &colors, const char* name);
cv::Mat combineMatSample(const cv::Mat& a, const cv::Mat& b);
cv::Mat getFaceSkinP(const cv::Mat& img);

#endif // PLANIMETRY_TOOLS_H

