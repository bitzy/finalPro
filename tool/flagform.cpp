#include "flagform.h"
#include <QtWidgets>
#include <QMessageBox>

#include "GLOBALDEFINE.h"
#include "GLOBALCONFIG.h"
#include "GLOBALFUNC.h"
#include "attrRecognize/waysInterface.h"

#include "myimglabel.h"

FlagForm::FlagForm(QString dir, QString storeDir)
{
    QDir tmpDir(dir);
    dealImages = tmpDir.entryInfoList(GLOBALDEFINE::imageTypefilter, QDir::AllEntries, QDir::DirsFirst);
    if(dealImages.size() == 0) {
        QMessageBox::information(this, "worning", "no image in this directory!", QMessageBox::Ok);
        return ;
    } else {
        storePath = GLOBALFUNC::pathSlashAdd(storeDir);

        UIcreateAll();  //create all window;
        showImgByIdx(0); //show content;
    }
}

//=======================ui=========================================
void FlagForm::UIcreateAll()
{
    prevBtn = UIcreateBtn(tr("&Previous"), SLOT(prevImg()));
    prevBtn->setShortcut(Qt::Key_P);
    prevBtn->setDisabled(true);

    nextBtn = UIcreateBtn(tr("&Next"), SLOT(nextImg()));
    nextBtn->setShortcut(Qt::Key_N);
    nextBtn->setDisabled(true);

    deleBtn = UIcreateBtn(tr("&Delete"), SLOT(deleImg()));
    deleBtn->setShortcut(Qt::Key_D);
    deleBtn->setDisabled(false);

    submitBtn = UIcreateBtn(tr("&Submit to DB"), SLOT(submitImg()));
    submitBtn->setShortcut(Qt::Key_S);
    submitBtn->setDisabled(true);

    resetBtn = UIcreateBtn(tr("&Reset"), SLOT(resetImg()));
    resetBtn->setShortcut(Qt::Key_R);
    //resetBtn->setDisabled(true);

    reviseBtn = UIcreateBtn(tr("R&evise"), SLOT(reviseImg()));
    reviseBtn->setShortcut(Qt::Key_E);
    //reviseBtn->setDisabled(true);

    skipAttrBtn = UIcreateBtn(tr("Skip&X"), SLOT(skipAttrImg()));
    skipAttrBtn->setShortcut(Qt::Key_X);
    skipAttrBtn->setDisabled(true);

    showResult = new QTextEdit("AttrInfo");
    showResult->setReadOnly(true);

    imageSkipEdit = new QLineEdit;
    QIntValidator* tmpV = new QIntValidator;
    tmpV->setRange(1, dealImages.size());
    imageSkipEdit->setValidator(tmpV);

    skipToBtn = UIcreateBtn(tr("Skip To"), SLOT(skipToImgIdx()));
    skipToBtn->setDisabled(false);

    extractEdit = new QLineEdit;
    extractEdit->setValidator(tmpV);

    extractBtn = UIcreateBtn(tr("Extract current img[and .xml]"),
                             SLOT(estractToPath()));
    extractBtn->setDisabled(false);

    findNameBtn = UIcreateBtn(tr("find"), SLOT(findImageName()));
    findNameBtn->setDisabled(false);

    findNameEdit = new QLineEdit;

    curCountLabel = new QLabel(tr("0"));
    totalCountLabel = new QLabel(tr("0"));
    curNameShowLabel = new QLabel(tr("null"));

    QHBoxLayout *scrollLayout = new QHBoxLayout;
    QScrollArea *scroll = new QScrollArea;
    scroll->setLayout(scrollLayout);
    picBoard = new MyImgLabel;
    connect(picBoard, SIGNAL(dataGetOk(QList<QPoint>&)),
            this, SLOT(dataGetFromPicBoard(QList<QPoint>&)));
    scroll->setWidget(picBoard);

    QHBoxLayout *hbox3 = new QHBoxLayout;
    hbox3->addStretch(1);
    hbox3->addWidget(prevBtn);
    hbox3->addWidget(nextBtn);
    hbox3->addWidget(deleBtn);
    QHBoxLayout *hbox33 = new QHBoxLayout;
    hbox33->addStretch(1);
    hbox33->addWidget(resetBtn);
    hbox33->addWidget(reviseBtn);
    hbox33->addWidget(skipAttrBtn);
    hbox33->addWidget(submitBtn);

    QHBoxLayout *hbox4 = new QHBoxLayout;
    QLabel* tmp0 = new QLabel(tr("Extract FILE:"));
    hbox4->addStretch(1);
    hbox4->addWidget(tmp0);
    hbox4->addWidget(extractEdit);
    hbox4->addWidget(extractBtn);

    QHBoxLayout *hbox2 = new QHBoxLayout;
    hbox2->addWidget(curCountLabel);
    QLabel *tmp = new QLabel(tr("/"));
    hbox2->addWidget(tmp);
    hbox2->addWidget(totalCountLabel);
    hbox2->addStretch(1);
    hbox2->addWidget(curNameShowLabel);

    //! [1]
    QVBoxLayout *leftBox = new QVBoxLayout;
    leftBox->addLayout(hbox2);
    leftBox->addWidget(scroll);
    leftBox->addLayout(hbox3);
    leftBox->addLayout(hbox33);
    leftBox->addLayout(hbox4);
    //! [1]

    leftSleeveValue = new QLabel("-1");
    rightSleeveValue = new QLabel("-1");

    color1Label = new QLabel("null");
    color2Label = new QLabel("null");
    color3Label = new QLabel("null");
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(UIcreateAttrTable(), 1);
    vbox->addWidget(leftSleeveValue);
    vbox->addWidget(rightSleeveValue);
    vbox->addWidget(color1Label);
    vbox->addWidget(color2Label);
    vbox->addWidget(color3Label);
    vbox->addWidget(showResult, 1);

    //! [21]
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(UIcreatePoseTable());
    hbox->addLayout(vbox);
    //! [21]

    QHBoxLayout *bottomLine = new QHBoxLayout;
    bottomLine->addStretch(1);
    bottomLine->addWidget(findNameEdit);
    bottomLine->addWidget(findNameBtn);
    bottomLine->addWidget(imageSkipEdit);
    bottomLine->addWidget(skipToBtn);

    //! [2]
    QVBoxLayout *rightBox = new QVBoxLayout;
    rightBox->addLayout(hbox);
    rightBox->addLayout(bottomLine);
    //! [2]

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftBox);
    layout->addLayout(rightBox);
    setLayout(layout);

    //UI data initial:
    totalCountLabel->setText(QString::number(dealImages.size()));
}

