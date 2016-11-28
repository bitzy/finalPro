#include "waysInterface.h"
#include "imgdata.h"    //imgTarget
#include <iostream>
using namespace std;

#include "planimetry_tools.h" //base tool
#include "superpixeltool.h"   //superpixels
using namespace cv;//imgData #include <imgproc.hpp>

//#define SUPERPIXELSSWITCH
#undef MYDEBUG

int posSimple(const Point& A, const Point& B, const Point& C, vector<Point>& points)
{
#ifdef MYDEBUG
    cout << "\t\tget human arm split dot..." << endl;
#endif
    int uppos = getDotsOnLine(A, B, C, points);
#ifdef SUPERPIXELSSWITCH
    cout << "\t\tpoints.size = " << points.size() << endl;
    uppos = SuperPixelTool::INST().transform(points, uppos);
    cout << "\t\tsp points.size = " << points.size() << endl;
#endif

#ifdef MYDEBUG
    cout << "\t\tsplit dot get." << endl;
#endif
    return uppos;
}

double getWeight(const Point& p, const Point& A, const Point& B) {
    double dist = getDistFromP2L(p, A, B);
    double weight = 2.0/(1+exp(dist));
    return weight;
}

int splitColor(const vector<Vec3b>& color, const vector<double>& weight) {
    int res = 0;
    return res;
}

double ATTRWAYS::GetSleeveLenth(const ImgData* img)
{
#ifdef MYDEBUG
    cout << "\tget sleeve Lenth from left&right percent value..." << endl;
#endif
    double sleeveLenth = 0.0;
    Vec3b clothColor, skinColor;
    GetClothColor(img, clothColor[0], clothColor[1], clothColor[2]);

    /*GetSkinnColor(img, skinColor[0], skinColor[1], skinColor[2]);
    //======== left ======================================================
    vector<Point> lpoints;
    int uppos = posSimple(img->lshoud, img->lelbow, img->lhand, lpoints);
    //get color & weight:
    int size = lpoints.size()+2;
    vector<Vec3b> lcolors(size);
    vector<double> lweight(size);
    lcolors[0] = clothColor;
    lweight[0] = 0.5;
    for(unsigned int i = 0; i < lpoints.size(); i++) {
        lcolors[i+1] = img->_getPixelColor(lpoints[i]);
        lweight[i+1] = 1;
#ifdef SUPERPIXELSSWITCH
        if(i < (unsigned int)(uppos)) {
            lweight[i+1] = getWeight(lpoints[i], img->lshoud, img->lelbow);
        } else {
            lweight[i+1] = getWeight(lpoints[i], img->lelbow, img->lhand);
        }
#endif
    }
    lcolors[size-1] = skinColor;
    lweight[size-1] = 1;
    normalnizeVector(lweight);
    //    for(int i = 0; i < size; i++) {
    //        cout << i << ": " << lweight[i] << endl;
    //    }
    //int idx = splitColor(lcolors, lweight);
    //======== left end! ===================================================

#ifdef MYDEBUG
    cout << "\tlValue=" << lValue << ";rValue=" << rValue << endl;
#endif*/
    return sleeveLenth;
}


int sleeveLenthDiscrete(double sleeveLenth) {
    int res = -1;
    if(sleeveLenth < 5) res = 0; //none;
    else if(sleeveLenth < 40) res = 1; //short;
    else if(sleeveLenth < 70) res = 2; //mid;
    else res = 3; //long
    return res;
}

//==============================================================================
/**
 * @brief ATTRWAYS::sleeveBaseWay
 * Recognize the image sleeve. -->imgTarget
 * @return
 */
int ATTRWAYS::sleeveBaseWay()
{
    int sleeveRes = -1;

#ifdef MYDEBUG
    cout << "\n\n*sleeve recognizing..." << endl;
#endif
    if(imgTarget->preprocessed() == false) {
#ifdef MYDEBUG
    cout << "*preprocess..." << endl;
#endif
        imgTarget->preprocess();
#ifdef MYDEBUG
    cout << "*preprocess ended!" << endl;
#endif
    }

#ifdef SUPERPIXELSSWITCH
    cout << "\t!super pixels switch = on" << endl;
    SuperPixelTool::INST().setImg(imgTarget->src);
#else
    cout << "\t!super pixels switch = off" << endl;
#endif
    double percent = GetSleeveLenth(imgTarget);
    sleeveRes = sleeveLenthDiscrete(percent);
    return sleeveRes;
}
