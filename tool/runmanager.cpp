#include "runmanager.h"
#include <QtWidgets>
#include <QDateTime> //time
#include <QDomDocument> //save xml analyze file
#include "runthread.h"

#include "GLOBALDEFINE.h"
#include "GLOBALCONFIG.h"

RunManager::RunManager(
        const QString dealDir,
        const QString saveDir,
        const QList<int> &attrIdxs,
        const QList<int> &funcIdxs,
        bool saveFlag,
        const QString saveFname)
{
    QDir dir(dealDir);
    QStringList filter;
    filter << "*.jpg";
    images = dir.entryList(filter);
    //prepare data;
    regAttrIdx = attrIdxs;
    configExist = !saveFlag;
    configFname = saveFname;
    rowCount = perRowCount = 0;
    //createUI;
    UIcreate();
    //fill data;
    dealCount->setText(tr("0"));
    sumCounts->setText(QString::number(images.size()));
    dealPath->setText(dealDir);
    savePath->setText(saveDir);

    //begin recognize with thread;
    recogDoner = new runthread(dealDir, images, attrIdxs, funcIdxs);
    connect(recogDoner, SIGNAL(imgPerAttrDone(int)), this, SLOT(updateTableWidget(int)));
    connect(recogDoner, SIGNAL(finishedAll()), this, SLOT(overMessage()));
    recogDoner->dealBegin();

    startTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
}

void RunManager::updateTableWidget(int res)
{
    if(perRowCount == 0) {
        QStandardItem *Item0 = new QStandardItem(images[rowCount++]);
        model->setItem(rowCount-1, perRowCount++, Item0);
        dealCount->setText(QString::number(rowCount));
    }
    QStandardItem *newItem = new QStandardItem(QString::number(res));
    model->setItem(rowCount-1, perRowCount++, newItem);
    if(perRowCount == regAttrIdx.size()+1) perRowCount = 0;
}

void RunManager::overMessage()
{
    endTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    saveBtn->setEnabled(true);
    QMessageBox::information(this, "tips", "Finished!", QMessageBox::Ok);
}

void RunManager::stopRun()
{
    recogDoner->dealStop();
    stopBtn->setEnabled(false);
    saveBtn->setEnabled(true);
}

void RunManager::saveRes()
{
    /*
    QString filename = QString("%1test%2.xml").arg(savePath->text()).arg(endTime);
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text)) {
        QMessageBox::information(this, "error", "file save error!", QMessageBox::Ok);
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");

    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction(
                "xml", "Version=\"1.0\", encoding\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("testRes");
    root.setAttribute("starttime", startTime);
    root.setAttribute("endTime", endTime);
    root.setAttribute("total", sumCounts->text());
    root.setAttribute("directory", dealPath->text());
    doc.appendChild(root);

    QList<QList<int> > countAttr;
    //img date store:
    int saveRows = dealCount->text().toInt();
    int colums = model->columnCount();
//    int attrNum = attrconfig->attrKind();
    if(!configExist) {
//        for(int i = 0; i < attrNum; i++) {
//            int valueNum = attrconfig->getAttrValuesByIndex(i).size();
//            vector<int> tmp;
//            tmp.clear();
//            for(int j = 0; j < valueNum; j++)  {
//                tmp.push_back(0);
//            }
//            countAttr.push_back(tmp);
//        }
    }
    for(int i = 0; i < saveRows; i++) {
        QDomElement childNode = doc.createElement("img");
        root.appendChild(childNode);
        QString attrfName = model->item(i, 0)->text();
        childNode.setAttribute("Aname", attrfName);

        for(int j = 1; j < colums; j++) {//each attribute;
            QString attr = model->horizontalHeaderItem(j)->text();

            QString attrValue = model->item(i, j)->text();
            childNode.setAttribute(QString("%1_1").arg(attr), attrValue);
            //true value:
            QString truevalue;
            childNode.setAttribute(attr, truevalue);
        }

        if(!configExist) {
//            for(int j = 0; j < attrNum; j++) {
//                QString value = getAttrValueFromXml(
//                            attrfName, attrconfig->getAttrNameByIndex(j));
//                int valueIdx = attrconfig->getIndexByAttrValue(j, value);
//                countAttr[j][valueIdx]++;
//            }
        }
    }
    doc.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    if(!configExist) {
//        QFile newfile(configFname);
//        if(newfile.open(QIODevice::WriteOnly)) {
//           QTextStream out(&newfile);
//           for(int i = 0; i < attrNum; i++) {
//               int valueNum = countAttr[i].size();
//               for(int j = 0; j < valueNum; j++) {
//                   out << countAttr[i][j] << endl;
//               }
//           }
//        }
//        out.flush();
//        newfile.close();
    }
    QMessageBox::information(this, "info", "finished", QMessageBox::Ok);
    */
}

QLabel *RunManager::UIcreateLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignLeft);
    return lbl;
}

QPushButton *RunManager::UIcreateButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void RunManager::UIcreate()
{
    QHBoxLayout *line1 = new QHBoxLayout;
    line1->addWidget(UIcreateLabel(tr("Dealing Directory:")));
    line1->addWidget((dealPath = UIcreateLabel("")));
    line1->addStretch(1);

    QHBoxLayout *line2 = new QHBoxLayout;
    line2->addWidget(UIcreateLabel(tr("Save Directory:")));
    line2->addWidget((savePath = UIcreateLabel("")));
    line2->addStretch(1);

    QHBoxLayout *line3 = new QHBoxLayout;
    line3->addWidget(UIcreateLabel(tr("Finished/Total:")));
    line3->addWidget((dealCount = UIcreateLabel("")));
    line3->addWidget(UIcreateLabel(tr("/")));
    line3->addWidget((sumCounts = UIcreateLabel("")));
    line3->addStretch(1);
    line3->addWidget((stopBtn = UIcreateButton(tr("stop"), SLOT(stopRun()))));
    line3->addWidget((saveBtn = UIcreateButton(tr("save"), SLOT(saveRes()))));

    QTableView* table = new QTableView;
    model = new QStandardItemModel;
    QStringList tableHeader;
    tableHeader << "ImageName";
    int size = regAttrIdx.size();
    for(int i = 0; i < size; i++) {
        tableHeader << GLOBALCONFIG::inst()->getAttrNameByIndex(regAttrIdx[i]);
    }
    model->setHorizontalHeaderLabels(tableHeader);
    table->setModel(model);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(line1);
    layout->addLayout(line2);
    layout->addLayout(line3);
    layout->addWidget(table);
    setLayout(layout);
}