QPushButton *FlagForm::UIcreateBtn(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

QTableWidget *FlagForm::UIcreatePoseTable()
{
    poseTable = new QTableWidget;
    poseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    poseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    poseTable->setSelectionMode(QAbstractItemView::SingleSelection);
    poseTable->setColumnCount(1);
    poseTable->setRowCount(GLOBALCONFIG::inst()->getPoseCounter());
    poseTable->setVerticalHeaderLabels(GLOBALCONFIG::inst()->getPoseName());
    QStringList header;
    header << "value";
    poseTable->setHorizontalHeaderLabels(header);
    poseTable->setColumnWidth(0, 180);
    poseTable->setMinimumWidth(250);
    poseTable->setMaximumWidth(250);

    return poseTable;
}

QTableWidget *FlagForm::UIcreateAttrTable()
{
    attrTable = new QTableWidget;
    attrTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int size = GLOBALCONFIG::inst()->getAllKind();
    attrTable->setColumnCount(3);
    attrTable->setRowCount(size);
    attrTable->setVerticalHeaderLabels(GLOBALCONFIG::inst()->getAllAttrs());

    QStringList header;
    header << "value" << "way" << "test";
    attrTable->setHorizontalHeaderLabels(header);
    for(int i = 0; i < size; i++) {
        QString attrName = GLOBALCONFIG::inst()->getAttrNameByIndex(i);

        //value:
        QComboBox *combo = new QComboBox;
        QStringList valueList = GLOBALCONFIG::inst()->getAttrValuesByattrName(attrName);
        combo->addItems(valueList);
        combo->setEnabled(false);
        attrTable->setCellWidget(i, 0, combo);

        //way & test:
        QComboBox *wayComboBox = new QComboBox;
        QStringList tmplist = ATTRWAYS::instance()->getWays(attrName);
        if(!tmplist.isEmpty()) {
            wayComboBox->addItems(tmplist);
            attrTable->setCellWidget(i, 1, wayComboBox);
            attrTable->setCellWidget(i, 2, UIcreateBtn(tr("test"), SLOT(testAttr())));
        }
    }

    return attrTable;
}

//=======================show image=========================================
void FlagForm::showImgByIdx(int index)
{
    updateCurImgIdx(index);
    QString imgPath = dealImages[index].absoluteFilePath();
    QString xmlPath = QString("%1%2.xml").arg(storePath).arg(curImgBaseName);
    bool ret = picBoard->labelLoad(imgPath, xmlPath);

    if(ret == true) DoShowImg(picBoard);
    else DoShowImgNULL();

    initialToolStatus(ret);
}

void FlagForm::DoShowImg(const MyImgLabel * imgLabel)
{
    //posedata:
    int size = (int)imgLabel->imgData.poseDatas.size();
    for(int i = 0; i < size; i++) {
        QString tmpValue = QString::fromStdString(imgLabel->imgData.poseDatas[i]);
        QTableWidgetItem* tmp = new QTableWidgetItem(tmpValue);
        poseTable->setItem(i, 0, tmp);
    }
    //attrdata:
    int size1 = imgLabel->imgData.attrDatas.size();
    for(int i = 0; i < size1; i++) {
        QComboBox *tmp = (QComboBox*)attrTable->cellWidget(i, 0);
        QString tmpValue = QString::fromStdString(imgLabel->imgData.attrDatas[i]);
        int tmpIdx = tmp->findText(tmpValue);
        if(tmpIdx == -1) {
            tmp->clear();
            tmp->addItem(tmpValue);
        } else {
            tmp->setCurrentText(tmpValue);
        }
        tmp->setDisabled(true);
    }
}

void FlagForm::reLoadImg(MyImgLabel * imgLabel)
{
    //posedata:
    imgLabel->imgData.poseDatas.clear();
    int size = poseTable->rowCount();
    for(int i = 0; i < size; i++) {
        QTableWidgetItem *tmp = poseTable->item(i, 0);
        QString tmpStr = tmp->text();

        int tmpType = GLOBALCONFIG::inst()->getPTypeIdxByIdx(i);
        int tmpSize = GLOBALDEFINE::BASEDATA_DATANUMS[tmpType];
        if(tmpStr.compare("null") == 0) {
            QString tmpValue("-1");
            for(int j = 1; j < tmpSize; j++) tmpValue.append(",-1");
            imgLabel->imgData.poseDatas.push_back(tmpValue.toStdString());
        } else imgLabel->imgData.poseDatas.push_back(tmpStr.toStdString());
    }
    //attrdata:
    imgLabel->imgData.attrDatas.clear();
    int size1 = attrTable->rowCount();
    for(int i = 0; i < size1; i++) {
        QComboBox* tmp = (QComboBox*)attrTable->cellWidget(i, 0);
        QString tmpStr = tmp->currentText();

        imgLabel->imgData.attrDatas.push_back(tmpStr.toStdString());
    }
    imgLabel->labelDataOKFlag = true;
}

void FlagForm::DoShowImgNULL()
{
    poseTable->clearContents();
    int size = GLOBALCONFIG::inst()->getAllKind();
    for(int i = 0; i < size; i++) {
        QComboBox* tmp = (QComboBox*)attrTable->cellWidget(i, 0);
        if(tmp->count() == 1) {
            tmp->clear();
            tmp->addItem("");
        }
        tmp->setEnabled(true);
    }
}

void FlagForm::updateCurImgIdx(int idx)
{
    curImgIndex = idx;
    curImgBaseName = dealImages[idx].baseName();

    //btn control:
    if(idx == 0) prevBtn->setEnabled(false);
    else prevBtn->setEnabled(true);
    if(idx == dealImages.size()-1) nextBtn->setEnabled(false);
    else nextBtn->setEnabled(true);

    //update panel:
    curCountLabel->setText(QString::number(idx+1));
    curNameShowLabel->setText(curImgBaseName);
    leftSleeveValue->setText(tr("-1"));
    rightSleeveValue->setText(tr("-1"));
    color1Label->setText(tr(""));
    color2Label->setText(tr(""));
    color3Label->setText(tr(""));
}

void FlagForm::updatePosLabelIdx(int idx)
{
    poseLabelIdx = idx;
    poseTable->selectRow(idx);
    picBoard->updateLabelIdx(idx);
    if(poseLabelIdx == GLOBALCONFIG::inst()->getPoseCounter()) {
        poseLabelFull = true;
        updateToolBtn();
    }
}

void FlagForm::initialToolStatus(bool status)
{
    poseLabelFull = status;
    updatePosLabelIdx(0);
    if(status) {//has data;
        isPoseLabel = false;

        resetBtn->setDisabled(false);//can reset
        reviseBtn->setDisabled(false);//can revise
        skipAttrBtn->setDisabled(true);//no skip attr
    } else {//no data;
        isPoseLabel = true;
        resetBtn->setDisabled(true);//no reset
        reviseBtn->setDisabled(true);//no revise
        skipAttrBtn->setDisabled(false);//can skip attr
    }
    submitBtn->setDisabled(true);// no submit
}

void FlagForm::updateToolBtn()
{
    if(isPoseLabel) {
        resetBtn->setDisabled(false);
        if(poseLabelIdx == GLOBALCONFIG::inst()->getPoseCounter())
            skipAttrBtn->setDisabled(true);//no skip attr
        else skipAttrBtn->setDisabled(false);

        //submit
        if(poseLabelFull) {
            submitBtn->setDisabled(false);
        } else {
            reviseBtn->setDisabled(true);//no edit
            submitBtn->setDisabled(true); //no submit
        }
    }
}

void FlagForm::updateAttrTable(const QList<QPoint> &data)
{
    if(data[0].x() == -1) return ;
    QString poseName = GLOBALCONFIG::inst()->getPoseNameByIndex(poseLabelIdx);

    QComboBox *tmpItem;
    QString resValue;
    if(!poseName.compare("L_Sleeve") || !poseName.compare("R_Sleeve")) {
        bool leftFlag = !poseName.compare("L_Sleeve");
        double percent = calSleeveLenth(data, leftFlag);
        resValue = QString::number(percent, 'f', 1);

        //choose idx:
        int attrRow = GLOBALCONFIG::inst()->getIndexByName("sleeve");
        tmpItem = (QComboBox*)attrTable->cellWidget(attrRow, 0);
        tmpItem->setCurrentIndex(sleeveDouble2Int(percent));

        //set value:
        attrRow = GLOBALCONFIG::inst()->getIndexByName("sleeveValue");
        tmpItem = (QComboBox*)attrTable->cellWidget(attrRow, 0);
        if(leftFlag) leftSleeveValue->setText(resValue);
        else rightSleeveValue->setText(resValue);
    } else if(!poseName.compare("Lenth")) {
        double percent = calClothLenth(data);
        resValue = QString::number(percent, 'f', 1);

        //choose idx:
        int attrRow = GLOBALCONFIG::inst()->getIndexByName("up_length");
        tmpItem = (QComboBox*)attrTable->cellWidget(attrRow, 0);
        tmpItem->setCurrentIndex(clothDouble2Int(percent));

        //set value:
        attrRow = GLOBALCONFIG::inst()->getIndexByName("lenthValue");
        tmpItem = (QComboBox*)attrTable->cellWidget(attrRow, 0);
    } else if(!poseName.compare("HSIColor1") || !poseName.compare("HSIColor2") ||
              !poseName.compare("HSIColor3")) {
        int BGR = calBGRColor(data);
        resValue = QString::number(BGR);

        //set value:
        QString attrName = poseName.mid(3);
        int attrRow = GLOBALCONFIG::inst()->getIndexByName(attrName);
        tmpItem = (QComboBox*)attrTable->cellWidget(attrRow, 0);
    } else return;

    tmpItem->clear();
    tmpItem->addItem(resValue);
}

double FlagForm::calSleeveLenth(const QList<QPoint> &data, bool left)
{
    double res;
    QPoint P1 = data[0], P2 = data[1];
    QPoint A, B, C;
    if(left == true) {
        A = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("left_shoulder"));
        B = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("left_elbow"));
        C = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("left_hand"));
    } else {
        A = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("right_shoulder"));
        B = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("right_elbow"));
        C = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("right_hand"));
    }

    QPoint ABcrossP;
    bool flagNoCross = false;
    if(A.x() == B.x()) {
        if(P1.x() != P2.x()) {
            double k2 = (P1.y()-P2.y())*1.0/(P1.x() - P2.x());
            ABcrossP.setX(A.x());
            int y = k2*(A.x()-P1.x())+P1.y();
            ABcrossP.setY(y);
        } else flagNoCross = true;
    } else {
        double k1 = (A.y()-B.y())*1.0/(A.x()-B.x());
        if(P1.x() == P2.x()) {
            ABcrossP.setX(P1.x());
            int y = k1*(P1.x()-A.x())+A.y();
            ABcrossP.setY(y);
        } else {
            double k2 = (P1.y()-P2.y())*1.0/(P1.x() - P2.x());
            if(k1 != k2) {
                double x = (k1*B.x()-B.y()-k2*P1.x()+P1.y())/(k1-k2);
                ABcrossP.setX(int(x));
                int y = k1*(x-B.x())+B.y();
                ABcrossP.setY(y);
            } else flagNoCross = true;
        }
    }
    if(!flagNoCross) {
        if(ABcrossP.x() >= std::min(A.x(), B.x())-5 &&
                ABcrossP.x() <= std::max(A.x(), B.x())+5 &&
                ABcrossP.y() >= std::min(A.y(), B.y())-5 &&
                ABcrossP.y() <= std::max(A.y(), B.y())+5 &&
                ABcrossP.x() >= std::min(P1.x(), P2.x())-5 &&
                ABcrossP.x() <= std::max(P1.x(), P2.x())+5 &&
                ABcrossP.y() >= std::min(P1.y(), P2.y())-5 &&
                ABcrossP.y() <= std::max(P1.y(), P2.y())+5) {
            //calculate percent:
            res = GLOBALFUNC::inst()->getEuclideanDist(A, ABcrossP)
                    /GLOBALFUNC::inst()->getEuclideanDist(A, B)*50;
            return res;
        }
    }

    //bottom arm:
    QPoint BCcrossP;
    if(B.x() == C.x()) {
        if(P1.x() != P2.x()) {
            double k2 = (P1.y()-P2.y())*1.0/(P1.x() - P2.x());
            BCcrossP.setX(B.x());
            int y = k2*(B.x()-P1.x())+P1.y();
            BCcrossP.setY(y);
        } else exit(1);
    } else {
        double k3 = (B.y()-C.y())*1.0/(B.x()-C.x());
        if(P1.x() == P2.x()) {
            BCcrossP.setX(P1.x());
            int y = k3*(P1.x()-B.x())+B.y();
            BCcrossP.setY(y);
        } else {
            double k2 = (P1.y()-P2.y())*1.0/(P1.x() - P2.x());
            //assert(k3 != k2);
            double xx = (k3*B.x()-B.y()-k2*P1.x()+P1.y())/(k3-k2);
            BCcrossP.setX(int(xx));
            int yy = k3*(xx-B.x())+B.y();
            BCcrossP.setY(yy);
        }
    }

    if(BCcrossP.x() >= std::min(B.x(), C.x())-5 &&
            BCcrossP.x() <= std::max(B.x(), C.x())+5 &&
            BCcrossP.y() >= std::min(B.y(), C.y())-5 &&
            BCcrossP.y() <= std::max(B.y(), C.y())+5 &&
            BCcrossP.x() >= std::min(P1.x(), P2.x())-5 &&
            BCcrossP.x() <= std::max(P1.x(), P2.x())+5 &&
            BCcrossP.y() >= std::min(P1.y(), P2.y())-5 &&
            BCcrossP.y() <= std::max(P1.y(), P2.y())+5) {
        res = GLOBALFUNC::inst()->getEuclideanDist(BCcrossP, B)
                /GLOBALFUNC::inst()->getEuclideanDist(B, C)*50 + 50;
        return res;
    }

    //cross not suitable or paralyze:
    if(GLOBALFUNC::inst()->getEuclideanDist(A, P1)
            < GLOBALFUNC::inst()->getEuclideanDist(C, P1)) {
        return 0;
    } else return 100;
}

