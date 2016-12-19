#include "waysInterface.h"
#include <iostream>
#include "svmpredict.h" //svm predict
#include <opencv2/objdetect/objdetect.hpp> //calhog
#include "planimetry_tools.h" //geteuc
using namespace std;
using namespace cv;

enum COLLARTYPE {ROUND=0, VCOLLAR, F1COLLAR, F2COLLAR};
enum COLLAR_RV_PART {PART_L2=0, PART_L1, PART_CENTER, PART_R1, PART_R2};
enum COLLAR_F2_PART {FPART_L2=0, FPART_L1, FPART_R1, FPART_R2};
vector<vector<struct svm_model*> > models;
using COLLARNODE = std::pair<Point, float>;
using COLLARNODE_PAIR = std::pair<COLLARNODE, COLLARNODE>;

#undef MYDEBUG
//#define ROUND_DEBUG
//#define V_DEBUG
//#define F2_DEBUG

vector<float> calHog(const Mat& src) {
    int n = 16;
    double xscale = n*1.0/src.cols;
    double yscale = n*1.0/src.rows;
    Mat src2;
    src.copyTo(src2);
    resize(src2, src2, Size(0,0), xscale, yscale);
    HOGDescriptor *hog = new HOGDescriptor(Size(src2.cols, src2.rows), Size(16,16),
                                           Size(8,8), Size(8,8), 9);
    vector<float> descriptors;
    hog->compute(src2, descriptors, Size(1,1), Size(0,0));
    return descriptors;
}

std::vector<float> calHog2(const Mat& src) {
    int n = 32;
    double xscale = n*1.0/src.cols;
    double yscale = n*1.0/src.rows;
    Mat src2;
    src.copyTo(src2);
    resize(src2, src2, Size(0,0), xscale, yscale);
    HOGDescriptor *hog = new HOGDescriptor(Size(src2.cols, src2.rows), Size(16,16),
                                           Size(8,8), Size(8,8), 9);
    vector<float> descriptors;
    hog->compute(src2, descriptors, Size(1,1), Size(0,0));
    return descriptors;
}

float getFeatFromModelByHog16(const Mat& src, struct svm_model* model) {
    INPUTPREDICT inputData;
    inputData = calHog(src);

    OUTPUTPREDICT outputData;
    svm_predict2b(inputData, outputData, model);
    return outputData.conf1;
}

float getFeatFromModelByHog32(const Mat& src, struct svm_model* model) {
    INPUTPREDICT inputData;
    inputData = calHog2(src);

    OUTPUTPREDICT outputData;
    svm_predict2b(inputData, outputData, model);
    return outputData.conf1;
}

Rect getPcenteredMat(const Mat& src, const Point& p, int lenth) {
    assert(src.cols >= lenth && src.rows >= lenth);
    int x = (p.x - lenth/2);
    x = (x < 0? 0:x); x = (x+lenth < src.cols?x:src.cols-lenth);
    int y = (p.y - lenth/2);
    y = (y < 0? 0:y); y = (y+lenth < src.rows?y:src.rows-lenth);
    return Rect(Point(x, y), Size(lenth, lenth));
}

int COLLARNODE_pushback(COLLARNODE* array, int TOPN, int p, COLLARNODE t)
{
    if(p == TOPN) {
        if(array[p-1].second < t.second) p--;
        else return TOPN;
    }
    //insert to array with no delete:
    int res = p+1;
    while(p && array[p-1].second < t.second) {
        array[p] = array[p-1];
        p--;
    }
    array[p] = t;
    return res;
}

int COLLARNODE_PAIR_pushback(COLLARNODE_PAIR* array, int TOPN, int p, COLLARNODE_PAIR t) {
    if(p == TOPN) {
        if(array[p-1].first.second + array[p-1].second.second
                < t.first.second + t.second.second) p--;
        else return TOPN;
    }
    //insert to array with no delete:
    int res = p+1;
    while(p && array[p-1].first.second + array[p-1].second.second
          < t.first.second + t.second.second) {
        array[p] = array[p-1];
        p--;
    }
    array[p] = t;
    return res;
}

