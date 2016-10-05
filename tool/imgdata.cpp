#include "imgdata.h"

ImgData::ImgData()
{
    MYIMG.clear();
    poseDatas.clear();
    attrDatas.clear();
    initFlag = false;
}

void ImgData::initial(const string img, const string xml)
{
    MYIMG = img;
    MYXML = xml;    
    initFlag = true;
}
