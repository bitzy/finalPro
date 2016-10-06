#include "imgdata.h"

void ImgData::_getManDotstd()
{
    int size = (int)poseDatas.size();
}

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
    src = imread(img);
    //get pose data;
}

//do the process step for all images.
void ImgData::preprocess()
{
    if(!getxmlDataLoadFlag()) {
        cout << "ERROR:img xml Data not loaded!" << endl;
        exit(12);
    }
    preprocessFlag = true;
}
