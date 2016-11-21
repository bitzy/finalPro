#include "tinyxml.h"
#include "tinystr.h"
#include "../imgdata.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

bool loadxml2imgdata(const string &fpath, ImgData& data) {
    try{
        //vector<string> posedata;
        //vector<string> attrdata;
        TiXmlDocument *myDocument = new TiXmlDocument(fpath.c_str());
        myDocument->LoadFile();
        TiXmlElement *rootElement = myDocument->RootElement();
        cout << rootElement->Value() << endl;
    } catch(string& e) {
        cout << e << endl;
        return false;
    }
    return true;
}
