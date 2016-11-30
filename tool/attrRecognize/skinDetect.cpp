#include "waysInterface.h"
#include "imgdata.h"    //imgTarget
#include <iostream>
using namespace std;

#include "planimetry_tools.h" //base tool
using namespace cv;
#include "colorkmeans.h"

void ATTRWAYS::GetSkinnColor(const ImgData *img, int rgb[])
{
#ifdef MYDEBUG
    time_t startT, endT;
    time(&startT);
#endif
    //extract area:
    int width = img->rshoud.x - img->lshoud.x;
    int hight = img->neck.y - img->head.y;
    Point topLeft(img->lshoud.x, img->head.y - hight/2);
    topLeft.y = (topLeft.y < 0? 0:topLeft.y);
    Rect faceArea(topLeft, Size(width, hight*1.5));
    Mat imgFaceROI = img->src(faceArea);

    //get skin points:
    Mat skinPoints = getFaceSkinP(imgFaceROI);
    ColorKmeansTool ctool(skinPoints);
    ColorFeats cfeat = ctool.getColorsByKmeans(2);
    rgb[0] = (cfeat[0].first)[2];
    rgb[1] = (cfeat[0].first)[1];
    rgb[2] = (cfeat[0].first)[0];

#ifdef MYDEBUG
    time(&endT);
    double cost = difftime(endT, startT);
    cout << "GetSkinColor cost Time: " << cost << " us." << endl;
#endif
}
