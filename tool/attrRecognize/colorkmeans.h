#ifndef COLORKMEANS_H
#define COLORKMEANS_H

#include <vector>
#include <cassert>
#include <functional>
#include <map>
#include <opencv2/opencv.hpp>

/*
class colorTwister {
private:
    unsigned int state[624];
    unsigned int index;
    void reseed() {
        for(unsigned int i = 0; i < 624; i++) {
            unsigned int y = (state[i] & 0x80000000)
                    + (state[(i+1)%624]& 0x7fffffff);
            state[i] = state[(i+397)%624] ^ (y>>1);
            if(y%2 != 0) state[i] ^= 0x9908B0DF;
        }
    }

public:
    colorTwister(unsigned int seed) {
        index = 0;
        state[0] = seed;
        for(unsigned int i = 1; i < 624; i++) {
            unsigned int tmp = 0x6C078965*(state[i-1]^(state[i-1]>>30));
            state[i] = (i+tmp)&0xffffffff;
        }
    }
    unsigned int rand() {
        if(index == 0) reseed();
        unsigned int y = state[index];
        y ^= (y>>11);
        y ^= (y <<  7) & 0x9D2C5680;
        y ^= (y << 15) & 0xEFC60000;
        y ^= (y >> 18);
        index = (index + 1)%624;
        return y;
    }
};

class TheColor {
public:
    double component[3];
    TheColor(double comp1, double comp2, double comp3) noexcept;
    double operator[](int value) const {
        assert((0<=value && value <=2));
        return component[value];
    }
    double &operator [](int index) {
        assert((0 <= index && index <= 2));
        return component[index];
    }
    bool operator < (const TheColor& color2) const {
        const TheColor color1 = *this;
        if (color1[0] < color2[0]) return true;
        else if (color1[0] == color2[0]) {
            if (color1[1] < color2[1]) return true;
            else if (color1[1] == color2[1]) {
                if (color1[2] < color2[2]) return true;
                else if (color1[2] == color2[2]) return false;
                else return false;
            } else return false;
        } else return false;
    }
    bool operator = (const TheColor& color2) const {
        const TheColor color1 = *this;
        if((color1[0] == color2[0]) &&
                (color1[1] == color2[1]) &&
                (color1[2] == color2[2])) return true;
        else return false;
    }
    bool operator > (const TheColor& color2) const {
        const TheColor color1 = *this;
        if (color1[0] > color2[0]) return true;
        else if (color1[0] == color2[0]) {
            if (color1[1] > color2[1]) return true;
            else if (color1[1] == color2[1]) {
                if (color1[2] > color2[2]) return true;
                else if (color1[2] == color2[2]) return false;//0;
                else return false;
            } else return false;
        } else return false;
    }
    static double euclidean(const TheColor& color1, const TheColor& color2) noexcept;
    double euclidean(const TheColor& color) noexcept;
    //static const TheColor RGB2HSV(const TheColor& color) noexcept;
    //static const TheColor HSV2RGB(const TheColor& color) noexcept;
};
*/
class ColorKmeansTool;
namespace cv { class Mat; }
//enum ColorSpaceRGB {Red, Green, Blue};
using ColorWithCount = std::pair<int, unsigned int>;
//using Cluster = std::vector<TheColor>;
//using ClusteredPoint = std::map<unsigned int, std::vector<ColorWithCount>>;
using ColorFeatPer = std::pair<cv::Vec3b, double>;
using ColorFeats = std::vector<ColorFeatPer>;
using ColorKmeansCallback = std::function<void(const cv::Mat& colors)>;

class ColorKmeansTool
{
    cv::Mat& image;
    //    Cluster kmeans(const std::vector<ColorWithCount>& getPixels,
    //                   unsigned int k, double min_diff = 1.0) noexcept;
    //cv::Mat kmeans(const cv::Mat& samples, unsigned int k) noexcept;
    //inner tools:
    static cv::Mat resize(const cv::Mat&src, int wlimit, int hlimit, int interpolation) noexcept;
    struct CmpByValue {
        bool operator() (const ColorWithCount& lhs, const ColorWithCount& rhs) {
            return lhs.second > rhs.second;
        }
    };

    //static std::vector<ColorWithCount> getPixels(cv::Mat& img) noexcept;
    //static TheColor center(const std::vector<ColorWithCount>& colors) noexcept;
public:
    ColorKmeansTool(cv::Mat& img) noexcept;
    //void getMainColor(unsigned int number, ColorKmeansCallback callback, int convertColor=-1) noexcept;
    ColorFeats getMainColor(unsigned int number, int convertColor = -1) noexcept;
    // c++ 14
    //    template<typename E>
    //    static constexpr auto toType(E enumerator) noexcept {
    //        return static_cast<std::underlying_type<E>>(enumerator);
    //    }
    /*get std::tuple element quickly.*/
    template<typename E>
    constexpr typename std::underlying_type<E>::type
    toUType(E enumerator) noexcept {
        return static_cast<typename
                std::underlying_type<E>::type>(enumerator);
    }
};

#endif // COLORKMEANS_H
