#include "imgdata.h"
#include "mytool.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
//=================== POSE NAME =============
#define PH  "head"
#define PN  "neck"
#define PLS "left_shoulder"
#define PLE "left_elbow"
#define PLH "left_hand"
#define PRS "right_shoulder"
#define PRE "right_elbow"
#define PRH "right_hand"
#define PLI "left_hip"
#define PLK "left_knee"
#define PLA "left_ankle"
#define PRI "right_hip"
#define PRK "right_knee"
#define PRA "right_ankle"
//=================== POSE NAME =============

ImgData::ImgData() {
}

bool ImgData::preprocessed() const
{
    return preprocessFlag;
}

bool ImgData::getxmlDataLoadFlag() const
{
    return xmlDataLoadFlag;
}

void ImgData::setXmlDataLoadFlag(const bool flag)
{
    xmlDataLoadFlag = flag;
}

const string &ImgData::getMYIMG() const
{
    return MYIMG;
}

const string &ImgData::getMYXML() const
{
    return MYXML;
}

const vector<string>& ImgData::getPoseDatas() const
{
    return poseDatas;
}

const vector<string> &ImgData::getAttrDatas() const
{
    return attrDatas;
}

void ImgData::setPoseDatas(const vector<string> &data)
{
    poseDatas = data;
}

void ImgData::setAttrDatas(const vector<string> &data)
{
    attrDatas = data;
}


void ImgData::loadImg(const string img, const string xml)
{
    MYIMG = img;
    MYXML = xml;
    src_ori = cv::imread(MYIMG);

    preprocessFlag = false;
    xmlDataLoadFlag = false;
}

//do the process step for all images.
void ImgData::preprocess()
{
    if(!getxmlDataLoadFlag()) {        
        cout << "Data Struct not load xml data yet!" << endl;
        cout << "exit with ERROR code 12." << endl;
        exit(12);
    }    
    _loadManDotstd();
    _resizeSrcToFixedHeight(__getBoundingBox());
    preprocessFlag = true;
}

void ImgData::getPointColor(const int x1, const int y1, int &r, int &g, int &b) const
{
    Vec3b color = _getPixelColor(Point(x1, y1));
    r = color[2];
    g = color[1];
    b = color[0];
}


void ImgData::_loadManDotstd()
{
    vector<Point2i> manDotstd;
    int size = (int)poseDatas.size();
    for(int i = 0; i < size; i++) {
        int x = -1, y = -1;
        sscanf(poseDatas[i].c_str(), "%d,%d", &x, &y);
        manDotstd.push_back(Point2i(x, y));
    }

    head = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PH)];
    neck = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PN)];
    lshoud = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PLS)];
    rshoud = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PRS)];
    lelbow = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PLE)];
    relbow = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PRE)];
    lhand = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PLH)];
    rhand = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PRH)];
    lhip = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PLI)];
    rhip = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PRI)];
    lknee = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PLK)];
    rknee = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PRK)];
    lank = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PLA)];
    rank = manDotstd[STDCONFIG::INST()->getPoseIndexByName(PRA)];
}

void ImgData::__resizePoint(Point &p) const
{
    p.x = (p.x-xoffset)*scale;
    p.y = (p.y-yoffset)*scale;
}

void ImgData::_resizePosePoints()
{
    __resizePoint(head);
    __resizePoint(neck);
    __resizePoint(lshoud);
    __resizePoint(rshoud);
    __resizePoint(lelbow);
    __resizePoint(relbow);
    __resizePoint(lhand);
    __resizePoint(rhand);
    __resizePoint(lhip);
    __resizePoint(rhip);
    __resizePoint(lknee);
    __resizePoint(rknee);
    __resizePoint(lank);
    __resizePoint(rank);
}

Rect ImgData::__getBoundingBox()
{
    int top = fminvalue6(head.y, lelbow.y, relbow.y, lhand.y, rhand.y);
    int left = fminvalue6(lshoud.x, lelbow.x, lhand.x, relbow.x, rhand.x, lhip.x);
    {
        if(lknee.x != -1) {
            left = fminvalue6(left, lknee.x, rknee.x);
            if(lank.x != -1) {
                left = fminvalue6(left, lank.x, rank.x);
                HSTYLE = ImgData::ALL;
            } else HSTYLE = ImgData::HALF;
        } else {
            HSTYLE = ImgData::TOP;
        }
    }
    int right = fmaxvalue6(rshoud.x, relbow.x, lelbow.x, lhand.x, rhip.x);
    {
        if(HSTYLE == ImgData::HALF) {
            right = fmaxvalue6(right, lknee.x, rknee.x);
        } else if(HSTYLE == ImgData::ALL) {
            right = fmaxvalue6(right, lknee.x, rknee.x, lank.x, rank.x);
        }
    }
    int bottom = fmaxvalue6(lelbow.y, lhand.y, relbow.y, rhand.y, lhip.y, rhip.y);
    {
        if(HSTYLE == ImgData::HALF) {
            bottom = fmaxvalue6(bottom, lknee.y, rknee.y);
        } else {
            bottom = fmaxvalue6(bottom, lknee.y, rknee.y, lank.y, rank.y);
        }
    }

    int marginVertical = (bottom - top) / 5;
    int marginHorinzol = (right - left) / 5;
    int imgwidth = src_ori.cols - 1;
    int imgheight = src_ori.rows - 1;

    int outTop = top - marginVertical;
    outTop = (outTop < 0? 0:outTop);
    int outRight = right + marginHorinzol;
    outRight = (outRight > imgwidth? imgwidth:outRight);
    int outLeft = left - marginHorinzol;
    outLeft = (outLeft < 0?0:outLeft);
    int outBottom = bottom + marginVertical;
    outBottom = (outBottom > imgheight?imgheight:outBottom);

    Rect res(Point(outLeft, outTop), Point(outRight, outBottom));
    return res;
}


void ImgData::_resizeSrcToFixedHeight(const Rect &rect, int height)
{    
    xoffset = rect.x;
    yoffset = rect.y;
    Mat focusImgROI = src_ori(rect);
    scale = 1.0*height/focusImgROI.rows;
    resize(src_ori, src, Size(), scale, scale);

    //resize Point
    _resizePosePoints();
}

Vec3b ImgData::_getPixelColor(const Point &p) const
{
    Vec3b color = src_ori.at<Vec3b>(p.y, p.x);
    return color;
}
