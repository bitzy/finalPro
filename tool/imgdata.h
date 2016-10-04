#ifndef IMGDATA_H
#define IMGDATA_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * @brief The ImgData class
 * This class used for image recognization.
 *
 */
class ImgData
{
public:
    string MYIMGPATH;  //IMAGE path;
    vector<string> poseDatas;  //IMAGE POSE STANDARD DATA;
    vector<string> attrDatas;  //IMAGE ATTR STANDARD DATA;
    ImgData(string imgPath, string xmlPath);
    bool loadDataFromXML(const string &img, const string &xml);
    bool loadImgData();
};

#endif // IMGDATA_H
