#include "planimetry_tools.h"
#define EOPS (1E-10)
using namespace cv;
#define DEBUG
#define DEBUG_OUTPUT_FILE "../TMP_OUTPUT/"

double getEucliDist(double p1x, double p1y, double p2x, double p2y) {
    return sqrt((p1x - p2x) * (p1x - p2x) +
                (p1y - p2y) * (p1y - p2y));
}

double getEucliDist(Point p1, Point p2) {
    return sqrt((double)(p1.x - p2.x) * (p1.x - p2.x) +
                (double)(p1.y - p2.y) * (p1.y - p2.y));
}

double getEucliDist(cv::Point3d p1, cv::Point3d p2)
{
    return sqrt((double)(p1.x-p2.x)*(p1.x-p2.x) +
                (double)(p1.y-p2.y)*(p1.y-p2.y) +
                (double)(p1.z-p2.z)*(p1.z-p2.z));
}

double getEucliDist(Vec3d p1, Vec3d p2)
{
    return sqrt((double)(p1[0]-p2[0])*(p1[0]-p2[0]) +
                (double)(p1[1]-p2[1])*(p1[1]-p2[1]) +
                (double)(p1[2]-p2[2])*(p1[2]-p2[2]));
}


void getDotsOnLine(Point p1, Point p2, std::vector<Point> &dots)
{    
    double dist = getEucliDist(p1, p2);
    double addx = (p2.x - p1.x)/dist;
    double addy = (p2.y - p1.y)/dist;

    dots.clear();
    for(int i = 0; ;i++) {
        Point p(int(p1.x + i*addx), int(p1.y + i*addy));
        dots.push_back(p);
        if(getEucliDist(p, p2) < 2) break;
    }
}


int getDotsOnLine(Point p1, Point p2, Point p3, std::vector<Point> &dots)
{
    int p1p2num = -1;
    dots.clear();
    std::vector<Point> res;
    getDotsOnLine(p1, p2, res);
    dots.assign(res.begin(), res.end());
    p1p2num = dots.size();

    getDotsOnLine(p2, p3, res);
    dots.insert(dots.end(), res.begin(), res.end());
    return p1p2num;
}

//p1 with Line(p2p3)
double getDistFromP2L(cv::Point p1, cv::Point p2, cv::Point p3) {
    double A = p3.y - p2.y;
    double B = p3.x - p2.x;
    double C = p3.x * p2.y - p2.x* p3.y;

    if(fabs(A) < EOPS && fabs(A) < EOPS) {
        return 0.0;
    }

    return abs(A * p1.x - B * p1.y + C)*1.0
            / sqrt(A * A + B * B);
}


void normalnizeVector(std::vector<double> &v)
{
    int size = v.size();
    double sum = 0;
    for(int i = 0; i < size; i++) sum += v[i];
    for(int i = 0; i < size; i++) v[i] /= sum;
}

HistoGram1D::HistoGram1D(int binNum, int minV, int maxV)
{
    histSize[0] = binNum;
    hranges[0] = minV;
    hranges[1] = maxV;
    ranges[0] = hranges;
    channels[0] = 0;
}

MatND HistoGram1D::getHistogram1D(const Mat &img)
{
    MatND hist;
    calcHist(&img, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
    return hist;
}

Mat HistoGram1D::getHistogramImage(const Mat &img)
{
    MatND hist = getHistogram1D(img);
    double maxVal = 0, minVal = 0;
    minMaxLoc(hist, &minVal, &maxVal, 0, 0);
    Mat histImg(255, histSize[0], CV_8U, Scalar(255));
    int hpt = static_cast<int>(230);
    for(int h = 0; h < histSize[0]; h++) {
        float binVal = hist.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);
        line(histImg, Point(h, histSize[0]), Point(h, histSize[0]-intensity),
                Scalar::all(0));
    }
    return histImg;
}


void outColors2IMG(const Mat &colors, const char *name)
{
#ifdef DEBUG
    assert(colors.channels() == 3);
    int row = colors.rows;
    int col = colors.cols;

    Mat outImg(row*10, col*10, CV_8UC3);
    for(int i = 0, rowptr = 0; i < row; i++, rowptr+= 10) {
        const Vec3b* ptr = colors.ptr<Vec3b>(i);
        for(int j = 0, colptr = 0; j < col; j++, colptr+=10) {
            rectangle(outImg, Rect(colptr, rowptr, 10, 10),
                      Scalar(ptr[j][0], ptr[j][1], ptr[j][2]), -1);
        }
    }
    std::string fpath = DEBUG_OUTPUT_FILE;
    fpath += name;
    imwrite(name, outImg);
#endif
}



Mat combineMatSample(const Mat &a, const Mat &b)
{
    Mat aps;
    if(!a.isContinuous()) {
        aps.create(a.rows, a.cols, a.type());
        a.copyTo(aps);
        aps = aps.reshape(0, aps.cols*a.rows);
    } else aps = a.reshape(0, a.cols*a.rows);
    Mat bps;
    if(!b.isContinuous()) {
        bps.create(b.rows, b.cols, b.type());
        b.copyTo(bps);
        bps = bps.reshape(0, b.rows*b.cols);
    } else bps = b.reshape(0, b.cols*b.rows);
    Mat res = Mat::zeros(aps.rows+bps.rows, 1, a.type());
    Mat resROI = res(Rect(0, 0, 1, aps.rows));
    aps.copyTo(resROI);
    resROI = res(Rect(0, aps.rows, 1, bps.rows));
    bps.copyTo(resROI);

    return res;
}

int getXFromLWithY(Point p1, Point p2, int y)
{
    assert(p1.y != p2.y);
    return (y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
}


double getAngleByFourP(Point p1, Point p2, Point p3, Point p4)
{
    double a_x = p2.x - p1.x;
    double a_y = p2.y - p1.y;
    double c_x = p4.x - p3.x;
    double c_y = p4.y - p3.y;
    double ab_mul_cb = a_x * c_x + a_y * c_y;
    double dist_ab = sqrt(a_x * a_x + a_y * a_y);
    double dist_cd = sqrt(c_x * c_x + c_y * c_y);
    double cosValue = ab_mul_cb / (dist_ab * dist_cd);
    return acos(cosValue);
}
