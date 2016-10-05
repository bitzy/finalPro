#include "GLOBALCONFIG.h"

GLOBALCONFIG GLOBALCONFIG::represant;

int GLOBALCONFIG::getMaxAlternative() const
{
    return STDCONFIG::INST()->maxAlternative;
}

QStringList GLOBALCONFIG::getAttrs() const
{
    QStringList res;
    int size = (int)STDCONFIG::INST()->attrName.size();
    for(int i = 0; i < size; i++) {
        QString tmp = QString::fromStdString(STDCONFIG::INST()->attrName[i]);
        res.push_back(tmp);
    }
    return res;
}

QStringList GLOBALCONFIG::getAllAttrs() const
{
    QStringList res;
    res << getAttrs();
    int size = (int)STDCONFIG::INST()->labelAttrName.size();
    for(int i = 0; i < size; i++) {
        QString tmp = QString::fromStdString(STDCONFIG::INST()->labelAttrName[i]);
        res.push_back(tmp);
    }
    return res;
}

int GLOBALCONFIG::attrKind() const
{
    return STDCONFIG::INST()->attrName.size();
}

int GLOBALCONFIG::getAllKind() const
{
    return attrKind() + STDCONFIG::INST()->labelAttrName.size();
}

int GLOBALCONFIG::getIndexByName(QString name) const
{
    string nameStr = name.toStdString();
    int size = attrKind();
    for(int i = 0; i < size; i++) {
        if(!STDCONFIG::INST()->attrName[i].compare(nameStr))
            return i;
    }
    int size1 = (int)STDCONFIG::INST()->labelAttrName.size();
    for(int i = 0; i < size1; i++) {
        if(!STDCONFIG::INST()->labelAttrName[i].compare(nameStr))
            return i+size;
    }
    return -1;
}

QString GLOBALCONFIG::getAttrNameByIndex(int index) const
{
    string res;
    if(index < attrKind()) {
        res = STDCONFIG::INST()->attrName[index];
    } else {
        res = STDCONFIG::INST()->labelAttrName[index-attrKind()];
    }
    return QString::fromStdString(res);
}

QStringList GLOBALCONFIG::getAttrValuesByattrName(QString name) const
{
    QStringList res;
    string nameStr = name.toStdString();
    int size = attrKind();
    for(int i = 0; i < size; i++) {
        if(!STDCONFIG::INST()->attrName[i].compare(nameStr)) {
            int size1 = STDCONFIG::INST()->attrValues[i].size();
            for(int j = 0; j < size1; j++) {
                string tmp = STDCONFIG::INST()->attrValues[i][j];
                res.push_back(QString::fromStdString(tmp));
            }
        }
    }
    return res;
}

QStringList GLOBALCONFIG::getAttrValuesByIndex(int index) const
{
    QStringList res;
    if(index >= attrKind()) return res;
    int size = STDCONFIG::INST()->attrValues[index].size();
    for(int i = 0; i < size; i++) {
        string tmp = STDCONFIG::INST()->attrValues[index][i];
        res.push_back(QString::fromStdString(tmp));
    }
    return res;
}

int GLOBALCONFIG::getIndexByAttrValue(int AttrIndex, QString value) const
{
    string valueStr = value.toStdString();
    int size = STDCONFIG::INST()->attrValues[AttrIndex].size();
    for (int i = 0; i < size; i++) {
        if(!STDCONFIG::INST()->attrValues[AttrIndex][i].compare(valueStr)) {
            return i;
        }
    }
    return -1;
}

QString GLOBALCONFIG::getAttrValue(int kind, int index) const
{
    string tmp = STDCONFIG::INST()->attrValues[kind][index];
    return QString::fromStdString(tmp);
}

int GLOBALCONFIG::getPoseCounter() const
{
    return STDCONFIG::INST()->poseName.size();
}

QStringList GLOBALCONFIG::getPoseName() const
{
    QStringList res;
    int size = (int)STDCONFIG::INST()->poseName.size();
    for(int i = 0; i < size; i++) {
        string tmp = STDCONFIG::INST()->poseName[i];
        res.push_back(QString::fromStdString(tmp));
    }
    return res;
}

int GLOBALCONFIG::getPoseIndexByName(QString name) const
{
    string nameStr = name.toStdString();
    int size = getPoseCounter();
    for(int i = 0; i < size; i++){
        if(!STDCONFIG::INST()->poseName[i].compare(nameStr))
            return i;
    }
    return -1;
}

QString GLOBALCONFIG::getPoseNameByIndex(int index) const
{
    if(index >= getPoseCounter()) return QString("error");
    string tmp = STDCONFIG::INST()->poseName[index];
    return  QString::fromStdString(tmp);
}

int GLOBALCONFIG::getPTypeIdxByIdx(int index) const
{
    if(index >= getPoseCounter()) return 0;
    return STDCONFIG::INST()->poseTypeIndex[index];
}

