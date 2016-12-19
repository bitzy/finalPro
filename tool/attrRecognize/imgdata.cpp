#include "imgdata.h"

#include <iostream>
using namespace std;
#include <opencv2/highgui/highgui.hpp> //imread

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

//#define IMGDATADEBUG
#ifdef IMGDATADEBUG
#include <opencv2/core/core.hpp>
using namespace cv;
#endif

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

ImgData::ImgData(){
    xmlPathsOKFlag = false;
    poseDataOKFlag = false;
    attrDataOKFlag = false;
    loadimgsOKFlag = false;
    preprocessFlag = false;
    pointOKFlag = false;
}

bool ImgData::getXmlPathsOKflag() const
{
    return xmlPathsOKFlag;
}

bool ImgData::getPoseDataOKflag() const
{
    return poseDataOKFlag;
}

bool ImgData::getAttrDataOKflag() const
{
    return attrDataOKFlag;
}

bool ImgData::getLoadimgsOKflag() const
{
    return loadimgsOKFlag;
}

bool ImgData::getPreprocessflag() const
{
    return preprocessFlag;
}

bool ImgData::getPointOKflag() const
{
    return pointOKFlag;
}

void ImgData::loadIMGsrc(const string img)
{
    MYIMG = img;
    src_ori = cv::imread(MYIMG);
    loadimgsOKFlag = true;
    //when load a img's:
    xmlPathsOKFlag = false;
    poseDataOKFlag = false;
    attrDataOKFlag = false;
    preprocessFlag = false;
    pointOKFlag = false;
    string tmp = img.substr(0, img.find_last_of("."));
    tmp += ".xml";
    setDefaultXMLPath(tmp);
}

void ImgData::loadIMGsrc(const string img, const string xml)
{
    MYIMG = img;
    src_ori = cv::imread(MYIMG);
    loadimgsOKFlag = true;

    DBxmlPATH = xml;
    xmlPathsOKFlag = true;
    //when load a img's:
    poseDataOKFlag = false;
    attrDataOKFlag = false;
    preprocessFlag = false;
    pointOKFlag = false;
    _loadPoseDatafromxml();
}

const string &ImgData::getMYIMG() const
{
    if(loadimgsOKFlag) return MYIMG;
    cout << "img not set yet!" << endl;
    exit(20);
}

void ImgData::setDefaultXMLPath(string str)
{
    this->DBxmlPATH = str;
    xmlPathsOKFlag = true;
}

const string &ImgData::getXMLPath() const
{
    if(xmlPathsOKFlag) return DBxmlPATH;
    cout << "xml path not set yet!" << endl;
    exit(20);
}

const vector<string>& ImgData::getPoseDatas() const
{
    if(poseDataOKFlag) return poseDatas;
    cout << "pose data not load yet!" << endl;
    exit(20);
}

const vector<string>& ImgData::getAttrDatas() const
{
    if(attrDataOKFlag) return attrDatas;
    cout << "attr data not load yet!" << endl;
    exit(20);
}

void ImgData::setPoseDatas(const vector<string> &data)
{    
    poseDatas.assign(data.begin(), data.end());
    poseDataOKFlag = true;
}

void ImgData::setAttrDatas(const vector<string> &data)
{
    attrDatas.assign(data.begin(), data.end());
    attrDataOKFlag = true;
}

//do the process step for all images.
void ImgData::preprocess()
{
    if(!getLoadimgsOKflag() || !getPoseDataOKflag()) {
        cout << "IMG/POSEDATA not load yet!" << endl;
        cout << "exit with ERROR code 12." << endl;
        exit(12);
    }

    //preprocess for all image:
    _loadPoints(); //get man dot point from poseDatas;
    _resizeSrcToFixedHeight(__getBoundingBox()); //fixed hegiht;
    preprocessFlag = true;
}

void ImgData::getPColorFromIMG(const int x1, const int y1, int &r, int &g, int &b) const
{
    if(getLoadimgsOKflag()) {
        cv::Vec3b color = _getPixelColorFromSRC_ORI(cv::Point(x1, y1));
        r = color[2];
        g = color[1];
        b = color[0];
    } else r = g = b = 0;
}

string selectSingleNode(TiXmlElement* root, string nodeName) {
    string res;
    if(root == NULL) return res;
    TiXmlElement* pNode = root->FirstChildElement();
    for(; pNode; pNode = pNode->NextSiblingElement()) {
        if(!nodeName.compare(pNode->Value())) {
            res = pNode->GetText();
            //cout <<  res << endl;
        }
    }
    return res;
}

