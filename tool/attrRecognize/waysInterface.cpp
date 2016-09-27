#include "waysInterface.h"

ATTRWAYS ATTRWAYS::represant;

ATTRWAYS::ATTRWAYS()
{
    funcAttrType = GLOBALCONFIG::inst()->getAttrs();
    QList<FUNCITEM> tmp;
    funcAttrWay.push_back(tmp);

    //sleeve:
    tmp.clear();
    funcAttrWay.push_back(tmp);

    //up lenth:
    tmp.clear();
    funcAttrWay.push_back(tmp);

    //texture:
    tmp.clear();
    funcAttrWay.push_back(tmp);
}

QStringList ATTRWAYS::getWays(QString name) const
{
    int size = funcAttrType.size();
    int pos = -1;
    for(int i = 0; i < size; i++) {
        if(!name.compare(funcAttrType[i])) {
            pos = i;
            break;
        }
    }
    QStringList res;
    if (pos == -1) return res;

    int waysNum = funcAttrWay[pos].size();
    for(int i = 0; i < waysNum; i++) {
        res << funcAttrWay[pos][i].name;
    }
    return res;
}

ATTR_RECOGNITION_FUNC ATTRWAYS::getAttrFuncAddress(QString attrName, int j) const
{
    int size = funcAttrType.size();
    int pos = -1;
    for(int i = 0; i < size; i++) {
        if(!attrName.compare(funcAttrType[i])) {
            pos = i;
            break;
        }
    }
    if (pos == -1) exit(20);
    return funcAttrWay[pos][j].address;
}

