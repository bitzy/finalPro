#ifndef MYIMGLABEL_H
#define MYIMGLABEL_H

#include <QLabel>
#include <QPainter>

#include "GLOBALFUNC.h"
#include "flagform.h"
#include "attrRecognize/imgdata.h"

/**
 * @brief The GLOBALTESTPOSE class
 * build relationship with "0pose.config" file.
 * used to draw people pose data;
 */
class GLOBALTESTPOSE {
    static GLOBALTESTPOSE represant;
    static QStringList testname;
    static QList<int> preIdxs;

    static bool _init;
    static bool init();
    GLOBALTESTPOSE(){}
    GLOBALTESTPOSE(const GLOBALTESTPOSE&);
public:
    static GLOBALTESTPOSE* inst() {
        return &represant;
    }
    int getTestPoseNum() const;
    QString getTestname(int idx) const;
    int getTestpreIdx(int idx) const;
};

/**
 * @brief The MyImgLabel class
 * This class used for update flagform data.
 */
class MyImgLabel : public QLabel
{    
    Q_OBJECT
private:
    enum FLAG_SHAPE {POINT=0, LINE, RECT};
    QPainter painter;
    QPen pen;
    void setMyPen(int width = 3, QColor color = Qt::red);

    QImage image;
    QImage showImage;
    QImage showImageCopy;    

    bool drawAllow;
    void initialDrawingStatus();
    FLAG_SHAPE drawShape;
    QColor drawColor;
    int curLabelIdx; //attributes index;    

    QPoint curSPoint;
    QList<QPoint> labelData;

    friend void FlagForm::DoShowImg(const MyImgLabel*);
    friend void FlagForm::reLoadImg(MyImgLabel*);

    bool labelDataOKFlag;
    ImgData imgData;
    bool _loadxmlData(ImgData& data);
    bool _savexmlData(const ImgData& data);

public:
    MyImgLabel();
    bool labelLoad(const QString& img, const QString& xml);
    void labelReset();
    bool labelSave();
    void labelRefreshPoseData();
    const QString labelTest(int attri, int wayj);

    void drawingSwitch(bool);
    void skipCurrentData();
    void updateLabelIdx(int idx);    

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void dataGetOk(QList<QPoint>&);
};

#endif // MYIMGLABEL_H
