#include "myimglabel.h"

#include "GLOBALCONFIG.h"
#include "GLOBALDEFINE.h"

#include <QDomDocument>
#include <QMouseEvent>
#include <QDebug>

GLOBALTESTPOSE GLOBALTESTPOSE::represant;
QStringList GLOBALTESTPOSE::testname;
QList<int> GLOBALTESTPOSE::preIdxs;
bool GLOBALTESTPOSE::_init = init();

#define sys_testPoseFile "./configs/0testDataForm.config"

bool GLOBALTESTPOSE::init() {
    QFile file(sys_testPoseFile);
    if(!file.open(QFile::ReadOnly)) {
        exit(10);
    }
    while(!file.atEnd()) {
        QString line = file.readLine(255).simplified();
        QStringList tmpStr = line.split(":");

        testname.push_back(tmpStr[0]);
        preIdxs.push_back(tmpStr[1].toInt());
    }
    file.close();
    return true;
}
int GLOBALTESTPOSE::getTestPoseNum() const {
    return testname.size();
}
QString GLOBALTESTPOSE::getTestname(int idx) const {
    return testname[idx];
}
int GLOBALTESTPOSE::getTestpreIdx(int idx) const {
    if(idx >= 0 && idx <= testname.size()) {
        return preIdxs[idx];
    }
    return -1;
}

void MyImgLabel::drawingSwitch(bool status)
{
    drawAllow = status;
    if(status) setCursor(Qt::CrossCursor);
    else setCursor(Qt::ForbiddenCursor);
}

void MyImgLabel::skipCurrentData()
{
    labelData.clear();
    int dataTypeIdx = GLOBALCONFIG::inst()->getPTypeIdxByIdx(curLabelIdx);
    int dataNum = GLOBALDEFINE::BASEDATA_DATANUMS[dataTypeIdx]/2;
    QPoint tmp(-1, -1);
    for(int i = 0; i < dataNum; i++) {
        labelData.push_back(tmp);
    }
    emit dataGetOk(labelData);    
}

void MyImgLabel::updateLabelIdx(int idx)
{
    curLabelIdx = idx;
}

void MyImgLabel::setMyPen(int width, QColor color)
{
    pen.setWidth(width);
    pen.setColor(color);
    painter.setPen(pen);
}

void MyImgLabel::initialDrawingStatus()
{
    drawingSwitch(false);
    curLabelIdx = 0;
}

MyImgLabel::MyImgLabel()
{    
}

bool MyImgLabel::loadImgData(const QString &img, const QString &xml)
{
    //img data:
    MYIMG = img;
    MYXML = xml;

    image.load(img);
    showImage = image.copy();
    showImageCopy = image.copy();
    update();
    this->resize(image.width(), image.height());
    initialDrawingStatus();

    //xml data:
    QFile file(xml);
    QDomDocument doc;
    if(doc.setContent(&file, false) == false) return false;
    QDomElement rootNode = doc.documentElement();

    QDomNode poseDataNode = rootNode.firstChild();
    int size = GLOBALCONFIG::inst()->getPoseCounter();
    poseDatas.clear();
    for(int i = 0; i < size; i++) {
        QString poseName = GLOBALCONFIG::inst()->getPoseNameByIndex(i);
        QString poseNameValue = poseDataNode.firstChildElement(poseName).text();
        if(poseNameValue.isEmpty()) poseDatas.push_back("null");
        else poseDatas.push_back(poseNameValue);
    }

    QDomNode attrDataNode = poseDataNode.nextSibling();
    int size1 = GLOBALCONFIG::inst()->getAllKind();
    attrDatas.clear();
    for(int i = 0; i < size1; i++) {
        QString attrName = GLOBALCONFIG::inst()->getAttrNameByIndex(i);
        QString attrNameValue = attrDataNode.firstChildElement(attrName).text();
        if(attrNameValue.isEmpty()) attrDatas.push_back("null");
        else attrDatas.push_back(attrNameValue);
    }
    file.close();

    refreshLabelRes();
    return true;
}

