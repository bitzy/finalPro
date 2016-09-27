#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H

#include "GLOBALHEADER.h"

class GLOBALFUNC {    
    static GLOBALFUNC represant;
    GLOBALFUNC(){}
    GLOBALFUNC(const GLOBALFUNC&);

public:
    static GLOBALFUNC* inst() {
        return &represant;
    }
    bool confirmFileExist(const QString& file);
    bool confirmDirExist(const QString& fpath);
    static QString& pathSlashAdd(QString &fpath);
    //void getStdPercent(const char* fpath, double &stdPercent);

    double getEuclideanDist(double p1x, double p1y, double p2x, double p2y);
    double getEuclideanDist(QPoint p1, QPoint p2);
};

#endif // GLOBALFUNC_H