COLLARNODE expandCollarR(
        const Mat& src, const Point& p,
        float alpha, float beta,
        int LIMIT,
        COLLAR_RV_PART idx) {
    COLLARNODE res;

    bool flag = false;
    float angleStep = (beta - alpha)/5;
    float step = LIMIT/4.0;
    for(float len = step; len <= LIMIT; len += step) { //lenth
        for(float angle = alpha; angle <= beta; angle += angleStep) {
            Point tmpp(p.x + len*cos(angle), p.y + len*sin(angle));
            if(tmpp.x < 0 || tmpp.x >= src.cols || tmpp.y < 0 || tmpp.y > src.rows)
                continue;
            Rect rect = getPcenteredMat(src, tmpp, LIMIT);
            float conf = getFeatFromModelByHog16(src(rect), models[ROUND][idx]);
            if(!flag) { res.first = tmpp; res.second = conf; flag = true;}
            else if(conf > res.second) {res.first = tmpp; res.second = conf;}
        }
    }
    return res;
}

void getCollarConf_R(const Mat& src, const Point refLine, float& conf)
{
    Point start(refLine.x, 0), end(refLine.y, src.rows-1);
    int rectsize = src.cols/5; //parameters need to adjust;
#ifdef ROUND_DEBUG   
    Mat src2;
    src.copyTo(src2);
    circle(src2, start, 3, Scalar(0,255,255), -1);
    circle(src2, end, 3, Scalar(0,255, 255), -1);
    cout << "ROUND rectsize:" << rectsize << endl;
#endif
    //get top pnode from(start, end);==> roundCollar-part2;
    const int TOPN = 5;
    COLLARNODE toppnode[TOPN];
    int topptr = 0;
    int stepLen = rectsize/6;
    int stepNum = getEucliDist(start, end)/stepLen;
    for(int i = 1; i < stepNum; i++) {
        Point p = start*((stepNum-i)*1.0/stepNum) + end*(i*1.0/stepNum);
        Rect pRect = getPcenteredMat(src, p, rectsize);
        float pConf = getFeatFromModelByHog16(src(pRect), models[ROUND][PART_CENTER]);
        topptr = COLLARNODE_pushback(toppnode, TOPN, topptr, COLLARNODE(p, pConf));
    }
#ifdef  ROUND_DEBUG
    for(int i = 0; i < TOPN; i++) {
        circle(src2, toppnode[i].first, 3, Scalar(0,255,0), -1);
        cout << toppnode[i].second << endl;
    }
#endif

    //#undef ROUND_DEBUG
#ifdef ROUND_DEBUG
    cout << "stepLen:" << stepLen << endl;
    cout << "sample number:" << stepNum << endl;
    cout << "top " << TOPN << " center pints:" << endl;
    for(int i = 0; i < TOPN; i++) {
        cout << "(" << toppnode[i].first.x << "," << toppnode[i].first.y <<")";
        cout << "==>" << toppnode[i].second << endl;
        //circle(src2, toppnode[i].first, 3, Scalar(255,0,0), -1);
    }
    imwrite("cpoint2.jpg", src2);
#endif

    //search other parts;
    float theta = getAngleByFourP(Point(0,0), Point(10, 0), start, end);
    float maxConfSum = -1;
    vector<COLLARNODE> resCollar;
    const int RCOLLAR_PARTN = 5;
    resCollar.reserve(RCOLLAR_PARTN);
#ifdef ROUND_DEBUG
    Mat srcCollar;
    src.copyTo(srcCollar);
    Scalar colors[TOPN] = {Scalar(0,0,255), Scalar(0,255,0), Scalar(0,255,255), Scalar(0,0,0), Scalar(0,0,0)};
#endif
    for(int i = 0; i < TOPN; i++) {
        COLLARNODE part[RCOLLAR_PARTN];
        part[PART_CENTER] = toppnode[i];
        part[PART_L1] = expandCollarR(src, toppnode[i].first, theta+CV_PI/2, theta+3*CV_PI/4,rectsize, PART_L1);
        part[PART_L2] = expandCollarR(src, part[PART_L1].first,theta+5*CV_PI/8, theta+7*CV_PI/8,rectsize, PART_L2);
        part[PART_R1] = expandCollarR(src, toppnode[i].first,theta-3*CV_PI/4, theta-CV_PI/2,rectsize, PART_R1);
        part[PART_R2] = expandCollarR(src, part[PART_R1].first,theta-7*CV_PI/8, theta-5*CV_PI/8,rectsize, PART_R2);
        float confSum = 0;
        for(int j = 0; j < RCOLLAR_PARTN; j++) confSum += part[j].second;
        if(confSum > maxConfSum) {
            maxConfSum = confSum;
            for(int j = 0; j < RCOLLAR_PARTN; j++) resCollar[j] = part[j];
        }
#ifdef ROUND_DEBUG
        for(int j = 1; j < RCOLLAR_PARTN; j++) {
            //cout << part[j-1].first.x << "," << part[j-1].first.y;
            //cout << "==" << part[j].first.x << "," << part[j].first.y << endl;
            //circle(srcCollar, part[j].first, 5, color, -1);
            line(srcCollar, part[j].first, part[j-1].first, colors[i]);
        }
        cout << "ROUND collar-" << i+1 << " confsum:"<< confSum << endl;
#endif
    }
#ifdef ROUND_DEBUG
    imwrite("collarRound_lines.jpg", srcCollar);
    cout << "--------------------------------" << endl;
#endif
    conf = maxConfSum;
}

