#include "waysInterface.h"
#include "imgdata.h"    //imgTarget
#include <iostream>
#include <iomanip> //cout
//#include <cstdio> //printf
#include <ctime>
#include "colorkmeans.h"
using namespace std;

#include "planimetry_tools.h" //base tool
using namespace cv;

void printmat(const Mat& m, int a, int b) {
    for(int i = 0; i < a; i++) {
        for(int j = 0; j < b; j++) {
            printf("%8d", m.at<ushort>(i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void ATTRWAYS::GetClothColors(const ImgData *img, int rgb[][3], double *percent, int num)
{
#ifdef MYDEBUG
    time_t startT, endT;
    time(&startT);
#endif
    //assert(num <= 5);

    //color feature extract area1:    
    assert(img->neck.x > img->lshoud.x);
    assert(img->lhip.y > img->lshoud.y);
    Point offset1 = 0.2*Point(img->neck.x-img->lshoud.x, img->lhip.y-img->lshoud.y);
    int width1  = (img->neck.x - img->lshoud.x) - 2*offset1.x;
    int hight1 = (img->lhip.y - img->lshoud.y) - 2*offset1.y;
    width1 = (width1 > 200? 200:width1);//no more than 200x200;
    hight1 = (hight1 > 200? 200:hight1);
    Rect area1 = Rect(img->lshoud + offset1, Size(width1, hight1));

    //color feature extract area2:
    assert(img->rshoud.x > img->neck.x);
    assert(img->rhip.y > img->rshoud.y);
    Point offset2 = 0.2*Point(img->rshoud.x-img->neck.x, img->rhip.y-img->rshoud.y);
    int width2 = (img->rshoud.x - img->neck.x) - 2*offset2.x;
    int hight2 = (img->rhip.y - img->rshoud.y) - 2*offset2.y;
    width2 = (width2 > 200? 200:width2);//no more than 200x200;
    hight2 = (hight2 > 200? 200:hight2);
    Rect area2 = Rect(img->neck + offset2, Size(width2, hight2));

    //get sample pixels;
    Mat samplePixels = combineMatSample(img->src(area1), img->src(area2));

    //int width = (width1 < width2? width1 : width2);
    //int hight = (hight1 < hight2? hight1 : hight2);

    //extract color feature:
    //    const int BINNUM = int(pow(2.0, 15.0));
    //    HistoGram1D histtool(BINNUM, 0, 0x7fff);
    //    for(int i = 0; i < FOCUS_TARGET_NUM; i++) {
    //        vector<Mat> mv;
    //        focusROI[i].convertTo(focusROI[i], CV_16UC3);
    //        split(focusROI[i], mv); //0- B; 1- G; 2-R;

    //        Mat mData(mv[0].size(), CV_16U);
    //        mData = (mv[2]&0xf8)*128 + (mv[1]&0xf8)*4 + (mv[0]&0xf8)/8;
    //        MatND hist = histtool.getHistogram1D(mData);
    //    }

    //kmeans extract main color:
    ColorKmeansTool cfeatTool(samplePixels);
    ColorFeats cfeat = cfeatTool.getColorsByKmeans(num);
    //outColors2IMG(colors, "output.jpg");
    //ColorFeats cfeat = cfeatTool.getColorsByPartition(3000);
    int size = cfeat.size();
    for(int i = 0; i < size; i++) {
        Vec3b color = cfeat[i].first;
        rgb[i][0] = color[2];//bgr
        rgb[i][1] = color[1];
        rgb[i][2] = color[0];
        percent[i] = cfeat[i].second;
    }
#ifdef MYDEBUG
    time(&endT);
    double cost = difftime(endT, startT);
    cout << "GetClothColor cost Time: " << cost << " us."<< endl;
#endif
}

void ATTRWAYS::helperGetColorMap()
{
    int rrange = 256, grange = 256, brange = 256;
    int rstep = 16, gstep = 16, bstep = 16;
    int rn = rrange/rstep, gn = grange/gstep, bn = brange/bstep;//brange/bstep;
    int width = 10 * rn * gn;
    int height = 10 * bn;
    Mat colormap;
    colormap.create(height, width, CV_8UC3);
    Rect rect = Rect(0, 0, 10, 10);
    for(int k = 0, bb = 0; k < bn; k++, bb += bstep, rect.y += 10) {
        for(int i = 0, rr = 0; i < rn; i++, rr += rstep) {
            for(int j = 0, gg = 0; j < gn; j++, gg += gstep) {
                rect.x = i*10*gn + j*10;
                rectangle(colormap, rect, Scalar(rr, gg, bb), -1);
            }
        }
    }
    imwrite("colormap.jpg", colormap);
}


