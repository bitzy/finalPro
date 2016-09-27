#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

#include "GLOBALHEADER.h"
class GLOBALDEFINE {
public:
    static const QString SYS_LABELPATH_DEFAULT;
    static const QString SYS_BULKPATH_DEFAULT;
    static const QString SYS_DELPATH;
    static const QString SYS_BULKPATH_DATA_FNAME;
    static const QString SYSTEST_DIR;

    static const int SYSTEM_BASEDATA_TYPES;
    static vector<QString> BASEDATA_NAMES;
    static vector<int> BASEDATA_DATANUMS;
    static QStringList imageTypefilter;
    static bool _init;
private:
    static bool init();
};

#endif // GLOBALDEFINE_H