COLLARNODE expandCollarV(const Mat& src, const Point& p, float angle, int step, int LIMIT, COLLAR_RV_PART idx) {
    COLLARNODE res;
    bool flag = false;
    for(int len = step; len < LIMIT; len+= step) {
        Point tmpp(p.x+len*cos(angle), p.y+len*sin(angle));
        Rect rect = getPcenteredMat(src, tmpp, LIMIT);
        float conf = getFeatFromModelByHog16(src(rect), models[VCOLLAR][idx]);
        if(!flag) {res.first = tmpp; res.second = conf; flag = true;}
        else if(conf > res.second) {res.first = tmpp; res.second = conf;}
    }
    return res;
}

void getCollarConf_V(const Mat& src, const Point refLine, float& conf)
{
    Point start(refLine.x, 0), end(refLine.y, src.rows-1);
    int rectsize = src.cols/5; //parameters around equal 44px;
#ifdef V_DEBUG
    Mat src2;
    src.copyTo(src2);
    circle(src2, start, 3, Scalar(0,255,255), -1);
    circle(src2, end, 3, Scalar(0,255, 255), -1);
    cout << "VCOLLAR rectsize:" << rectsize << endl;
    //imwrite("collar_V.jpg", src2);
#endif
    const int TOPN = 6;
    COLLARNODE toppnode[TOPN];
    int topptr = 0;
    int stepLen = rectsize/5;
    int stepNum = getEucliDist(start, end)/stepLen;
    int searchWid = stepLen*3;
    for(int offw = -searchWid; offw <= searchWid; offw += stepLen) {
        Point offp(offw, 0);
        Point startt = start+offp;
        Point endt = end+offp;
        for(int i = 1; i < stepNum; i++) {
            Point p = startt*((stepNum-i)*1.0/stepNum) + endt*(i*1.0/stepNum);
            Rect pRect = getPcenteredMat(src, p, rectsize);
            float pConf = getFeatFromModelByHog16(src(pRect), models[VCOLLAR][PART_CENTER]);
            topptr = COLLARNODE_pushback(toppnode, TOPN, topptr, COLLARNODE(p, pConf));
#ifdef V_DEBUG
            circle(src2, p, 2, Scalar(0,255,0), -1);
            //cout << p.x << "," << p.y << "=" << pConf << endl;
#endif
        }
    }
#ifdef V_DEBUG
    cout << "steplen:" << stepLen << endl;
    cout << "sample number:" << stepNum << "x" << 6 << endl;
    cout << "top " << TOPN << " center pints:" << endl;
    for(int i = 0; i < TOPN; i++) {
        cout << "(" << toppnode[i].first.x << "," << toppnode[i].first.y << ")";
        cout << "==>" << toppnode[i].second << endl;
        circle(src2, toppnode[i].first, 3, Scalar(255,0,0), -1);
    }
    imwrite("collarV_CenterPoints.jpg", src2);
#endif

    //search other parts;
    const int VCOLLAR_PARTN = 5;
    float maxConfSum = -1;
    int step = (rectsize/10<3?3:rectsize/10);
    vector<COLLARNODE> resCollar;
    resCollar.reserve(VCOLLAR_PARTN);
#ifdef V_DEBUG
    Mat srcCollar;
    src.copyTo(srcCollar);
#endif
    for(int i = 0; i < TOPN; i++) {
        float theta = getAngleByFourP(start, toppnode[i].first, Point(0,0), Point(10, 0));
        Point s1 = (toppnode[i].first + start)*0.5;
        Point s2 = toppnode[i].first*0.25 +  start*0.75;

        COLLARNODE part[VCOLLAR_PARTN];
        part[PART_CENTER] = toppnode[i];
        part[PART_L1] = expandCollarV(src, s1, theta+CV_PI/2, step, rectsize, PART_L1);
        part[PART_L2] = expandCollarV(src, s2, theta+CV_PI/2, step, rectsize, PART_L2);
        part[PART_R1] = expandCollarV(src, s1, theta-CV_PI/2, step, rectsize, PART_R1);
        part[PART_R2] = expandCollarV(src, s2, theta-CV_PI/2, step, rectsize, PART_R2);
        float confSum = 0;
        for(int j = 0; j < VCOLLAR_PARTN; j++) confSum += part[j].second;
        if(confSum > maxConfSum) {
            maxConfSum = confSum;
            for(int j = 0; j < VCOLLAR_PARTN; j++) resCollar[j] = part[j];
        }
#ifdef V_DEBUG
        Scalar color = Scalar(rand()%256, rand()%256, rand()%256);
        line(srcCollar, s1, s2, color);
        line(srcCollar, part[PART_CENTER].first, part[PART_L1].first, color);
        line(srcCollar, part[PART_CENTER].first, part[PART_R1].first, color);
        line(srcCollar, part[PART_L1].first, part[PART_L2].first, color);
        line(srcCollar, part[PART_R1].first, part[PART_R2].first, color);
        cout << "V collar-" << i+1 << " confsum:" << confSum << endl;
#endif
    }
#ifdef V_DEBUG
    imwrite("collarV_lines.jpg", srcCollar);
    cout << "--------------------------------" << endl;
#endif
    conf = maxConfSum;
}

