#ifndef IMGDATA_H
#define IMGDATA_H

#include "stdconfig.h"
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

/**
 * @brief The ImgData class
 * This class used for image recognization.
 */
class ImgData
{
private:
    friend class ATTRWAYS;      //ATTREAYS class can use imgData.

    bool preprocessFlag;
    bool xmlDataLoadFlag;
    string MYIMG;
    string MYXML;

    //external function load standard poseDatas & attrDatas;
    vector<string> poseDatas;   //image standard pose data;
    vector<string> attrDatas;   //image standard attr data;

    //recognize process related data;    
    vector<int> attrVstd;

    Mat src;
    Point2i head, neck, lshoud, rshoud, lelbow, relbow, lhand;
    Point2i rhand, lhip, rhip, lknee, rknee, lank, rank;
    enum POSETYPE { TOP=1, HALF, ALL};
    POSETYPE HSTYLE;

public:
    ImgData();
    bool preprocessed() const;
    bool getxmlDataLoadFlag() const;
    void setXmlDataLoadFlag(const bool flag);
    const string& getMYIMG() const;
    const string& getMYXML() const;
    const vector<string> &getPoseDatas() const;
    const vector<string> &getAttrDatas() const;
    void setPoseDatas(const vector<string> &data);
    void setAttrDatas(const vector<string> &data);

    //You must set img's path & corresponding xml's path;
    void loadImg(const string img, const string xml);
    void preprocess();
private:
    void _loadManDotstd();
    void __resizePoint(Point& p, int xoffset, int yoffset, double scale);
    void _resizePosePoints(int xoffset, int yoffset, double scale);
    Rect __getBoundingBox();
    void _resizeSrcToFixedHeight(const Rect& rect, int height = 800);
};

#endif // IMGDATA_H