int FlagForm::sleeveDouble2Int(double percent)
{
    int perInt = (int)percent;
    if(perInt < 5) return 0;
    else if(perInt < 40) return 1; //short
    else if(perInt < 60) return 2; //mid-long
    else return 3;
}

double FlagForm::calClothLenth(const QList<QPoint> &data)
{
    double res;
    int len = (data[0].y() + data[1].y())/2;

    QPoint neck = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("neck"));
    QPoint hip1 = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("left_hip"));
    QPoint hip2 = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("right_hip"));
    QPoint hip((hip1.x()+hip2.x())>>1, (hip1.y()+hip2.y())>>1);

    QPoint knee1 = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("left_knee"));
    QPoint knee2 = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("right_knee"));
    if(knee1.x() == -1 || knee2.x() == -1) {
        res = (len - neck.y())*40 / GLOBALFUNC::inst()->getEuclideanDist(neck, hip);
        return res;
    }

    QPoint knee((knee1.x()+knee2.x())>>1, (knee1.y()+knee2.y())>>1);
    QPoint ankle1 = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("left_ankle"));
    QPoint ankle2 = _getDataFromBoard(GLOBALCONFIG::inst()->getPoseIndexByName("right_ankle"));
    if(ankle1.x() == -1 || ankle2.x() == -1) {
        res = (len - neck.y())*70 / GLOBALFUNC::inst()->getEuclideanDist(neck, knee);
        return res;
    }

    QPoint ankle((ankle1.x()+ankle2.x())>>1, (ankle1.y()+ankle2.y())>>1);
    res = (len - neck.y())*100/ GLOBALFUNC::inst()->getEuclideanDist(neck, ankle);
    if(res > 100) res = 100;
    return res;
}

