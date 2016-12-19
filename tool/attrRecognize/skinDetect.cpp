#include "waysInterface.h"
#include "imgdata.h"    //imgTarget
#include <iostream>
using namespace std;

#include "planimetry_tools.h" //base tool
using namespace cv;
#include "colorkmeans.h"

#define HAAR_FACE "./configs/haarcascade_frontalface_default.xml"
#define HAAR_EYES "./configs/haarcascade_eye.xml"

bool rectInside(const Rect& father, const Rect child) {
    if(!father.contains(Point(child.x, child.y))) return false;
    else {
        if(child.x + child.width <= father.x + father.width &&
                child.y + child.height <= father.y + father.height)
            return true;
        else return false;
    }
}

Mat getFaceSkinP(const Mat &img) {
    Mat res;
    Mat faceMask;
    faceMask = Mat::zeros(img.size(), CV_8UC1);

    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    equalizeHist(gray, gray);
    CascadeClassifier face, eyes;
    if(face.load(HAAR_FACE) && eyes.load(HAAR_EYES)){
        std::vector<Rect> faceVec, eyesVec;
        face.detectMultiScale(gray, faceVec, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(50,50));
        eyes.detectMultiScale(gray, eyesVec, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));
        std::cout << "get face:" << faceVec.size() << std::endl;
        std::cout << "get eyes:" << eyesVec.size() << std::endl;
        //face rect:
        //img.copyTo(res);
        Rect faceRect;
        if(faceVec.size() == 1) faceRect = faceVec[0];
        else if(faceVec.size() == 0) faceRect = Rect(0, 0, img.cols, img.rows);
        else {
            int pos = 0;
            int area0 = faceVec[0].area();
            for(unsigned int idx = 1; idx < faceVec.size(); idx++) {
                int tmparea = faceVec[idx].area();
                if(tmparea >= area0) {
                    area0 = tmparea;
                    pos = idx;
                } else continue;
            }
            faceRect = faceVec[pos];
        }
        Point center(faceRect.x+faceRect.width/2, faceRect.y+faceRect.height/2);
        circle(faceMask, center, faceRect.width/2, 1, -1);
        //faceMask(faceRect) = 1;
        for(unsigned int i = 0; i < eyesVec.size(); i++) {
            if(rectInside(faceRect, eyesVec[i])) {
                Rect tmp = eyesVec[i];
                Point center1(tmp.x+tmp.width/2, tmp.y+tmp.height/2);
                circle(faceMask, center1, tmp.width/2, 0, -1);
                //faceMask(eyesVec[i]) = 0;
            } else continue;
        }
        //img.copyTo(res, faceMask);
        //imwrite("skinpixels.jpg", res);
        std::vector<Vec3b> points;
        for(int i = 0; i < img.rows; i++) {
            for(int j = 0; j < img.cols; j++) {
                if(faceMask.at<uchar>(i, j) == 0) continue;
                points.push_back(img.at<Vec3b>(i, j));
            }
        }
        res = Mat(points);
        //        std::cout << res.type() << "; row=" << res.rows
        //                  << ";cols=" << res.cols << "channel="<< res.channels();
    } else {
        std::cout << "load classifier not found!";
    }
    return res;
}

void ATTRWAYS::GetSkinnColor(const ImgData *img, int rgb[])
{
#define MYDEBUG
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
#ifdef MYDEBUG
    imwrite("face.jpg", imgFaceROI);
#endif
    //get skin points:
    Mat skinPoints = getFaceSkinP(imgFaceROI);
    //    ColorKmeansTool ctool(skinPoints);
    //    ColorFeats cfeat = ctool.getColorsByKmeans(2);
    //    rgb[0] = (cfeat[0].first)[2];
    //    rgb[1] = (cfeat[0].first)[1];
    //    rgb[2] = (cfeat[0].first)[0];

#ifdef MYDEBUG
    time(&endT);
    double cost = difftime(endT, startT);
    cout << "GetSkinColor cost Time: " << cost << " us." << endl;
#endif
}
