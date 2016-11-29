#include "waysInterface.h"
#include "imgdata.h"    //imgTarget
#include <iostream>
using namespace std;

#include "planimetry_tools.h" //base tool
using namespace cv;


void ATTRWAYS::GetSkinnColor(const ImgData *img, int rgb[])
{
#ifdef MYDEBUG
    time_t startT, endT;
    time(&startT);
#endif
    int width = img->rshoud.x - img->lshoud.x;
    int hight = img->neck.y - img->head.y;
    Point topLeft(img->lshoud.x, img->head.y - hight/2);
    topLeft.y = (topLeft.y < 0? 0:topLeft.y);
    Rect faceArea(topLeft, Size(width, hight*2));
    Mat imgFaceROI = img->src(faceArea);

    Mat skinPoints = getFaceSkinP(imgFaceROI);

#ifdef MYDEBUG
    time(&endT);
    double cost = difftime(endT, startT);
    cout << "GetSkinColor cost Time: " << cost << " us." << endl;
#endif
}