void getCollarConf_F1(const Mat& src, const Point /*refLine*/, float& conf) {
#ifdef F1_DEBUG
    imwrite("collarF1.jpg", src);
#endif
    conf = getFeatFromModelByHog32(src, models[F1COLLAR][0]);
}

void getCollarConf_F2(const Mat& src, const Point /*refLine*/, float& conf) {
    //Point start(refLine.x, 0), end(refLine.y, src.rows-1);
    int rectsize = src.cols/5;
    //#define F2_DEBUG
#ifdef F2_DEBUG
    Mat src2;
    src.copyTo(src2);
    //circle(src2, start, 3, Scalar(0,255,255), -1);
    //circle(src2, end, 3, Scalar(0,255,255), -1);
    cout << "FCOLLAR rectsize:" << rectsize << endl;
    //imwrite("collarF2.jpg", src);
#endif    
    //get left&right points conf array;
    int stepLen = rectsize/3;
    vector<vector<COLLARNODE> > lnodes;
    vector<vector<COLLARNODE> > rnodes;
    int rows = src.rows / stepLen + 1;
    int cols = (src.cols/2) / stepLen + 1;
    for(int i = 0; i < rows; i++) {
        vector<COLLARNODE> lnodesPer;
        vector<COLLARNODE> rnodesPer;
        int py = i*stepLen;
        for(int j = 0; j < cols; j++) {
            Point pl(j*stepLen, py);
            Rect pRectl = getPcenteredMat(src, pl, rectsize);
            float pConfl = getFeatFromModelByHog32(src(pRectl), models[F2COLLAR][FPART_L1]);
            lnodesPer.push_back(COLLARNODE(pl, pConfl));

            Point pr(src.cols-pl.x, py);
            Rect pRectr = getPcenteredMat(src, pr, rectsize);
            float pConfr = getFeatFromModelByHog32(src(pRectr), models[F2COLLAR][FPART_R1]);
            rnodesPer.push_back(COLLARNODE(pr, pConfr));
        }
        lnodes.push_back(lnodesPer);
        rnodes.push_back(rnodesPer);
    }
    const int TOPN = 3;
    COLLARNODE_PAIR toppnode[TOPN];
    int topptr = 0;
    int searchIdxoff = 1;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            COLLARNODE rightTop[1];
            int rightTopptr = 0;
            for(int ii = i-searchIdxoff; ii <= i+searchIdxoff; ii++) {
                if(ii < 0 || ii >= rows) continue;
                for(int jj = j-searchIdxoff; jj <= j+searchIdxoff; jj++) {
                    if(jj < 0 || jj >= cols) continue;
                    COLLARNODE_pushback(rightTop, 1, rightTopptr, rnodes[ii][jj]);
                }
            }
#ifdef F2_DEBUG
            circle(src2, lnodes[i][j].first, 3, Scalar(0,255,0), -1);
#endif
            topptr = COLLARNODE_PAIR_pushback(
                        toppnode, TOPN, topptr,
                        COLLARNODE_PAIR(lnodes[i][j], rightTop[0]));
        }
    }
