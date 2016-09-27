#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include "GLOBALHEADER.h"

class GLOBALCONFIG {
    static GLOBALCONFIG represant;

    //attrconfig:
    static int maxAlternative;
    static QList<QString> attrName;
    static QList<QString> labelAttrName;
    static QList<QStringList> attrValues;

    //poseconfig:
    static QList<QString> poseName;
    static QList<int> poseTypeIndex;

    GLOBALCONFIG(){}
    GLOBALCONFIG(const GLOBALCONFIG&);
    static bool _init;
    static bool init();
public:
    //get instance:
    static GLOBALCONFIG* inst() {
        return &represant;
    }    

    int getMaxAlternative() const;
    QStringList getAttrs() const;
    QStringList getAllAttrs() const;
    int attrKind() const;
    int getAllKind() const;
    int getIndexByName(QString name) const;
    QString getAttrNameByIndex(int index) const;
    QStringList getAttrValuesByattrName(QString name) const;
    QStringList getAttrValuesByIndex(int index) const;
    int getIndexByAttrValue(int AttrIndex, QString value) const;
    QString getAttrValue(int kind, int index) const;

    int getPoseCounter() const;
    QStringList getPoseName() const;
    int getPoseIndexByName(QString name) const;
    QString getPoseNameByIndex(int index) const;
    int getPTypeIdxByIdx(int index) const;
};

#endif // GLOBALCONFIG_H