int FlagForm::clothDouble2Int(double percent)
{
    int perInt = (int)percent;
    if(perInt < 30) return 0;
    else if(perInt < 55) return 1;
    else if(perInt < 75) return 2;
    else return 3;
}

int FlagForm::calBGRColor(const QList<QPoint> &data)
{
    return 0;
}

QPoint FlagForm::_getDataFromBoard(int row)
{
    QString str = poseTable->item(row, 0)->text();
    if(str.compare("null") == 0) return QPoint(-1, -1);
    QStringList dataStr = str.simplified().split(",");
    QPoint res(dataStr[0].toInt(), dataStr[1].toInt());
    return res;
}


//======================= up/down btn =========================================
void FlagForm::prevImg()
{
    if(poseLabelIdx != 0 && isPoseLabel == true) {
        int ret = QMessageBox::information(
                    this, "Confirm",
                    "Change not saved, confirm to the previous one?",
                    QMessageBox::Ok | QMessageBox::Cancel);
        if(ret == 0x00400000) return;
    }
    showImgByIdx(curImgIndex - 1);
}

void FlagForm::nextImg()
{
    if(poseLabelIdx != 0 && isPoseLabel == true) {
        int ret = QMessageBox::information(
                    this, "Confirm",
                    "Change not saved, confirm to the next one?",
                    QMessageBox::Ok | QMessageBox::Cancel);
        if(ret == 0x00400000) return;
    }
    showImgByIdx(curImgIndex + 1);
}

