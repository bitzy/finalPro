#include "tinyxml.h"
#include "tinystr.h"
#include "../imgdata.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

string selectSingleNode(TiXmlElement* root, string nodeName) {
    string res;
    if(root == NULL) return res;
    TiXmlElement* pNode = root->FirstChildElement();
    for(; pNode; pNode = pNode->NextSiblingElement()) {
        if(!nodeName.compare(pNode->Value())) {
            res = pNode->GetText();
            cout <<  res << endl;
        }
    }
    return res;
}

bool loadxml2imgdata(const string &fpath, ImgData& data) {
    try{
        vector<string> posedata;
        vector<string> attrdata;

        TiXmlDocument *myDocument = new TiXmlDocument(fpath.c_str());
        myDocument->LoadFile();
        TiXmlElement *rootElement = myDocument->RootElement();
        //extract posedata&attrdata by tixmltlement

        TiXmlElement* poseDataNode = rootElement->FirstChildElement();
        int size = STDCONFIG::INST()->getPoseCounter();
        for(int i = 0; i < size; i++) {
            string poseName = STDCONFIG::INST()->getPoseNameByIndex(i);
            string poseNameValue = selectSingleNode(poseDataNode, poseName);
            if(poseNameValue.empty()) posedata.push_back("null");
            else posedata.push_back(poseNameValue);
        }

        TiXmlElement* attrDataNode = poseDataNode->NextSiblingElement();
        int size1 = STDCONFIG::INST()->getAllKind();
        for(int i = 0; i < size1; i++) {
            string attrName = STDCONFIG::INST()->getAttrNameByIndex(i);
            string attrNameValue = selectSingleNode(attrDataNode, attrName);
            if(attrNameValue.empty()) attrdata.push_back("null");
            else attrdata.push_back(attrNameValue);
        }
        //set imgdata:
        data.setPoseDatas(posedata);
        data.setAttrDatas(attrdata);
        data.setXmlDataLoadFlag(true);
    } catch(string& e) {
        data.setXmlDataLoadFlag(false);
        cout << e << endl;
        return false;
    }
    return true;
}
