#ifndef IMGDATA_H
#define IMGDATA_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * @brief The ImgData class
 * This class used for image recognization.
 */
class ImgData
{
public:
    bool initFlag;
    string MYIMG;
    string MYXML;
    //external function load standard poseDatas & attrDatas;
    vector<string> poseDatas;   //image standard pose data;
    vector<string> attrDatas;   //image standard attr data;
    friend class ATTRWAYS;      //ATTREAYS class can use imgData.

public:
    ImgData();

    //You must set img's path & corresponding xml's path;
    void initial(const string img, const string xml);
    //recognize function list;
};

#endif // IMGDATA_H
