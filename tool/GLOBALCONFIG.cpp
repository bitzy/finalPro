#include "GLOBALCONFIG.h"

GLOBALCONFIG GLOBALCONFIG::represant;
int GLOBALCONFIG::maxAlternative;
QStringList GLOBALCONFIG::attrName;
QStringList GLOBALCONFIG::labelAttrName;
QList<QStringList > GLOBALCONFIG::attrValues;
QStringList GLOBALCONFIG::poseName;
QList<int> GLOBALCONFIG::poseTypeIndex;
bool GLOBALCONFIG::_init = init();

#define sys_attrConfigFile "./configs/0attr.config"
#define sys_poseConfigFile "./configs/0pose.config"

bool GLOBALCONFIG::init() {
    QFile file(sys_attrConfigFile);
    if(!file.open(QFile::ReadOnly)) {
        qDebug() << "Cann't find file:" << sys_attrConfigFile << endl;
        qDebug() << "exit with ERROR code 10." << endl;
        exit(10);
    }
    while(!file.atEnd()) {
        QString line = file.readLine(255);
        QStringList tmpStr;
        tmpStr = line.simplified().split(":");
        if(tmpStr.size() != 2) {
            continue;
        }

        QStringList tmpValues;
        tmpValues = tmpStr.at(1).split(";");
        int tmpValueSize = tmpValues.size();
        if(tmpValueSize > maxAlternative)
            maxAlternative = tmpValueSize;

        if(tmpValueSize == 1 && tmpValues.at(0).isEmpty()) {
            labelAttrName.push_back(tmpStr.at(0));
        } else {
            attrName.push_back(tmpStr.at(0));
            attrValues.push_back(tmpValues);
        }
    }
    file.close();

    file.setFileName(sys_poseConfigFile);
    if(!file.open(QFile::ReadOnly)) {
        qDebug() << "Cann't find file:" << sys_poseConfigFile << endl;
        qDebug() << "exit with ERROR code 10." << endl;
        exit(10);
    }
    while(!file.atEnd()) {
        QString line = file.readLine(255);
        QStringList tmpStr;
        tmpStr = line.simplified().split(":");
        if(tmpStr.size() != 2) {
            continue;
        }

        poseName << tmpStr.at(0);
        poseTypeIndex << tmpStr.at(1).toInt();
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
    res = attrName;
    res += labelAttrName;
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
    int size = attrKind();
    for(int i = 0; i < size; i++) {
        if(!name.compare(attrName[i])) return i;
    }
    int size1 = (int)labelAttrName.size();
    for(int i = 0; i < size1; i++) {
        if(!name.compare(labelAttrName[i])) return i+size;
    }
    qDebug() << "WORNING:getIndexByName return error -1!" << endl;
    return -1;
}

QString GLOBALCONFIG::getAttrNameByIndex(int index) const
{
    if(index < attrKind()) return attrName[index];
    else return labelAttrName[index-attrKind()];
}

QStringList GLOBALCONFIG::getAttrValuesByattrName(QString name) const
{
    QStringList res;
    int size = attrKind();
    for(int i = 0; i < size; i++) {
        if(!attrName[i].compare(name)) return attrValues[i];
    }
    return res;
}

QStringList GLOBALCONFIG::getAttrValuesByIndex(int index) const
{
    QStringList res;
    if(index >= attrKind()) return res;
    return attrValues[index];
}

int GLOBALCONFIG::getIndexByAttrValue(int AttrIndex, QString value) const
{
    int size = attrValues[AttrIndex].size();
    for (int i = 0; i < size; i++) {
        if(!attrValues[AttrIndex][i].compare(value)) return i;
    }
    return -1;
}

QString GLOBALCONFIG::getAttrValue(int kind, int index) const
{
    return attrValues[kind][index];
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
    int size = getPoseCounter();
    for(int i = 0; i < size; i++){
        if(!poseName[i].compare(name)) return i;
    }
    return -1;
}

QString GLOBALCONFIG::getPoseNameByIndex(int index) const
{
    if(index >= getPoseCounter()) return QString("error");
    return poseName[index];
}

int GLOBALCONFIG::getPTypeIdxByIdx(int index) const
{
    if(index >= getPoseCounter()) return 0;
    return poseTypeIndex[index];
}

