#ifndef MYIMGLABEL_H
#define MYIMGLABEL_H

#include <QLabel>
#include <QPainter>
#include <string>
#include <vector>
using namespace std;
class ImgData;

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

    ImgData* imgData;
    //bool _loadxmlData(ImgData& data);
    QString _int2color(int v) const;
    vector<string> QString2StdVec(const QStringList&);
    void labelRefreshPoseData(); //show pose data on label;

public:
    MyImgLabel();
    //load xml data for this label
    bool labelLoadDataByXML(const QString& img, const QString& xml);
    void labelLoadDataByData(const QStringList& pose,
                        const QStringList& attr);
    void labelReset();//just reset the image content; not label index;    
    bool labelSave(); //save imgdata to disk;
    const QString labelTest(int attri, int wayj);

    //get data from label;
    const QStringList labelDataGetPoseDatas() const;
    const QStringList labelDataGetAttrDatas() const;

    void drawingSwitch(bool);
    void updateLabelIdx(int idx);//outer control label index;
    void skipCurrentData();   //skip current label data;
    QString getColor(const QPoint& pos) const; //get color from label point

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void dataGetOk(QList<QPoint>&);
};

#endif // MYIMGLABEL_H