void ImgData::_loadPoseDatafromxml()
{
    if(xmlPathsOKFlag) {
        //loadxml2imgdata(DBxmlPATH, *this);
        try{
            vector<string> posedata;
            vector<string> attrdata;
            TiXmlDocument *myDocument = new TiXmlDocument(DBxmlPATH.c_str());
            myDocument->LoadFile();
            TiXmlElement *rootElement = myDocument->RootElement();
            //extract posedata&attrdata by tixmltlement

            TiXmlElement* poseDataNode = rootElement->FirstChildElement();
            int size = STDCONFIG::INST()->getPoseCounter();
            for(int i = 0; i < size; i++) {
                string poseName = STDCONFIG::INST()->getPoseNameByIndex(i);
                string poseNameValue = selectSingleNode(poseDataNode, poseName);
                if(poseNameValue.empty()) posedata.push_back("null");
                else posedata.push_back(poseNameValue);
            }

            TiXmlElement* attrDataNode = poseDataNode->NextSiblingElement();
            int size1 = STDCONFIG::INST()->getAllKind();
            for(int i = 0; i < size1; i++) {
                string attrName = STDCONFIG::INST()->getAttrNameByIndex(i);
                string attrNameValue = selectSingleNode(attrDataNode, attrName);
                if(attrNameValue.empty()) attrdata.push_back("null");
                else attrdata.push_back(attrNameValue);
            }
            //set imgdata:
            setPoseDatas(posedata);
            setAttrDatas(attrdata);
        } catch(string& e) {
            cout << e << endl;
        }
    }
}


void ImgData::_loadPoints()
{
    if(!getPoseDataOKflag()) {
        cout << "pose data not load yet!" << endl;
        exit(20);
    }
    vector<cv::Point2i> manDotstd;
    int size = (int)poseDatas.size();
    for(int i = 0; i < size; i++) {
        int x = -1, y = -1;
        sscanf(poseDatas[i].c_str(), "%d,%d", &x, &y);
        manDotstd.push_back(cv::Point2i(x, y));
    }

    //from stdconfig get posename corresponding index;
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
    pointOKFlag = true;
}

void ImgData::__resizePoint(cv::Point &p) const
{    
    p.x = (p.x-xoffset)*scale;
    p.y = (p.y-yoffset)*scale;
}

void ImgData::_resizePosePoints()
{
    if(!getPointOKflag()) {
        cout << "points not load yet!" << endl;
        exit(20);
    }
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

cv::Rect ImgData::__getBoundingBox()
{
    if(!getPointOKflag() || !getLoadimgsOKflag()) {
        cout << "img not set or points not load yet!" << endl;
        exit(20);
    }
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

    int marginVertical = (bottom - top) / 8;
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

    cv::Rect res(cv::Point(outLeft, outTop), cv::Point(outRight, outBottom));
#ifdef IMGDATADEBUG
    cout << outLeft << "," << outTop << ","
         << outRight << "," << outBottom << endl;
    cv::rectangle(src_ori, res, cv::Scalar(255,0,0));
    imwrite("rect.jpg", src_ori);
#endif
    return res;
}


void ImgData::_resizeSrcToFixedHeight(const cv::Rect &rect, int height)
{
    if(!getLoadimgsOKflag()) {
        cout << "img not set yet!" << endl;
        exit(20);
    }
#ifdef IMGDATADEBUG
    cout << "img_ori's size" << src_ori.cols << "x"
         << src_ori.rows << endl;
#endif
    xoffset = rect.x;
    yoffset = rect.y;
    cv::Mat focusImgROI = src_ori(rect);
    scale = 1.0*height/focusImgROI.rows;
    resize(focusImgROI, src, cv::Size(), scale, scale);

    //resize Point
    _resizePosePoints();
#ifdef IMGDATADEBUG
    cout << "src's size" << src.cols << "x"
         << src.rows << endl;
    line(src, lshoud, lelbow, Scalar(0,255,0));
    imwrite("scaleimg.jpg", src);
#endif
}

cv::Vec3b ImgData::_getPixelColorFromSRC_ORI(const cv::Point &p) const
{
    if(!getLoadimgsOKflag()) {
        cout << "img not set yet!" << endl;
        exit(20);
    }
    cv::Vec3b color = src_ori.at<cv::Vec3b>(p.y, p.x);
    return color;
}

cv::Vec3b ImgData::_getPixelColorFromSrc(const cv::Point &p) const
{
    cv::Vec3b color = src.at<cv::Vec3b>(p.y, p.x);
    return color;
}

int ImgData::fminvalue6(int x, int y, int z1, int z2, int z3, int z4)
{
    int res = (min)(x,y);
    res = (min)(res, z1);
    res = (min)(res, z2);
    res = (min)(res, z3);
    res = (min)(res, z4);
    return res;
}

int ImgData::fmaxvalue6(int x, int y, int z1, int z2, int z3, int z4)
{
    int res = (max)(x,y);
    res = (max)(res, z1);
    res = (max)(res, z2);
    res = (max)(res, z3);
    res = (max)(res, z4);
    return res;
}
