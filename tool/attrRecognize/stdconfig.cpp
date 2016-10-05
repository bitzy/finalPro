#include "stdconfig.h"
#include <fstream>
#include <cstring>
#include <cstdio>

STDCONFIG STDCONFIG::represant;

int STDCONFIG::maxAlternative;
vector<string> STDCONFIG::attrName;
vector<string> STDCONFIG::labelAttrName;
vector<vector<string> > STDCONFIG::attrValues;
vector<string> STDCONFIG::poseName;
vector<int> STDCONFIG::poseTypeIndex;
bool STDCONFIG::_init = init();

#define sys_attrConfigFile "./configs/0attr.config"
#define sys_poseConfigFile "./configs/0pose.config"

bool STDCONFIG::init() {
    ifstream infile(sys_attrConfigFile);
    if(!infile) exit(10);

    const int LLIMIT = 255;
    char line[LLIMIT], tmpName[LLIMIT], tmpValues[LLIMIT];
    maxAlternative = -1;
    while(!infile.eof()) {
        infile.getline(line, LLIMIT);
        int ret = sscanf(line, "%[^:]%*c%[^\n]", tmpName, tmpValues);
        if(ret == 2) {//has value
            vector<string> values;
            char tmpValue[LLIMIT];
            int pos = 0;
            int lenth = strlen(tmpValues);
            while(pos < lenth && sscanf(tmpValues+pos, "%[^;]", tmpValue)) {
                values.push_back(tmpValue);
                pos += strlen(tmpValue)+1;
            }
            attrValues.push_back(values);
            attrName.push_back(tmpName);
        } else labelAttrName.push_back(tmpName);
    }
    infile.close();

    //pose config load:
    infile.open(sys_poseConfigFile);
    if(!infile) exit(10);
    int tmpType;
    while(!infile.eof()) {
        infile.getline(line, LLIMIT);
        sscanf(line, "%[^:]%*c%d", tmpName, &tmpType);
        poseName.push_back(tmpName);
        poseTypeIndex.push_back(tmpType);
    }
    infile.close();
    return true;
}

