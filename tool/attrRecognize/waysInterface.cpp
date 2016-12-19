#include "waysInterface.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib> //exit();
using namespace std;

vector<string> ATTRWAYS::funcAttrType;
vector<vector<FUNCITEM> > ATTRWAYS::attrWays;
bool ATTRWAYS::_init = init();
ATTRWAYS ATTRWAYS::represant;

bool ATTRWAYS::init() {
    //funcAttrType initial:
    ifstream attrconfig("./configs/0attr.config");
    if(!attrconfig) {
        cout << "Cann't find file: ./configs/0attr.config"  << endl;
        cout << "exit with ERROR code 11." << endl;
        exit(11);
    }

    const int LINE_LIMIT = 255;
    char line[LINE_LIMIT], tmpname[LINE_LIMIT], tmpStr[LINE_LIMIT];
    while(!attrconfig.eof()) {
        attrconfig.getline(line, LINE_LIMIT);
        int ret = sscanf(line, "%[^:\n]%*c%[^;\n]", tmpname, tmpStr);
        if(ret != 2) continue;
        funcAttrType.push_back(tmpname);
    }
    attrconfig.close();

    //attrWays initial:
    attrWays.resize(funcAttrType.size());
    //===============================================
    //collar:
    vector<FUNCITEM> collarWays;
    FUNCITEM c1("baseWay", &ATTRWAYS::collarBaseWay);
    collarWays.push_back(c1);

    //sleeve:
    vector<FUNCITEM> sleeveWays;
    FUNCITEM s1("baseWay", &ATTRWAYS::sleeveBaseWay);
    sleeveWays.push_back(s1);

    //uplenth:
    vector<FUNCITEM> uplenWays;
    FUNCITEM u1("baseWay", &ATTRWAYS::uplenBaseWay);
    uplenWays.push_back(u1);

    //texture:
    vector<FUNCITEM> textureWays;
    FUNCITEM s2("TextureReg", &ATTRWAYS::textureBaseWay);
    textureWays.push_back(s2);

    attrWays[0] = collarWays;
    attrWays[1] = sleeveWays;
    attrWays[2] = uplenWays;
    attrWays[3] = textureWays;
    return true;
}

inline bool ATTRWAYS::_between(int x, int l, int r) {
    if(x >= l && x < r) return true;
    return false;
}

vector<string> ATTRWAYS::getWays(string name) const
{    
    int size = funcAttrType.size();
    int pos = -1;
    for(int i = 0; i < size; i++) {
        if(!name.compare(funcAttrType[i])) {
            pos = i;
            break;
        }
    }
    vector<string> res;
    if (pos == -1) return res;
    int waysNum = attrWays[pos].size();
    for(int i = 0; i < waysNum; i++) {
        res.push_back(attrWays[pos][i].funcname);
    }
    return res;
}

/**
 * @brief ATTRWAYS::recognize
 * @param attri
 * @param wayj
 * @return
 */
bool ATTRWAYS::RECOGNIZE(ImgData *img, int attri, int wayj)
{
    if(_between(attri, 0, attrWays.size())) {
        if(_between(wayj, 0, attrWays[attri].size())) {
            imgTarget = img;
            if(imgTarget->loadimgsOKFlag && imgTarget->poseDataOKFlag) {
                attrIdx = attri;
                waysIdx = wayj;
                result = -1;
                ATTR_FUNC func = attrWays[attri][wayj].funcaddr;
                result = (this->*func)();
                return true;
            } else return false;
        } else cout <<"wayIdx is error!" << endl;
    } else cout << "attrIdx is error!" << endl;
    return false;
}

ATTR_FUNC ATTRWAYS::getFuncAttr(int attri, int wayj)
{
    return attrWays[attri][wayj].funcaddr;
}

int ATTRWAYS::getresult()
{    
    return result;
}

string ATTRWAYS::getWaysDetail() const
{
    string res;
    int size = funcAttrType.size();
    for(int i = 0; i < size; i++) {
        res += funcAttrType[i];
        res += ":";
        stringstream ss;
        ss << attrWays[i].size();
        res += ss.str();
        res += "\n";
    }
    return res;
}