#ifdef F2_DEBUG
    cout << "steplen:" << stepLen << endl;
    cout << "sample number:" << rows << "x" << cols << endl;
    cout << "top " << TOPN << " center pints:" << endl;
    for(int i = 0; i < TOPN; i++) {
        cout << "(" << toppnode[i].first.first.x << ",";
        cout << toppnode[i].first.first.y << ")" << "+(";
        cout << toppnode[i].second.first.x << ",";
        cout << toppnode[i].second.first.y << ")" << "==>";
        cout << toppnode[i].first.second + toppnode[i].second.second << endl;
        circle(src2, toppnode[i].first.first, 3, Scalar(255,0,0), -1);
        circle(src2, toppnode[i].second.first, 3, Scalar(255,0,0), -1);
        line(src2, toppnode[i].first.first, toppnode[i].second.first, Scalar(0,0,255));
    }
    imwrite("collarF2_Centerpoints.jpg", src2);
#endif
    //search the other part_pair
    int searchLenth = rectsize*2;
    float maxConfSum  = -1;
    vector<COLLARNODE_PAIR> resCollar;
    resCollar.reserve(2);
#ifdef F2_DEBUG
    Mat srcCollar;
    src.copyTo(srcCollar);
#endif
    for(int k = 0; k < TOPN; k++) {
        Point lstartP(toppnode[k].first.first.x - searchLenth,
                      toppnode[k].first.first.y - searchLenth/5);
        Point rstartP(toppnode[k].second.first.x+ searchLenth,
                      toppnode[k].second.first.y- searchLenth/5);
        vector<vector<COLLARNODE> > lnodes;
        vector<vector<COLLARNODE> > rnodes;
        for(int i = 0; i*stepLen <= searchLenth; i++) {
            vector<COLLARNODE> lnodesPer;
            vector<COLLARNODE> rnodesPer;
            Point pl, pr;
            pl.y = lstartP.y + i*stepLen;
            pr.y = rstartP.y + i*stepLen;
            for(int j = 0; j*stepLen <= searchLenth; j++) {
                pl.x = lstartP.x + j*stepLen;
                pr.x = rstartP.x - j*stepLen;
                if(pl.x < 0 || pl.x >= src.cols || pr.x < 0 || pr.x >= src.cols)
                    continue;
                Rect pRectl = getPcenteredMat(src, pl, rectsize);
                float pConfl = getFeatFromModelByHog32(src(pRectl), models[F2COLLAR][FPART_L2]);
                lnodesPer.push_back(COLLARNODE(pl, pConfl));

                Rect pRectr = getPcenteredMat(src, pr, rectsize);
                float pConfr = getFeatFromModelByHog32(src(pRectr), models[F2COLLAR][FPART_R2]);
                rnodesPer.push_back(COLLARNODE(pr, pConfr));
            }
            lnodes.push_back(lnodesPer);
            rnodes.push_back(rnodesPer);
        }
        COLLARNODE_PAIR toppnodeOther[1];
        int topptr = 0;
        int size = lnodes.size();
        for(int i = 0; i < size; i++) {
            int lsize = lnodes[i].size();
            for(int j = 0, searchIdxoff=4; j < lsize; j++) {
                COLLARNODE rightTop[1];
                int rightTopptr = 0;
                for(int ii = i-searchIdxoff; ii <= i+searchIdxoff; ii++) {
                    if(ii<0 || ii >= size) continue;
                    for(int jj = j-searchIdxoff; jj <= j+searchIdxoff; jj++) {
                        if(jj < 0 || jj>= lsize) continue;
                        COLLARNODE_pushback(rightTop, 1, rightTopptr, rnodes[ii][jj]);
                    }
                }
                topptr = COLLARNODE_PAIR_pushback(
                            toppnodeOther, 1, topptr,
                            COLLARNODE_PAIR(lnodes[i][j], rightTop[0]));
            }
        }
        float confSum = toppnode[k].first.second + toppnode[k].second.second;
        confSum += toppnodeOther[0].first.second + toppnodeOther[0].second.second;
        if(confSum > maxConfSum) {
            maxConfSum = confSum;
            resCollar[0] = toppnode[k];
            resCollar[1] = toppnodeOther[0];
        }
#ifdef F2_DEBUG
        Scalar color = Scalar(rand()%256, rand()%256, rand()%256);
        cout << "F2 collar-" << k << " confsum:" << confSum << endl;
        //cout << " (" << toppnode[k].first.first.x << "," << toppnode[k].first.first.y << ")";
        //cout << "(" << toppnode[k].second.first.x << "," << toppnode[k].second.first.y << ")";
        //cout << "(" << toppnodeOther[0].first.first.x << "," << toppnodeOther[0].first.first.y << ")";
        //cout << "(" << toppnodeOther[0].second.first.x << ","<<toppnodeOther[0].second.first.y << ")" << endl;
        line(srcCollar, toppnode[k].first.first, toppnode[k].second.first, color);
        line(srcCollar, toppnode[k].first.first, toppnodeOther[0].first.first, color);
        line(srcCollar, toppnode[k].second.first, toppnodeOther[0].second.first, color);
#endif
    }
