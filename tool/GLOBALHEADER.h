#ifndef GLOBALHEADER_H
#define GLOBALHEADER_H

#include <string>
#include <iostream>
#include <vector>

#ifdef WIN32
    #include <Windows.h>
    #include <io.h>
#else
    #include <unistd.h>
#endif
using namespace std;

#include <QFile>
#include <QString>
#include <QPoint>
#include <QStringList>

#endif // GLOBALHEADER_H
