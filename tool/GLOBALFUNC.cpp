#include "GLOBALFUNC.h"

GLOBALFUNC GLOBALFUNC::represant;
void GLOBALFUNC::confirmDirExist(const QString &fpath) {
    if(access(fpath.toStdString().c_str(), F_OK) == -1) {
        QString cmd = QString("mkdir %1").arg(fpath);
        system(cmd.toStdString().c_str());
    }
}

QString &GLOBALFUNC::pathSlashAdd(QString &fpath)
{    
    int strLength = fpath.length();
    int slashPos = fpath.lastIndexOf("/");
    if(slashPos+1 != strLength) {
        fpath.append("/");
    }
    return fpath;
}

double GLOBALFUNC::getEuclideanDist(double p1x, double p1y, double p2x, double p2y)
{
    return sqrt((p1x - p2x) * (p1x - p2x) + (p1y - p2y) * (p1y - p2y));
}

double GLOBALFUNC::getEuclideanDist(QPoint p1, QPoint p2)
{
    return sqrt((double)(p1.x()-p2.x())*(p1.x()-p2.x())+(double)(p1.y()-p2.y())*(p1.y()-p2.y()));
}