//=======================function btn =========================================

void FlagForm::deleImg()
{
    int retn = QMessageBox::information(this, "confirm", "Delete image?",
                                        QMessageBox::Ok | QMessageBox::Cancel);
    if(retn == 0x00400000) return;

    QString cmd;
    //dalete image;
    QString deleImg = dealImages[curImgIndex].absoluteFilePath();
    cmd = QString("mv %1 %2").arg(deleImg).arg(GLOBALDEFINE::SYS_DELPATH);
    system(cmd.toStdString().c_str());

    //delete xml;
    QString deleXml = QString("%1%2.xml").arg(storePath).arg(curImgBaseName);
    if(GLOBALFUNC::inst()->confirmFileExist(deleXml)) {
        cmd = QString("mv %1 %2").arg(deleXml).arg(GLOBALDEFINE::SYS_DELPATH);
        system(cmd.toStdString().c_str());
    }

    //inner record:
    dealImages.removeAt(curImgIndex);
    totalCountLabel->setText(QString::number(dealImages.size()));
    if(curImgIndex > dealImages.size()-1) curImgIndex--;
    showImgByIdx(curImgIndex);
}

void FlagForm::resetImg()
{
    picBoard->labelReset();
    DoShowImgNULL();

    isPoseLabel = true;
    poseLabelFull = false;
    updatePosLabelIdx(0);
    updateToolBtn();
}

