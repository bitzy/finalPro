#include "runthread.h"
#include <QDir>

//runthread <==> imgdata
#include "attrRecognize/imgdata.h"
#include <iostream>

runthread::runthread(
        const QString dealPath,
        const QStringList imgs,
        const QList<int> &attrs,
        const QList<int> &ways)
{
    path = dealPath;
    images = imgs;
    runAttrs = attrs;
    runFuncs = ways;
}

void runthread::dealBegin()
{
    start();
}

void runthread::dealStop()
{
    terminate();
}

void runthread::run()
{
    ImgData* imgData = new ImgData;
    int funcNum = runAttrs.size();

    int size = images.size();
    for(int i = 0; i < size; i++) { //each image
        QString imgpath = path + images[i];
        QString xmlpath = path + images[i].mid(0, images[i].lastIndexOf(".")) + ".xml";
        imgData->loadIMGsrc(imgpath.toStdString(), xmlpath.toStdString());

        //each function recognize;
        for(int j = 0; j < funcNum; j++) {
            ATTRWAYS::instance()->RECOGNIZE(imgData, runAttrs[j], runFuncs[j]);
            int res = ATTRWAYS::instance()->getresult();
            emit imgPerAttrDone(res);
        }
    }
    emit finishedAll();
}
