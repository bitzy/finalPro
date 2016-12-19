#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include <QThread>
#include "GLOBALFUNC.h"

class runthread : public QThread
{
    Q_OBJECT

    QString path;
    QStringList images;
    QList<int>  runAttrs;
    QList<int>  runFuncs;
public:
    runthread(const QString dealPath,
              const QStringList imgs,
              const QList<int>& attrs,
              const QList<int>& ways);
    void dealBegin();
    void dealStop();

signals:
    void imgPerAttrDone(int);
    void finishedAll();

protected:
    void run();
};

#endif // RUNTHREAD_H
