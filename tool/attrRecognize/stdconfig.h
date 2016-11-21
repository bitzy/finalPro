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
public:
    static STDCONFIG* INST() {
        return &represant;
    }
    //const vector<string>& getAttrs() const;
    //const vector<string> getAllAttrs() const;
    int attrKind() const;
    int getAllKind() const;
    //int getIndexByName(const string name) const;
    string getAttrNameByIndex(int index) const;
    //QStringList getAttrValuesByattrName(QString name) const;
    //QStringList getAttrValuesByIndex(int index) const;
    //int getIndexByAttrValue(int AttrIndex, const string value) const;
    //string getAttrValue(int kind, int index) const;

    int getPoseCounter() const;
    //QStringList getPoseName() const;
    int getPoseIndexByName(const string name) const;
    string getPoseNameByIndex(int index) const;
    int getPTypeIdxByIdx(int index) const;
};

#endif // STDCONFIG_H
