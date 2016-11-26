#ifndef STDCONFIG_H
#define STDCONFIG_H

#include <string>
#include <vector>
/**
 * @brief The STDCONFIG class
 * This class must consist with the GLOBALCONFIG.H
 */
class STDCONFIG
{
    static STDCONFIG represant;
    //attrconfig:
    static int maxAlternative;
    static std::vector<std::string> attrName;
    static std::vector<std::string> labelAttrName;
    static std::vector<std::vector<std::string> > attrValues;
    //poseconfig:
    static std::vector<std::string> poseName;
    static std::vector<int> poseTypeIndex;
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
    int getIndexByName(const std::string name) const;
    std::string getAttrNameByIndex(int index) const;
    //QStringList getAttrValuesByattrName(QString name) const;
    //QStringList getAttrValuesByIndex(int index) const;
    //int getIndexByAttrValue(int AttrIndex, const string value) const;
    //string getAttrValue(int kind, int index) const;

    int getPoseCounter() const;
    //QStringList getPoseName() const;
    int getPoseIndexByName(const std::string name) const;
    std::string getPoseNameByIndex(int index) const;
    int getPTypeIdxByIdx(int index) const;
};

#endif // STDCONFIG_H
