#ifndef WAYSINTERFACE_H
#define WAYSINTERFACE_H

#include <iostream>
#include <string>
#include <vector>
#include "../imgdata.h"
#include "data.h"
using namespace std;

class ATTRWAYS;
typedef int(ATTRWAYS::*ATTR_FUNC)();
class FUNCITEM {
public:
    string funcname;
    ATTR_FUNC funcaddr;
    FUNCITEM(const char* str, ATTR_FUNC func)
        :funcname(str), funcaddr(func) {}
};

/**
 * @brief The ATTRWAYS class
 * This class doing image's recognization.
 */
class ATTRWAYS {
    static ATTRWAYS represant;
    static vector<string> funcAttrType;
    static vector<vector<FUNCITEM> > attrWays;
    static bool _init;
    static bool init();
    ATTRWAYS(){}
    ATTRWAYS(const ATTRWAYS&);

    inline bool _between(int x, int l, int r);

    int attrIdx;    //recognize the attrIdx attribute;
    int waysIdx;    //recognize by the way of waysIdx;
    int result;     //recognize result
    Data myimgData;

public:
    static ATTRWAYS* instance() {
        return &represant;
    }
    //recognize img's attribute i by way j;
    bool recognize(const ImgData& img, int attri, int wayj);

    //preprocess img;
    void preprocess(const ImgData& img);

    vector<string> getWays(string name) const;
    //ATTR_FUNC getAttrFuncAddress(string attrName, int j) const;

    //attribute's recognize ways;
    int sleeveBaseWay();
};

#endif // WAYSINTERFACE_H

