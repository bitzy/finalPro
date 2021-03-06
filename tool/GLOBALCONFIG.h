#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include "GLOBALHEADER.h"

/**
 * @brief The GLOBALCONFIG class
 * DEFINE all attribute and pose data type.
 */
class GLOBALCONFIG {
    static GLOBALCONFIG represant;

    //attrconfig:
    static int maxAlternative;
    static QStringList attrName;
    static QStringList labelAttrName;
    static QList<QStringList > attrValues;
    //poseconfig:
    static QStringList poseName;
    static QList<int> poseTypeIndex;
    static bool _init;
    static bool init();
    GLOBALCONFIG(){}
    GLOBALCONFIG(const GLOBALCONFIG&);
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
