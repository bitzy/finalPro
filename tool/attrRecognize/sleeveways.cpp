#include "waysInterface.h"
#include <iostream>
using namespace std;

#include "planimetry_tools.h" //base tool
#include "superpixeltool.h"   //superpixels
using namespace cv;//imgData #include <imgproc.hpp>

#undef MYDEBUG
#define SUPERPIXELSSWITCH

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

double getWeight(const Point& p, const Point& A, const Point& B)
{
    double dist = getDistFromP2L(p, A, B);
    double weight = 2.0/(1+exp(dist));
    return weight;
}

double diff(const vector<Vec3b>& v, const vector<double>& w, int start, int end, Vec3b& p)
{
    double diffValue = 0;
    double totalw = 0;
    for(int i = start; i <= end; i++) {
        totalw += w[i];
    }
    Vec3f centerMid(0., 0., 0.);
    for(int i = start; i <= end; i++) {
        centerMid += (v[i]*w[i]/totalw);
        for(int j = i+1; j <= end; j++) {
            diffValue += getEucliDist(v[i], v[j])*w[i]*w[j]*2;
        }
    }
    centerMid += (v[end]*w[end]/totalw);
    p = centerMid;
    return diffValue;
}

int splitColor(const vector<Vec3b>& color, const vector<double>& weight)
{
    int res = 0;
    int size = color.size() - 1;
    /*
    cout << "size="<< size << endl;
    Mat img;
    img = Mat::zeros(200, 600, CV_8UC3);
    int startx = 0;
    for(int i = 0; i < size; i++) {
        int lenth = weight[i]*600;
        Vec3b tmpc = color[i];
        rectangle(img, Rect(startx, 0, lenth, 200), Scalar(tmpc[0], tmpc[1], tmpc[2]), -1);
        startx+=lenth;
    }
    imwrite("colormap.jpg", img);
    */

    bool flag = false;
    double conf;
    for(int splitIdx = 0; splitIdx < size; splitIdx++) {
        Vec3b upcolor, lowcolor;
        double upDIFF = diff(color, weight, 0, splitIdx, upcolor);
        double lowDIF = diff(color, weight, splitIdx+1, size, lowcolor);
        double BETWEN = getEucliDist(upcolor, lowcolor);
        double tmpDIF = BETWEN - upDIFF - lowDIF;
        //        cout << splitIdx << ":" << tmpDIF << "="
        //                << BETWEN << "-" << upDIFF << "-" << lowDIF << endl;
        if(!flag) {
            conf = tmpDIF; res = splitIdx; flag = true; continue;
        }
        if(tmpDIF > conf) {
            conf = tmpDIF; res = splitIdx;
        }
    }
    return res;
}

double ATTRWAYS::GetSleeveLenth(const ImgData* img)
{
#ifdef MYDEBUG
    cout << "\tget sleeve Lenth from left&right percent value..." << endl;
#endif
    double sleeveLenth = 0.0;

    //get cloth color:
    const int MAIN_COLORS = 5;
    int rgb_cloth[MAIN_COLORS][3];
    double percent_cloth[MAIN_COLORS];
    GetClothColors(img, rgb_cloth, percent_cloth, MAIN_COLORS);
    //    for(int i = 0; i < MAIN_COLORS; i++) {
    //        cout << "(" << rgb_cloth[i][0] << "," << rgb_cloth[i][1] <<
    //                "," << rgb_cloth[i][2] << "):" << percent_cloth[i] << endl;
    //    }

    //get skin color:
    int rgb_skin[3];
    GetSkinnColor(img, rgb_skin);
    //    cout << "(" << rgb_skin[0] << ","<< rgb_skin[1] << "," << rgb_skin[2] << ")" << endl;

    //======== left ======================================================
    vector<Point> lpoints;
    int uppos = posSimple(img->lshoud, img->lelbow, img->lhand, lpoints);
    //get color & weight:
    int size = lpoints.size()+2;
    vector<Vec3b> lcolors(size);
    vector<double> lweight(size);
    lcolors[0] = Vec3b(rgb_cloth[0][2], rgb_cloth[0][1], rgb_cloth[0][0]);
    lweight[0] = 0.5;
    //Mat printmat;
    //img->src.copyTo(printmat);
    for(unsigned int i = 0; i < lpoints.size(); i++) {
        //circle(printmat, lpoints[i], 1, Scalar(255,0,0), -1);
        lcolors[i+1] = img->_getPixelColorFromSrc(lpoints[i]);
        lweight[i+1] = 1;
#ifdef SUPERPIXELSSWITCH
        if(i < (unsigned int)uppos) {
            lweight[i+1] = getWeight(lpoints[i], img->lshoud, img->lelbow);
        } else lweight[i+1] = getWeight(lpoints[i], img->lelbow, img->lhand);
#endif
    }
    //    imwrite("sampleMat.jpg", printmat);
    lcolors[size-1] = Vec3b(rgb_skin[2], rgb_skin[1], rgb_skin[0]);
    lweight[size-1] = 1;
    normalnizeVector(lweight);
    int idx = splitColor(lcolors, lweight);
    for(int i = 0; i < idx; i++) sleeveLenth += lweight[i];

    //======== left end! ===================================================
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
    if(imgTarget->getPreprocessflag() == false) {
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
    cout << percent << endl;
    sleeveRes = sleeveLenthDiscrete(percent);
    return sleeveRes;
}
