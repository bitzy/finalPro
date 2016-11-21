#include "waysInterface.h"
#include <fstream>
#include <cstdio>

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
    vector<FUNCITEM> sleeveWays;

    //sleeve:
    FUNCITEM s1("baseWay", &ATTRWAYS::sleeveBaseWay);
    sleeveWays.push_back(s1);
    attrWays[1] = sleeveWays;

    //texture:
    vector<FUNCITEM> textureWays;
    FUNCITEM s2("TextureReg", &ATTRWAYS::textureWay1);
    textureWays.push_back(s2);
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
bool ATTRWAYS::recognize(ImgData &img, int attri, int wayj)
{    
    if(_between(attri, 0, attrWays.size())) {
        if(_between(wayj, 0, attrWays[attri].size())) {
            attrIdx = attri;
            waysIdx = wayj;
            result = -1;
            if(img.getxmlDataLoadFlag() == false) img.preprocess();
            ATTR_FUNC func = attrWays[attri][wayj].funcaddr;
            result = (this->*func)();
            return true;
        } else cout <<"wayIdx is error!" << endl;
    } else cout << "attrIdx is error!" << endl;
    return false;

}

string ATTRWAYS::getresult()
{
    string res = "[\tnot any answer!\t]\n\n";
    if (result != -1) {
        stringstream ss;
        ss << "[\t" << result << "\t]\n\n";
        res = ss.str();
    }
    return res;
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