#ifdef F2_DEBUG
    imwrite("collarF2_lines.jpg", srcCollar);
    cout << "--------------------------------" << endl;
#endif
    conf = maxConfSum;
}

void loadcollarmodels() {
    const char* modelfilePath[4][5] = {
        {
            "./model/ModelForRound/trainDataSet1.dat.model", //part_left2
            "./model/ModelForRound/trainDataSet2.dat.model", //part_left1
            "./model/ModelForRound/trainDataSet3.dat.model", //part_center
            "./model/ModelForRound/trainDataSet4.dat.model", //part_right1
            "./model/ModelForRound/trainDataSet5.dat.model", //part_right2
        }, {
            "./model/ModelForV/trainDataSet1.dat.model", //part_l2
            "./model/ModelForV/trainDataSet2.dat.model", //part_l1
            "./model/ModelForV/trainDataSet3.dat.model", //center
            "./model/ModelForV/trainDataSet4.dat.model", //part_r1
            "./model/ModelForV/trainDataSet5.dat.model", //part_r2
        }, {
            "./model/ModelForF/ModelForFclose/trainDataSet.dat.model",
        }, {
            "./model/ModelForF/ModelForFopen/trainDataSet1.dat.model", //PART_L2
            "./model/ModelForF/ModelForFopen/trainDataSet2.dat.model", //PART_L1
            "./model/ModelForF/ModelForFopen/trainDataSet3.dat.model", //PART_R1
            "./model/ModelForF/ModelForFopen/trainDataSet4.dat.model"  //PART_R2
        }};
    vector<struct svm_model*> models_onecollar;
    struct svm_model* tmp;
    //round;
    for(int i = 0; i < 5; i++) {
        tmp = svm_load_model(modelfilePath[0][i]);
        models_onecollar.push_back(tmp);
    }
    models.push_back(models_onecollar);
    //vcollar;
    models_onecollar.clear();
    for(int i = 0; i < 5; i++) {
        tmp = svm_load_model(modelfilePath[1][i]);
        models_onecollar.push_back(tmp);
    }
    models.push_back(models_onecollar);
    //f1:
    models_onecollar.clear();
    tmp = svm_load_model(modelfilePath[2][0]);
    models_onecollar.push_back(tmp);
    models.push_back(models_onecollar);
    //f2:
    models_onecollar.clear();
    for(int i = 0; i < 4; i++) {
        tmp = svm_load_model(modelfilePath[3][i]);
        models_onecollar.push_back(tmp);
    }
    models.push_back(models_onecollar);
}

/**
 * @brief ATTRWAYS::collarBaseWay
 * Recognize the image collar. --> imgTarget
 * @return
 */
