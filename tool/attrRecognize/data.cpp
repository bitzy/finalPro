#include "data.h"

Data::Data()
{
    imgpath.clear();
}

bool Data::compareSame(string pimg)
{
    if(pimg.compare(imgpath) == 0) return true;
    return false;
}

/**
 * @brief Data::initial
 * @param pimg: image path;
 * @param p: pose data;
 * @param a: attr data;
 */
void Data::initial(
        string pimg,
        const vector<string> &p,
        const vector<string> &a)
{
    img = imread(pimg.c_str());  //cv_8uc3

    //load poseData
}
