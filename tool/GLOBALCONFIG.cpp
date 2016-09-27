#include "GLOBALCONFIG.h"

GLOBALCONFIG GLOBALCONFIG::represant;

int GLOBALCONFIG::maxAlternative;
QList<QString> GLOBALCONFIG::attrName;
QList<QString> GLOBALCONFIG::labelAttrName;
QList<QStringList> GLOBALCONFIG::attrValues;
QList<QString> GLOBALCONFIG::poseName;
QList<int> GLOBALCONFIG::poseTypeIndex;
bool GLOBALCONFIG::_init = init();

#define sys_attrConfigFile "./configs/0attr.config"
#define sys_poseConfigFile "./configs/0pose.config"
#include <QDebug>

bool GLOBALCONFIG::init()
{
    //attr config load:
    QFile file(sys_attrConfigFile);
    if(!file.open(QFile::ReadOnly)) {        
        exit(10);
    }
    maxAlternative = -1;
    while(!file.atEnd()) {
        QString line = file.readLine(255).simplified();
        QStringList tmpStr = line.split(":");

        QString tmpName = tmpStr.at(0);
        QStringList tmpValues = tmpStr.at(1).split(";");
        int tmpSize = tmpValues.size();
        if(maxAlternative < tmpSize) {
            maxAlternative = tmpSize;
        }
        if(tmpSize == 1 && tmpValues.at(0).isEmpty()) {
            labelAttrName.push_back(tmpStr.at(0));
        } else {
            attrName.push_back(tmpName);
            attrValues.push_back(tmpValues);
        }
    }
    file.close();

    //pose config load:
    file.setFileName(sys_poseConfigFile);
    if(!file.open(QFile::ReadOnly)) exit(10);
    while(!file.atEnd()) {
        QString line = file.readLine(255).simplified();
        QStringList tmpStr = line.split(":");
        poseName.push_back(tmpStr.at(0));
        poseTypeIndex.push_back(tmpStr.at(1).toInt());
    }
    file.close();
    return true;
}

int GLOBALCONFIG::getMaxAlternative() const
{
    return maxAlternative;
}

QStringList GLOBALCONFIG::getAttrs() const
{
    return attrName;
}

QStringList GLOBALCONFIG::getAllAttrs() const
{
    QStringList res;
    res << attrName << labelAttrName;
    return res;
}

int GLOBALCONFIG::attrKind() const
{
    return attrName.size();
}

int GLOBALCONFIG::getAllKind() const
{
    return attrName.size() + labelAttrName.size();
}

int GLOBALCONFIG::getIndexByName(QString name) const
{
    int size = attrName.size();
    for(int i = 0; i < size; i++) {
        if(!attrName.at(i).compare(name))
            return i;
    }
    size = labelAttrName.size();
    for(int i = 0; i < size; i++) {
        if(!labelAttrName.at(i).compare(name))
            return i+attrName.size();
    }
    return -1;
}

QString GLOBALCONFIG::getAttrNameByIndex(int index) const
{
    if(index < attrName.size())
        return attrName.at(index);
    return labelAttrName.at(index - attrName.size());
}

QStringList GLOBALCONFIG::getAttrValuesByattrName(QString name) const
{
    int size = attrName.size();
    for(int i = 0; i < size; i++) {
        if(!attrName.at(i).compare(name))
            return attrValues.at(i);
    }
    return QStringList();
}

QStringList GLOBALCONFIG::getAttrValuesByIndex(int index) const
{
    if(index >= attrValues.size()) return QStringList();
    return attrValues.at(index);
}

int GLOBALCONFIG::getIndexByAttrValue(int AttrIndex, QString value) const
{
    int size = attrValues.at(AttrIndex).size();
    for (int i = 0; i < size; i++) {
        if(attrValues.at(AttrIndex).at(i).compare(value) == 0) {
            return i;
        }
    }
    return -1;
}

QString GLOBALCONFIG::getAttrValue(int kind, int index) const
{
    QStringList tmp = this->attrValues.at(kind);
    return tmp.at(index);
}

int GLOBALCONFIG::getPoseCounter() const
{
    return poseName.size();
}

QStringList GLOBALCONFIG::getPoseName() const
{
    return poseName;
}

int GLOBALCONFIG::getPoseIndexByName(QString name) const
{
    for(int i = 0; i < poseName.size(); i++){
        if(!poseName.at(i).compare(name)) return i;
    }
    return -1;
}

QString GLOBALCONFIG::getPoseNameByIndex(int index) const
{
    if(index >= poseName.size()) return QString("error");
    return  this->poseName.at(index);
}

int GLOBALCONFIG::getPTypeIdxByIdx(int index) const
{
    if(index >= poseName.size()) return 0;
    return this->poseTypeIndex.at(index);
}

