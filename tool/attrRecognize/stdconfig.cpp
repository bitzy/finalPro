#include "stdconfig.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>//exit()
using namespace std;

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
    if(!infile) {
        cout << "Cann't find file:" << sys_attrConfigFile << endl;
        cout << "exit with ERROR code 10." << endl;
        exit(10);
    }

    const int LLIMIT = 255;
    char line[LLIMIT], tmpName[LLIMIT], tmpValues[LLIMIT];
    maxAlternative = -1;
    while(infile.getline(line, LLIMIT)) {
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
    if(!infile) {
        cout << "Cann't find file:" << sys_poseConfigFile << endl;
        cout << "exit with ERROR code 10." << endl;
        exit(10);
    }
    int tmpType;
    while(infile.getline(line, LLIMIT)) {
        sscanf(line, "%[^:]%*c%d", tmpName, &tmpType);
        poseName.push_back(tmpName);
        poseTypeIndex.push_back(tmpType);
    }
    infile.close();
    return true;
}

/*
const vector<string>& STDCONFIG::getAttrs() const {
    return attrName;
}

const vector<string> STDCONFIG::getAllAttrs() const
{
    vector<string> res;
    res.insert(attrName.begin(), attrName.end());
    res.insert(labelAttrName.begin(), labelAttrName.end());
    return res;
}
*/
int STDCONFIG::attrKind() const
{
    return attrName.size();
}

int STDCONFIG::getAllKind() const
{
    return attrName.size() + labelAttrName.size();
}

int STDCONFIG::getIndexByName(const string name) const
{
    int size = attrKind();
    for(int i = 0; i < size; i++) {
        if(!name.compare(attrName[i])) return i;
    }
    int size1 = (int)labelAttrName.size();
    for(int i = 0; i < size1; i++) {
        if(!name.compare(labelAttrName[i]))
            return i+size;
    }
    return -1;
}

string STDCONFIG::getAttrNameByIndex(int index) const
{
    if(index < attrKind()) return attrName[index];
    else return labelAttrName[index-attrKind()];
}

int STDCONFIG::getPoseCounter() const
{
    return poseName.size();
}

int STDCONFIG::getPoseIndexByName(const string name) const
{
    int size = getPoseCounter();
    for(int i = 0; i < size; i++) {
        if(!name.compare(poseName[i])) return i;
    }
    return -1;
}

string STDCONFIG::getPoseNameByIndex(int index) const
{
    if(index >= getPoseCounter()) return "error";
    return poseName[index];
}

int STDCONFIG::getPTypeIdxByIdx(int index) const
{
    if(index >= getPoseCounter()) return 0;
    return poseTypeIndex[index];
}


