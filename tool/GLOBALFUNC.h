#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H

#include "GLOBALHEADER.h"
#include "attrRecognize/waysInterface.h"    //provide the ways interface
/**
 * @brief The GLOBALFUNC class
 *  forms used function.
 */
class GLOBALFUNC {    
    static GLOBALFUNC represant;
    GLOBALFUNC(){}
    GLOBALFUNC(const GLOBALFUNC&);

public:
    static GLOBALFUNC* inst() {
        return &represant;
    }
    static QString& pathSlashAdd(QString &fpath);
    bool confirmFileExist(const QString& file);
    bool confirmDirExist(const QString& fpath);
    QStringList stdvec2qvec(const std::vector<std::string> &v);
    std::vector<std::string> qvec2stdvec(const QStringList &v);
    //void getStdPercent(const char* fpath, double &stdPercent);

    double getEuclideanDist(double p1x, double p1y, double p2x, double p2y);
    double getEuclideanDist(QPoint p1, QPoint p2);
};

#endif // GLOBALFUNC_H