int ATTRWAYS::collarBaseWay()
{
    int collarRes = -1;
#ifdef MYDEBUG
    cout << "\n\n*collar recognizeing..." << endl;
#endif
    if(imgTarget->getPreprocessflag() == false) {
        imgTarget->preprocess();
    }

    loadcollarmodels();

    //for each collar type test:
    const int COLLARTYPE = 4;
    float conf[COLLARTYPE];
    void (*getCollarConf[COLLARTYPE])(const Mat& src, const Point line,float& collarConf) = {
            getCollarConf_R, getCollarConf_V, getCollarConf_F1, getCollarConf_F2};
    //focus area:
    Point hipCenter = (imgTarget->lhip+imgTarget->rhip)/2;
    int shouldLen = getEucliDist(imgTarget->lshoud, imgTarget->rshoud);
    int upbodyLen = getEucliDist(imgTarget->neck, hipCenter);
    Rect area[COLLARTYPE];//diff collar with diff area;
    Point line[COLLARTYPE];
    //r area:==>UNDO: RotatedRect;
    int width   = shouldLen;
    int height  = upbodyLen/3;
    area[0] = Rect(imgTarget->neck-Point(width/2, height/5), Size(width, height));
    area[0].x = (area[0].x < 0?0:area[0].x);
    area[0].y = (area[0].y < 0?0:area[0].y);
    int x1 = getXFromLWithY(imgTarget->neck, hipCenter, area[0].y);
    int x2 = getXFromLWithY(imgTarget->neck, hipCenter, area[0].y+height);
    line[0] = Point(x1-area[0].x, x2-area[0].x);
    //v area:
    height = upbodyLen/2;
    area[1] = area[0];
    area[1].height = height;
    x2 = getXFromLWithY(imgTarget->neck, hipCenter, area[1].y+height);
    line[1] = line[0];
    line[1].y = x2 - area[1].x;
    //f1 area:
    int lenth = shouldLen/2;
    area[2] = Rect(imgTarget->neck - Point(lenth/2, 0), Size(lenth, lenth));
    area[2].x = (area[2].x < 0?0:area[2].x);
    area[2].y = (area[2].y < 0?0:area[2].y);
    //f2 area:
    width   = shouldLen*3/5; //renew
    height  = shouldLen/2;//renew
    area[3] = Rect(imgTarget->neck - Point(width/2, height/5), Size(width, height));
    area[3].x = (area[3].x < 0?0:area[3].x);
    area[3].y = (area[3].y < 0?0:area[3].y);
    x1 = getXFromLWithY(imgTarget->neck, hipCenter, area[3].y);
    x2 = getXFromLWithY(imgTarget->neck, hipCenter, area[3].y+height);
    line[3] = Point(x1-area[0].x, x2-area[0].x);
    for(int i = 0; i < COLLARTYPE; i++) {
        (*getCollarConf[i])((imgTarget->src)(area[i]), line[i], conf[i]);
    }

#ifdef MYDEBUG
    cout << "*feature scaling..." << endl;
#endif
    //scale feature;
    const char* featFname = "collarFeature.txt";
    int left, right;
    FILE* fp = fopen("./model/modelPara/collarRange.txt", "r");
    FILE* fo = fopen(featFname, "w+");
    fscanf(fp, "%*[^\n]%*c%d%d", &left, &right);
    fprintf(fo, "0 ");
    for(int i = 0; i < COLLARTYPE; i++) {
        float a, b, tmp;
        fscanf(fp, "%*d%f%f", &a, &b);
        tmp = left + (conf[i]-a)*2/(b-a);
        fprintf(fo, "%d:%f ", i+1, tmp);
#ifdef MYDEBUG
        cout << conf[i] << " ==> " << tmp << endl;
#endif
    }
    fclose(fp);
    fclose(fo);

#ifdef MYDEBUG    
    cout << "\n*svm predict..." << endl;
#endif
    //svm-predict:
    const char* collarResFile = "collarRes.txt";
    char cmd[300];
    sprintf(cmd, "./model/svm %s ./model/modelPara/collarmodel.txt %s > svmOut.txt",
            featFname, collarResFile);
    system(cmd);
    fp = fopen(collarResFile, "r");
    fscanf(fp, "%d", &collarRes);
    fclose(fp);
#ifdef MYDEBUG
    cout << "collar recognize ok!" << collarRes << endl;
#endif

#ifndef MYDEBUG
    system("rm svmOut.txt");
    sprintf(cmd, "rm %s", featFname);
    system(cmd);
    sprintf(cmd, "rm %s", collarResFile);
    system(cmd);
#endif
    return collarRes;
}

