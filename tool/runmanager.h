#ifndef RUNMANAGER_H
#define RUNMANAGER_H

#include <QDialog>
#include "GLOBALFUNC.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QString;
class QPushButton;
class QStandardItemModel;
class runthread;
QT_END_NAMESPACE

class RunManager : public QDialog
{
    Q_OBJECT
    QLabel *dealCount;
    QLabel *sumCounts;
    QLabel *dealPath;
    QLabel *savePath;
    QPushButton *saveBtn;
    QPushButton *stopBtn;
    QStandardItemModel *model;

    bool configExist;    
    QString configFname;
    QStringList images;
    QList<int> regAttrIdx;

    runthread *recogDoner;
    int rowCount, perRowCount;
    QString startTime, endTime;
public:
    RunManager(const QString dealDir,
               const QString saveDir,
               const QList<int> &attrIdxs,
               const QList<int> &funcIdxs,
               bool saveFlag, const QString saveFname);

signals:

public slots:
    void updateTableWidget(int res);
    void overMessage();
    void stopRun();
    void saveRes();

private:
    QLabel *UIcreateLabel(const QString &text);
    QPushButton *UIcreateButton(const QString &text, const char *member);
    void UIcreate();

};

#endif // RUNMANAGER_H
