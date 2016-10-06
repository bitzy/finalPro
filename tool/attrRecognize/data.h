#ifndef DATA_H
#define DATA_H

#include "stdconfig.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

class Data
{
    string imgpath;
    Mat img;

public:
    Data();
    bool compareSame(string pimg);
    void initial(string pimg, const vector<string> &p, const vector<string> &a);
};

#endif // DATA_H
