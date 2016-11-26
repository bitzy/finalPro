#ifndef SUPERPIXELTOOL_H
#define SUPERPIXELTOOL_H

#include <vector>
#include <string>
#include <opencv2/core/core.hpp> //mat
#include <opencv2/ximgproc.hpp> //superpixels
using namespace cv::ximgproc;

/**
 * @brief The SuperPixelTool class
 * provide imgdata's superpixels info.
 */
class SuperPixelTool
{
    static SuperPixelTool* represant;
    SuperPixelTool();
    SuperPixelTool(const SuperPixelTool&);
    bool flagok;
    cv::Ptr<SuperpixelSLIC> slic;
    cv::Mat labels;
    std::vector<cv::Point> labelCenter;

    int _getLabV(int x, int y);
    int _getLabV(const cv::Point p);
    cv::Point getlabelCenter(int i);
    cv::Point getlabelCenter(const cv::Point p);
public:
    static SuperPixelTool& INST() {
        if(represant == NULL) {
            represant = new SuperPixelTool;
        }

        return *represant;
    }
    void setImg(const cv::Mat& src);
    int transform(std::vector<cv::Point> &v, int uppos);
};

#endif // SUPERPIXELTOOL_H