void MyImgLabel::refreshLabelRes(/*const QStringList &data*/)
{
    painter.begin(&showImage);
    setMyPen();

    //get data:
    QList<QPoint> dots;
    int size = GLOBALTESTPOSE::instance()->getTestPoseNum();
    for(int i = 0; i < size; i++) {
        QPoint tmpDot(-1, -1);
        QString poseDotName = GLOBALTESTPOSE::instance()->getTestname(i);
        int poseDotIdx = GLOBALCONFIG::inst()->getPoseIndexByName(poseDotName);

        QString dataStr = poseDatas[poseDotIdx];
        if(dataStr.compare("null") != 0) {
            QStringList dataValues = dataStr.split(",");
            tmpDot.setX(dataValues[0].toInt());
            tmpDot.setY(dataValues[1].toInt());
        }
        dots.push_back(tmpDot);
    }

    //draw data:
    for(int i = 0; i < size; i++) {
        if(dots[i].x() == -1) continue;
        int dotPreIdx = GLOBALTESTPOSE::instance()->getTestpreIdx(i);
        if(dotPreIdx == -1) painter.drawPoint(dots[i].x(), dots[i].y());
        else painter.drawLine(dots[i], dots[dotPreIdx]);
    }
    showImageCopy = showImage;
    painter.end();
    update();
}

void MyImgLabel::resetData()
{
    showImage = image.copy();
    showImageCopy = image;
    update();
}

bool MyImgLabel::saveToFile()
{
    QString cmd;
    if(GLOBALFUNC::inst()->confirmFileExist(MYXML)) {
        cmd = QString("mv %1 %2.bak").arg(MYXML).arg(MYXML);
        system(cmd.toStdString().c_str());
    }
    QFile file(MYXML);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction(
                "xml", "Version=\"1.0\", encoding\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("file");
    root.setAttribute("name", MYIMG);
    doc.appendChild(root);
    QDomElement node1 = doc.createElement("pose");
    root.appendChild(node1);
    QDomElement node2 = doc.createElement("attr");
    root.appendChild(node2);

    //pose data:
    int size = poseDatas.size();
    for(int i = 0; i < size; i++) {
        QString tmpName = GLOBALCONFIG::inst()->getPoseNameByIndex(i);
        QDomElement tmpNode1 = doc.createElement(tmpName);
        int tmpType = GLOBALCONFIG::inst()->getPTypeIdxByIdx(i);
        tmpNode1.setAttribute("type", GLOBALDEFINE::BASEDATA_NAMES[tmpType]);
        QDomText nodeText = doc.createTextNode(poseDatas[i]);
        tmpNode1.appendChild(nodeText);
        node1.appendChild(tmpNode1);
    }

    //attr data:
    int size1 = attrDatas.size();
    for(int i = 0; i < size1; i++) {
        QString tmpName = GLOBALCONFIG::inst()->getAttrNameByIndex(i);
        QDomElement tmpNode2 = doc.createElement(tmpName);
        QDomText nodeText = doc.createTextNode(attrDatas[i]);
        tmpNode2.appendChild(nodeText);
        node2.appendChild(tmpNode2);
    }
    doc.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();
    return true;
}

void MyImgLabel::paintEvent(QPaintEvent * /*event*/)
{
    if(showImage.isNull()) return;
    QPainter painter(this);
    painter.drawImage(0, 0, showImage);
    painter.end();
}

void MyImgLabel::mousePressEvent(QMouseEvent *event)
{
    if(drawAllow) {
        curSPoint = event->pos();
        switch(GLOBALCONFIG::inst()->getPTypeIdxByIdx(curLabelIdx)) {
        case POINT: drawShape = POINT; break;
        case LINE: drawShape = LINE;break;
        case RECT: drawShape = RECT; break;
        }
        QColor tmp = image.pixel(curSPoint);
        int r = (tmp.hue()%255)+1;
        drawColor = QColor(r, tmp.saturation(), tmp.value());
    }
}

void MyImgLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(drawAllow) {
        if(drawShape == FLAG_SHAPE::LINE || drawShape == FLAG_SHAPE::RECT) {
            showImage = showImageCopy;

            painter.begin(&showImage);
            setMyPen(3, drawColor);
            if(drawShape == LINE) painter.drawLine(curSPoint, event->pos());
            else  painter.drawRect(QRect(curSPoint, event->pos()));
            painter.end();
            update();
        }
    }
}

void MyImgLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(drawAllow) {
        painter.begin(&showImageCopy);
        setMyPen(3, drawColor);

        labelData.clear();
        if(drawShape == POINT) {
            painter.drawEllipse(curSPoint, 3, 3);
            labelData << curSPoint;
        } else if(drawShape == LINE){
            painter.drawLine(curSPoint, event->pos());
            labelData << curSPoint << event->pos();
        } else if(drawShape == RECT) {
            painter.drawRect(QRect(curSPoint, event->pos()));
            labelData << curSPoint << event->pos();
        }
        showImage = showImageCopy;
        painter.end();
        update();

        emit dataGetOk(labelData);        
    }
}
