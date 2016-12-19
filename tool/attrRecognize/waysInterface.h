#ifndef WAYSINTERFACE_H
#define WAYSINTERFACE_H

#include <string>
#include <vector>
#ifndef MYDEBUG
#define MYDEBUG
#endif

//class ImgData;
#include "imgdata.h"

class ATTRWAYS;
typedef int(ATTRWAYS::*ATTR_FUNC)();
class FUNCITEM {
public:
    std::string funcname;
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
    static std::vector<std::string> funcAttrType;
    static std::vector<std::vector<FUNCITEM> > attrWays;
    static bool _init;
    static bool init();
    ATTRWAYS(){}
    ATTRWAYS(const ATTRWAYS&);

    inline bool _between(int x, int l, int r);

    //bool debug;
    ImgData* imgTarget;
    int attrIdx;    //recognize the attrIdx attribute;
    int waysIdx;    //recognize by the way of waysIdx;
    int result;     //recognize result
public:
    static ATTRWAYS* instance() {
        return &represant;
    }
    //======================== outer ======================
    bool RECOGNIZE(ImgData *img, int attri, int wayj);
    ATTR_FUNC getFuncAttr(int attri, int wayj);
    std::vector<std::string> getWays(std::string name) const;
    int getresult();
    std::string getWaysDetail() const;

private:
    //======================== inner ======================
    //attribute's recognize ways;
    int collarBaseWay();
    int sleeveBaseWay();
    int uplenBaseWay();
    int textureBaseWay();

    //inner function:
    double GetSleeveLenth(const ImgData* img);
    void   GetClothColors(const ImgData* img, int rgb[][3], double* percent, int num=1);
    void   GetSkinnColor (const ImgData* img, int rgb[3]);
    void   helperGetColorMap();
};

#endif // WAYSINTERFACE_H