void FlagForm::reviseImg()
{    
    int size = GLOBALCONFIG::inst()->getAllKind();
    for(int i = 0; i < size; i++) {
        QComboBox* tmp = (QComboBox*)attrTable->cellWidget(i, 0);
        tmp->setEnabled(true);
    }

    isPoseLabel = true;
    int poseIdx = poseTable->currentRow();
    updatePosLabelIdx(poseIdx==-1?0:poseIdx);
    updateToolBtn();
}

void FlagForm::skipAttrImg()
{
    isPoseLabel = true;
    picBoard->skipCurrentData();
}

void FlagForm::submitImg()
{
    reLoadImg(picBoard);
    picBoard->labelRefreshPoseData();
    if(picBoard->labelSave()) {
        initialToolStatus(true);
        isPoseLabel = false;
        int size = GLOBALCONFIG::inst()->getAllKind();
        for(int i = 0; i < size; i++) {
            QComboBox* tmp = (QComboBox*)attrTable->cellWidget(i, 0);
            tmp->setEnabled(true);
        }
    } else {
        QMessageBox::information(this, "error", "save error!", QMessageBox::Ok);
        resetImg();
    }
}

void FlagForm::dataGetFromPicBoard(QList<QPoint> &data)
{    
    QString showDataStr;
    showDataStr.append(QString::number(data[0].x()));
    showDataStr.append(",");
    showDataStr.append(QString::number(data[0].y()));
    int size = data.size();
    for(int i = 1; i < size; i++) {
        showDataStr.append(",");
        showDataStr.append(QString::number(data[i].x()));
        showDataStr.append(",");
        showDataStr.append(QString::number(data[i].y()));
    }
    QTableWidgetItem* tmpItem = new QTableWidgetItem(showDataStr);
    poseTable->setItem(poseLabelIdx, 0, tmpItem);
    updateAttrTable(data);

    updatePosLabelIdx(poseLabelIdx + 1);
}

