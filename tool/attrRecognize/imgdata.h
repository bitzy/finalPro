#ifndef IMGDATA_H
#define IMGDATA_H

#include "stdconfig.h"  //has:#include <string>;#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The ImgData class
 * This class used for image recognization.
 *
 * You must "1) setIMGpath;" first
 * to confirm src_ori is ok. then you do save & recognize operation.
 *
 * this is confirmed by OKFLAG variable.
 */

#define DATABASEDIR "/home/sharon/Videos/TOOLDB/"
class ImgData
{
public:
    enum POSETYPE { TOP=1, HALF, ALL};
private:
    friend class ATTRWAYS;

    //---------- layout setting: -------------------------------
    std::string MYIMG;

    std::string DBxmlPATH;
    bool xmlPathsOKFlag;

    //external function load standard poseDatas & attrDatas;
    std::vector<std::string> poseDatas;   //image standard pose data;
    bool poseDataOKFlag;
    std::vector<std::string> attrDatas;   //image standard attr data;
    bool attrDataOKFlag;
    //---------- layout setting: -------------------------------

    //---------- programing setting: ---------------------------    

    cv::Mat src_ori;    //initial image;
    bool loadimgsOKFlag;
    cv::Mat src;
    bool preprocessFlag;        //images's process step;

    cv::Point2i head, neck, lshoud, rshoud, lelbow, relbow, lhand;
    cv::Point2i rhip, lknee, rknee, lank, rank, rhand, lhip;
    bool pointOKFlag;
    POSETYPE HSTYLE;

    //scale data;
    int xoffset, yoffset;
    double scale;
    //---------- programing setting: ---------------------------

public:    
    ImgData();    
    bool getXmlPathsOKflag() const;
    bool getPoseDataOKflag() const;
    bool getAttrDataOKflag() const;
    bool getLoadimgsOKflag() const;
    bool getPreprocessflag() const;
    bool getPointOKflag() const;

    //load data:
    void loadIMGsrc(const std::string img);
    void loadIMGsrc(const std::string img, const std::string xml);    

    //inner variable:    
    const std::string& getMYIMG() const;    
    const std::string& getXMLPath() const;
    const std::vector<std::string> &getPoseDatas() const;
    const std::vector<std::string> &getAttrDatas() const;    
    void setPoseDatas(const std::vector<std::string> &data);
    void setAttrDatas(const std::vector<std::string> &data);

    //function:
    void preprocess();
    //get image color at point;
    void getPColorFromIMG(const int x1, const int y1,
                       int&r, int&g, int&b) const;

private:    
    void _loadPoseDatafromxml();
    void setDefaultXMLPath(std::string str);
    void _loadPoints();
    void __resizePoint(cv::Point& p) const;
    void _resizePosePoints();
    cv::Rect __getBoundingBox();
    void _resizeSrcToFixedHeight(const cv::Rect& rect, int height = 800);
    cv::Vec3b _getPixelColorFromSRC_ORI(const cv::Point& p) const;
    cv::Vec3b _getPixelColorFromSrc(const cv::Point& p) const;
    int fminvalue6(int x, int y, int z1=0x7fffffff, int z2=0x7fffffff,
                   int z3 = 0x7fffffff, int z4 = 0x7fffffff);
    int fmaxvalue6(int x, int y, int z1=-1, int z2=-1, int z3=-1, int z4=-1);
};

#endif // IMGDATA_H
