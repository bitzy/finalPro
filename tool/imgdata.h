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
    string MYIMG;
    string MYXML;
    vector<string> poseDatas;  //IMAGE POSE STANDARD DATA;
    vector<string> attrDatas;  //IMAGE ATTR STANDARD DATA;

public:
    ImgData();
    bool initial(const string img, const string xml);
};

#endif // IMGDATA_H
