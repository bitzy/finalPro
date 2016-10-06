#ifndef IMGDATA_H
#define IMGDATA_H

#include "attrRecognize/stdconfig.h"
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
    vector<Point2i> ManDotstd;
    vector<int> AttrVstd;

    Mat src;
    int POSETYPE;

    void _getManDotstd();
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
    //recognize function list;
};

#endif // IMGDATA_H
