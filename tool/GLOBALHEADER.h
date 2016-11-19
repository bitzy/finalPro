#ifndef GLOBALHEADER_H
#define GLOBALHEADER_H

#ifdef WIN32
    #include <Windows.h>
    #include <io.h>
#else
    #include <unistd.h>
#endif

#include <QFile>
#include <QString>
#include <QPoint>
#include <QStringList>
#include <QDebug>

#endif // GLOBALHEADER_H
