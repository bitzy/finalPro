#include "waysInterface.h"
#include "imgdata.h"
#include <iostream>
using namespace std;

#include <ctime>
#include "planimetry_tools.h"
using namespace cv;

int judgeTexture(const Mat& img) {
    //recognize texture type:
    //Single;Vertical;Horizontal;Grid;Point;Leopard;Other
    int res = -1;

    return res;
}

int ATTRWAYS::textureBaseWay()
{
    int textureRes = -1;
#ifdef MYDEBUG
    cout << "\n\n*sleeve recognizing..." << endl;
#endif
    if(imgTarget->preprocessed() == false) {
#ifdef MYDEBUG
        cout << "\tpreprocess..." << endl;
#endif
        imgTarget->preprocess();
#ifdef MYDEBUG
        cout << "\tpreprocess ended!" << endl;
#endif
    }

    //texture recognize:
#ifdef MYDEBUG
    time_t startT, endT;
    time(&startT);
#endif
    //texture feature extract area1:
    const int TEXTURE_NUM = 5;
    vector<Rect> areas;
    areas.resize(TEXTURE_NUM);
    //topleft:
    Point offset = Point((imgTarget->neck.x-imgTarget->lshoud.x)*0.2,
                         (imgTarget->lhip.y-imgTarget->lshoud.y)*0.1);
    int width = (imgTarget->neck.x - imgTarget->lshoud.x)-2*offset.x;
    int hight = (imgTarget->lhip.y - imgTarget->lshoud.y)/2 - 2*offset.y;
    areas[0] = Rect(imgTarget->lshoud + offset, Size(width, hight));//area1:
    //bottomleft:
    offset.y += (imgTarget->lhip.y - imgTarget->lshoud.y)/2;
    areas[2] = Rect(imgTarget->lshoud + offset, Size(width, hight));//area3;

    //topright:
    offset = Point((imgTarget->rshoud.x-imgTarget->neck.x)*0.2,
                   (imgTarget->rhip.y - imgTarget->rshoud.y)*0.1);
    width = (imgTarget->rshoud.x - imgTarget->neck.x)-2*offset.x;
    hight = (imgTarget->rhip.y - imgTarget->rshoud.y)/2-2*offset.y;
    areas[1] = Rect(imgTarget->neck + offset, Size(width, hight)); //area2;
    //bottomright:
    offset.y += (imgTarget->rhip - imgTarget->rshoud.y)/2;
    areas[3] = Rect(imgTarget->neck + offset, Size(width, hight)); //area4;
    //center:
    area[4] = Rect(Point(areas[0].x+areas[0].width/2, areas[0].y+areas[0].height/2),
            Size(width, hight));

    vector<int> textureAreas;
    textureAreas.reserve(TEXTURE_NUM);
    for(int i = 0; i < TEXTURE_NUM; i++) {
        textureAreas[i] = judgeTexture(imgTarget->src(areas[i]));
    }

#ifdef MYDEBUG
    time(&endT);
    double cost = difftime(endT, startT);
    cout << "GetTexture cost Time: " << cost << " us."<< endl;
#endif
    return textureRes;
}

