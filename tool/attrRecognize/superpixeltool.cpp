#include "superpixeltool.h"
#include <iostream>
using namespace std;
#include <opencv2/imgproc/imgproc.hpp> //findcontours
#include <opencv2/highgui/highgui.hpp> //imwrite
using namespace cv;

SuperPixelTool* SuperPixelTool::represant = 0;

SuperPixelTool::SuperPixelTool()
{
    flagok = false;
}

int SuperPixelTool::_getLabV(int x, int y)
{
    int* p = labels.ptr<int>(y);
    return *(p+x);
}

int SuperPixelTool::_getLabV(const Point p)
{
    return _getLabV(p.x, p.y);
}

Point SuperPixelTool::getlabelCenter(int i)
{    
    return labelCenter[i];
}

Point SuperPixelTool::getlabelCenter(const Point p)
{
    return labelCenter[_getLabV(p)];
}

void SuperPixelTool::setImg(const Mat& src) {
    //Simple Linear Iterative Clustering: SLIC
    slic = createSuperpixelSLIC(src, SLICO);
    //Superpixels Extracted via Energy-Driven Sampling
    //    Ptr<SuperpixelSEEDS> seeds = createSuperpixelSEEDS(
    //                src.cols, src.rows, src.channels(), 1000, 15);
    slic->iterate(25);
    slic->enforceLabelConnectivity();
    slic->getLabels(labels);//CV_32UC1
    flagok = true;

    //    Mat src2;
    //    slic->getLabelContourMask(src2);
    //calculate sp's center:
    int num = slic->getNumberOfSuperpixels();
    labelCenter.reserve(num);
    for(int i = 0; i < num; i++) {
        Mat mask;
        compare(labels, i, mask, CMP_EQ);
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL,
                     CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        int size = contours.size();
        if(size == 0) {
            cout << "contours none." << endl;
            flagok = false;return ;
        } else if(size == 1) {
            Moments mu;
            mu = moments(contours[0], false);
            Point mc(mu.m10/mu.m00, mu.m01/mu.m00);
            labelCenter[i] = mc;
        } else {
            int pos = 0;
            double area0 = contourArea(contours[0]);
            for(int j = 1; j < size; j++) {
                if(contourArea(contours[j]) > area0) pos = j;
            }
            Moments mu;
            mu = moments(contours[pos], false);
            Point mc(mu.m10/mu.m00, mu.m01/mu.m00);
            labelCenter[i] = mc;
        }
        //        circle(src2, labelCenter[i], 1, Scalar(255,0,0));
    }
    //show result:
    //    imwrite("center.jpg", src2);
}

/**
 * @brief SuperPixelTool::transform
 *  replace the pos points ==> super pixel's center position.
 * @param v
 */
int SuperPixelTool::transform(std::vector<Point>& v, int uppos)
{
    int upposnew = -1;
    assert(uppos >= 1);
    //cout << "sp---before: dots has " << v.size() << endl;
    if(flagok && v.size()) {
        std::vector<Point> dots;
        dots.assign(v.begin(), v.end());
        v.clear();

        int size = dots.size();
        v.push_back(getlabelCenter(dots[0]));
        int count = 1;
        for(; count < uppos; count++) {
            Point tmp = getlabelCenter(dots[count]);
            if(tmp == v.back()) continue;
            else v.push_back(tmp);
        }
        upposnew = v.size();
        for(; count < size; count++) {
            Point tmp = getlabelCenter(dots[count]);
            if(tmp == v.back()) continue;
            else v.push_back(tmp);
        }
    } else {
        cout << "sp---There isn't any image in superpixels." << endl;
    }
    return upposnew;
}
