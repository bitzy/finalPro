#include "imgdata.h"

#include "GLOBALCONFIG.h"
#include "GLOBALDEFINE.h"

#include <QDomDocument>

/*
bool ImgData::loadDataFromXML(const QString& img, const QString& xml)
{
    QFile file(xml);
    QDomDocument doc;
    doc.setContent(&file, false);
    QDomElement rootNode = doc.documentElement();

    QDomNode poseDataNode = rootNode.firstChild();
    int size = GLOBALCONFIG::inst()->getPoseCounter();
    for(int i = 0; i < size; i++) {
        QString poseName = GLOBALCONFIG::inst()->getPoseNameByIndex(i);
        QString poseNameValue = poseDataNode.firstChildElement(poseName).text();
        if(poseNameValue.isEmpty()) poseDatas.push_back("null");
        else poseDatas.push_back(poseNameValue);
    }

    QDomNode attrDataNode = poseDataNode.nextSibling();
    int size1 = GLOBALCONFIG::inst()->getAllKind();
    for(int i = 0; i < size1; i++) {
        QString attrName = GLOBALCONFIG::inst()->getAttrNameByIndex(i);
        QString attrNameValue = attrDataNode.firstChildElement(attrName).text();
        if(attrNameValue.isEmpty()) attrDatas.push_back("null");
        else attrDatas.push_back(attrNameValue);
    }
    file.close();
}
*/

ImgData::ImgData(string imgPath, string xmlPath)
{
    //
}

bool ImgData::loadDataFromXML(const string &img, const string &xml)
{
    //
    return false;
}

bool ImgData::loadImgData()
{
    /*/xml data:
    QFile file(xml);
    QDomDocument doc;
    if(doc.setContent(&file, false) == false) return false;
    QDomElement rootNode = doc.documentElement();

    QDomNode poseDataNode = rootNode.firstChild();
    int size = GLOBALCONFIG::inst()->getPoseCounter();
    poseDatas.clear();
    for(int i = 0; i < size; i++) {
        QString poseName = GLOBALCONFIG::inst()->getPoseNameByIndex(i);
        QString poseNameValue = poseDataNode.firstChildElement(poseName).text();
        if(poseNameValue.isEmpty()) poseDatas.push_back("null");
        else poseDatas.push_back(poseNameValue);
    }

    QDomNode attrDataNode = poseDataNode.nextSibling();
    int size1 = GLOBALCONFIG::inst()->getAllKind();
    attrDatas.clear();
    for(int i = 0; i < size1; i++) {
        QString attrName = GLOBALCONFIG::inst()->getAttrNameByIndex(i);
        QString attrNameValue = attrDataNode.firstChildElement(attrName).text();
        if(attrNameValue.isEmpty()) attrDatas.push_back("null");
        else attrDatas.push_back(attrNameValue);
    }
    file.close();
*/
    return true;
}
