#ifndef STDCONFIG_H
#define STDCONFIG_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;
/**
 * @brief The STDCONFIG class
 * This class must consist with the GLOBALCONFIG.H
 */
class STDCONFIG
{
    static STDCONFIG represant;
    //attrconfig:
    static int maxAlternative;
    static vector<string> attrName;
    static vector<string> labelAttrName;
    static vector<vector<string> > attrValues;
    //poseconfig:
    static vector<string> poseName;
    static vector<int> poseTypeIndex;
    static bool _init;
    static bool init();
    STDCONFIG(){}
    STDCONFIG(const STDCONFIG&);
    friend class GLOBALCONFIG;
public:
    static STDCONFIG* INST() {
        return &represant;
    }
};

#endif // STDCONFIG_H
