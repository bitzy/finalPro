#ifndef FLAGFORM_H
#define FLAGFORM_H

#include <QDialog>
#include <QFileInfoList>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QGroupBox;
class QTableWidget;
class QTextEdit;
class QLineEdit;
class MyImgLabel;
QT_END_NAMESPACE

class FlagForm : public QDialog
{
    Q_OBJECT
public:
    explicit FlagForm(QString dir, QString storeDir);

private:
    void UIcreateAll();
    QPushButton* UIcreateBtn(const QString&, const char*);    
    QTableWidget* UIcreatePoseTable();
    QTableWidget* UIcreateAttrTable();
    void UIsetColor(int id, const QString& color);

    /**
     * @brief picBoard
     * is a contain imgdata's label.
     */
    MyImgLabel* picBoard;

    QPushButton* prevBtn;
    QPushButton* nextBtn;
    QPushButton* deleBtn;
    QPushButton* submitBtn;
    QPushButton* resetBtn;
    QPushButton* reviseBtn;
    QPushButton* skipAttrBtn;
    QTableWidget* poseTable;
    QTableWidget* attrTable;
    QTextEdit* showResult;
    QLineEdit* imageSkipEdit;
    QPushButton* skipToBtn;
    QPushButton* extractBtn;
    QLineEdit* extractEdit;
    QPushButton* findNameBtn;
    QLineEdit* findNameEdit;
    QLabel* curCountLabel;
    QLabel* totalCountLabel;
    QLabel* curNameShowLabel;
    QLabel* leftSleeveValue;
    QLabel* rightSleeveValue;
    QLabel* color1Label;
    QLabel* color2Label;
    QLabel* color3Label;

    QFileInfoList  dealImages;
    QString storePath;

    int curImgIndex;
    QString curImgBaseName;
    void updateCurImgIdx(int idx);

    int poseLabelIdx;
    bool poseLabelFull;
    bool isPoseLabel;
    void updatePosLabelIdx(int idx);
    void initialToolStatus(bool status);
    void updateToolBtn();

    void updateAttrTable(const QList<QPoint> &data);
    double calSleeveLenth(const QList<QPoint>& data, bool left);
    int sleeveDouble2Int(double percent);
    double calClothLenth(const QList<QPoint>& data);
    int clothDouble2Int(double percent);    

    QPoint _getDataFromBoard(int row);

private slots:
    void prevImg();
    void nextImg();    
    void deleImg();
    void resetImg();
    void reviseImg();
    void skipAttrImg();
    void submitImg();
    void dataGetFromPicBoard(QList<QPoint>& data);

    void skipToImgIdx();
    void estractToPath();
    void findImageName();

    void testAttr();

protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);

public:
    void showImgByIdx(int index);
    void imgLabelDataShow();
    void DoShowImgNULL();
};

#endif // FLAGFORM_H
