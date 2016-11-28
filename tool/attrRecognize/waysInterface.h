#ifndef WAYSINTERFACE_H
#define WAYSINTERFACE_H

#include <string>
#include <vector>
#ifndef MYDEBUG
#define MYDEBUG
#endif

class ImgData;

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
    std::vector<std::string> getWays(std::string name) const;
    std::string getresult();
    std::string getWaysDetail() const;

private:
    //======================== inner ======================
    //attribute's recognize ways;
    int sleeveBaseWay();
    int textureWay1();    

    //inner function:
    double GetSleeveLenth(const ImgData* img);    
    void GetClothColor(const ImgData* img);
    void getColorMap();
    void GetSkinnColor(const ImgData* img,
                       unsigned char &r,
                       unsigned char &g,
                       unsigned char &b);
};

#endif // WAYSINTERFACE_H

