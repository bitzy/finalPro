#ifndef IMGDATA_H
#define IMGDATA_H

#include "GLOBALHEADER.h"

class ImgData
{
private:
    QStringList poseDatas;
    QStringList attrDatas;

public:
    ImgData(){}
    bool loadDataFromXML(const QString &img, const QString &xml);
};

#endif // IMGDATA_H
