#ifndef IMGDATA_H
#define IMGDATA_H

#include "stdconfig.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;

/**
 * @brief The ImgData class
 * This class used for image recognization.
 *
 * You must "1) setIMGpath;" first
 * to confirm src_ori is ok. then you do save & recognize operation.
 *
 * this is confirmed by OKFLAG variable.
 */

class ImgData
{
public:
    enum POSETYPE { TOP=1, HALF, ALL};
private:
    friend class ATTRWAYS;

    //---------- layout setting: -------------------------------
    bool okFlag;
    string MYIMG;
    string MYXML;
    //external function load standard poseDatas & attrDatas;
    vector<string> poseDatas;   //image standard pose data;
    vector<string> attrDatas;   //image standard attr data;
    //---------- layout setting: -------------------------------


    //---------- programing setting: ---------------------------
    bool preprocessFlag;        //images's process step;
    bool xmlDataLoadFlag;       //images's xml file;
    //recognize process related data;
    vector<int> attrVstd;

    cv::Mat src_ori;    //initial image;
    cv::Mat src;
    cv::Point2i head, neck, lshoud, rshoud, lelbow, relbow, lhand;
    cv::Point2i rhip, lknee, rknee, lank, rank, rhand, lhip;
    POSETYPE HSTYLE;

    //scale data;
    int xoffset, yoffset;
    double scale;
    //---------- programing setting: ---------------------------

public:    
    ImgData();
    bool getOKflag() const;
    void setIMGpath(const string img, const string xml);

    //inner variable:
    bool preprocessed() const;
    bool getxmlDataLoadFlag() const;
    void setXmlDataLoadFlag(const bool flag);
    const string& getMYIMG() const;
    const string& getMYXML() const;
    const vector<string> &getPoseDatas() const;
    const vector<string> &getAttrDatas() const;
    void setPoseDatas(const vector<string> &data);
    void setAttrDatas(const vector<string> &data);

    //function:
    void preprocess();
    //get image color at point;
    void getPointColor(const int x1, const int y1,
                       int&r, int&g, int&b) const;

private:
    void _loadManDotstd();
    void __resizePoint(cv::Point& p) const;
    void _resizePosePoints();
    cv::Rect __getBoundingBox();
    void _resizeSrcToFixedHeight(const cv::Rect& rect, int height = 800);
    cv::Vec3b _getPixelColor(const cv::Point& p) const;
};

#endif // IMGDATA_H
