#include "imgdata.h"
#include "mytool.h"
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

ImgData::ImgData() {}

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
    preprocessFlag = false;
    xmlDataLoadFlag = false;
}

//do the process step for all images.
void ImgData::preprocess()
{
    if(!getxmlDataLoadFlag()) {
        cout << "ERROR:img xml Data not loaded!" << endl;
        exit(12);
    }
    src = imread(MYIMG);
    _loadManDotstd();
    _resizeSrcToFixedHeight(__getBoundingBox());
    preprocessFlag = true;
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

void ImgData::__resizePoint(Point &p, int xoffset, int yoffset, double scale)
{
    p.x = (p.x-xoffset)*scale;
    p.y = (p.y-yoffset)*scale;
}

void ImgData::_resizePosePoints(int xoffset, int yoffset, double scale)
{
    __resizePoint(head, xoffset, yoffset, scale);
    __resizePoint(neck, xoffset, yoffset, scale);
    __resizePoint(lshoud, xoffset, yoffset, scale);
    __resizePoint(rshoud, xoffset, yoffset, scale);
    __resizePoint(lelbow, xoffset, yoffset, scale);
    __resizePoint(relbow, xoffset, yoffset, scale);
    __resizePoint(lhand, xoffset, yoffset, scale);
    __resizePoint(rhand, xoffset, yoffset, scale);
    __resizePoint(lhip, xoffset, yoffset, scale);
    __resizePoint(rhip, xoffset, yoffset, scale);
    __resizePoint(lknee, xoffset, yoffset, scale);
    __resizePoint(rknee, xoffset, yoffset, scale);
    __resizePoint(lank, xoffset, yoffset, scale);
    __resizePoint(rank, xoffset, yoffset, scale);
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
                HSTYLE = POSETYPE::ALL;
            } else HSTYLE = POSETYPE::HALF;
        } else {
            HSTYLE = POSETYPE::TOP;
        }
    }
    int right = fmaxvalue6(rshoud.x, relbow.x, lelbow.x, lhand.x, rhip.x);
    {
        if(HSTYLE == POSETYPE::HALF) {
            right = fmaxvalue6(right, lknee.x, rknee.x);
        } else if(HSTYLE == POSETYPE::ALL) {
            right = fmaxvalue6(right, lknee.x, rknee.x, lank.x, rank.x);
        }
    }
    int bottom = fmaxvalue6(lelbow.y, lhand.y, relbow.y, rhand.y, lhip.y, rhip.y);
    {
        if(HSTYLE == POSETYPE::HALF) {
            bottom = fmaxvalue6(bottom, lknee.y, rknee.y);
        } else {
            bottom = fmaxvalue6(bottom, lknee.y, rknee.y, lank.y, rank.y);
        }
    }

    int marginVertical = (bottom - top) / 5;
    int marginHorinzol = (right - left) / 5;
    int imgwidth = src.cols - 1;
    int imgheight = src.rows - 1;

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
    int xoffset = rect.x, yoffset = rect.y;
    Mat focusImgROI = src(rect);
    double scale = 1.0*height/focusImgROI.rows;
    resize(src, src, Size(), scale, scale);

    //resize Point
    _resizePosePoints(xoffset, yoffset, scale);
}
