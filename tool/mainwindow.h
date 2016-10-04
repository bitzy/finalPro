#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QGroupBox;
class QTableWidget;
class QStandardItemModel;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void initial_system_dataLoad();
    void loadAttrTableData(const QString& fpath);
    QLabel *UIcreateLabel(const QString &text);
    QPushButton *UIcreateButton(const QString &text, const char *member);
    QGroupBox *UIcreatePoseGroup();
    QGroupBox *UIcreateAttrGroup();
    QGroupBox *UIcreateConfigGroup();
    QList<QPoint> getFuncNeedtoRun();

    QLabel *LabelChoosedPath;
    QLabel *BulkChoosedPath;
    QLabel *analyzeChoosedFile;
    QPushButton *labelPathBtn;
    QPushButton *bulkPathBtn;
    QPushButton *analyzePathBtn;
    QPushButton *bulkBeginBtn;
    QPushButton *labelBeginBtn;
    QPushButton *analyzeBeginBtn;
    QGroupBox *poseGroup;
    QGroupBox *attrGroup;
    QGroupBox *configGroup;
    QTableWidget *table2;
    QStandardItemModel *model;

    int _windowH;
    int _windowW;
private slots:
    void browseLabelPath();
    void browseBulkPath();
    void browseAnalyzeFile();

    void exportBaseData();
    void labelBegin();
    void bulkBegin();
    void analyzeBegin();
    void sleeveBulkBegin();
};

#endif // MAINWINDOW_H
