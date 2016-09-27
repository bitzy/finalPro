#ifndef WAYSINTERFACE_H
#define WAYSINTERFACE_H

#include "../GLOBALCONFIG.h"
#include "../GLOBALHEADER.h"

//types of function:
typedef int(*ATTR_RECOGNITION_FUNC)(const char*, const char*);

class ATTRWAYS {
    static ATTRWAYS represant;
    class FUNCITEM {
    public:
        QString name;
        ATTR_RECOGNITION_FUNC address;
    };
    QList<QString> funcAttrType;
    QList< QList<FUNCITEM> > funcAttrWay;

    ATTRWAYS();
    ATTRWAYS(const ATTRWAYS&){}
public:
    static ATTRWAYS* instance() {
        return &represant;
    }

    QStringList getWays(QString name) const;
    ATTR_RECOGNITION_FUNC getAttrFuncAddress(QString attrName, int j) const;
};

#endif // WAYSINTERFACE_H