void FlagForm::skipToImgIdx()
{
    QString numStr = imageSkipEdit->text();
    int num = numStr.toInt()-1;
    showImgByIdx(num);
    imageSkipEdit->clear();
}

void FlagForm::estractToPath()
{
    QString tmpDir = QFileDialog::getExistingDirectory(
                this,tr("Find Files"), QDir::currentPath());
    if(tmpDir.isEmpty()) return;

    QString numStr = extractEdit->text();
    int num = numStr.toInt();
    for(int i = 0; i < num; i++) {
        QString imgpath = QString("%1/%2").arg(dealImages[i].absolutePath()).arg(curImgBaseName);
        QString newpath = QString("%1/%2").arg(tmpDir).arg(curImgBaseName);
        QString cmd = QString("cp %1.jpg %2.jpg").arg(imgpath).arg(newpath);
        system(cmd.toStdString().c_str());
        cmd.clear();

        QString imgpath1 = QString("%1/%2").arg(storePath).arg(curImgBaseName);
        cmd = QString("cp %1.xml %2.xml").arg(imgpath1).arg(newpath);
        system(cmd.toStdString().c_str());
    }
    imageSkipEdit->clear();
    QMessageBox::information(this, "tips", "finished!", QMessageBox::Ok);
}

void FlagForm::findImageName()
{
    QString findName = findNameEdit->text().simplified();
    int size = dealImages.size();
    for(int num = 0; num < size; num++){
        if(dealImages.at(num).baseName().compare(findName) == 0) {
            showImgByIdx(num);
            imageSkipEdit->clear();
            break;
        }
    }
}

//=======================key set=========================================
void FlagForm::closeEvent(QCloseEvent * /*e*/) {
    if(isPoseLabel == true) {
        int ret = QMessageBox::information(
                    this, "Confirm",
                    "Change not saved, confirm to exit?",
                    QMessageBox::Ok | QMessageBox::Cancel);
        if(ret == 0x00400000) return;
    }
}

void FlagForm::keyPressEvent(QKeyEvent *e) {
    if((e->key() == Qt::Key_Control)) {
        picBoard->drawingSwitch(isPoseLabel);
    } else if(e->key() == Qt::Key_X) {
    }
}

void FlagForm::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control) {
        picBoard->drawingSwitch(false);
    }
}
