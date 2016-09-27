#include "GLOBALDEFINE.h"

const QString GLOBALDEFINE::SYS_LABELPATH_DEFAULT("/home/sharon/Documents/code/qt_project/finalpro/testImage");
const QString GLOBALDEFINE::SYS_BULKPATH_DEFAULT("/home/sharon/Videos/500Test");
const QString GLOBALDEFINE::SYS_DELPATH("../delImg/");
const QString GLOBALDEFINE::SYS_BULKPATH_DATA_FNAME("data.config");
const QString GLOBALDEFINE::SYSTEST_DIR( "../singleExeDir/");
const int GLOBALDEFINE::SYSTEM_BASEDATA_TYPES = 3;

vector<QString> GLOBALDEFINE::BASEDATA_NAMES;
vector<int> GLOBALDEFINE::BASEDATA_DATANUMS;
QStringList GLOBALDEFINE::imageTypefilter;
bool GLOBALDEFINE::_init = GLOBALDEFINE::init();

bool GLOBALDEFINE::init()
{
    GLOBALDEFINE::BASEDATA_DATANUMS.push_back(2);
    GLOBALDEFINE::BASEDATA_DATANUMS.push_back(4);
    GLOBALDEFINE::BASEDATA_DATANUMS.push_back(4);
    GLOBALDEFINE::BASEDATA_NAMES.push_back("Point");
    GLOBALDEFINE::BASEDATA_NAMES.push_back("Line");
    GLOBALDEFINE::BASEDATA_NAMES.push_back("Rectangle");
    GLOBALDEFINE::imageTypefilter << "*.jpg";
    return true;
}
