#include "colorkmeans.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


Cluster ColorKmeansTool::kmeans(const std::vector<ColorWithCount> &pixels, unsigned int k, double min_diff)
noexcept {    
    Cluster clusters;
    unsigned int randmax = static_cast<unsigned int>(pixels.size());

    colorTwister mt(static_cast<unsigned int>(time(NULL)));
    for (unsigned int i = 0; i < k; i++) {
        auto iter = pixels.cbegin();
        for (unsigned int t = 0; t < mt.rand() % randmax; t++, iter++);
        clusters.emplace_back(iter->first);
    }

    while (1) {
        ClusteredPoint points;

        for (auto iter = pixels.cbegin(); iter != pixels.cend(); iter++) {
            TheColor color = iter->first;

            double smallestDistance = DBL_MAX;
            double distance;
            unsigned int smallestIndex;
            for (unsigned int i = 0; i < k; i++) {
                distance = color.euclidean(clusters[i]);

                if (distance < smallestDistance) {
                    smallestDistance = distance;
                    smallestIndex = i;
                }
            }
            points[smallestIndex].emplace_back(ColorWithCount(color, iter->second));
        }

        double diff = 0;
        for (unsigned int i = 0; i < k; i++) {
            TheColor oldCenter = clusters[i];
            TheColor newCenter = ColorKmeansTool::center(points[i]);
            clusters[i] = newCenter;
            diff = max(diff, oldCenter.euclidean(newCenter));
        }

        if (diff < min_diff) break;
    }

    return clusters;
}

ColorKmeansTool::ColorKmeansTool(Mat &_img) noexcept : image(_img) {}

void ColorKmeansTool::color(unsigned int number, ColorKmeansCallback callback, int convertColor)
noexcept {
    //convertColor = -1:no convert color space;
    Mat smallerImage;
    ColorKmeansTool::resize(image, smallerImage, 200, 200, CV_INTER_LINEAR);
    //imwrite("result.jpg", smallerImage);

    //color space & pixels;
    if (convertColor != -1) cvtColor(smallerImage, smallerImage, convertColor);
    vector<ColorWithCount> pixels = ColorKmeansTool::getPixels(smallerImage);

    //kmeans result:
    Cluster clusters = this->kmeans(pixels, number, 3000);
    callback(clusters);
}

void ColorKmeansTool::resize(const Mat &src, Mat &dest, int wlimit, int hlimit, int interpolation)
noexcept {
    int width  = src.cols;
    int height = src.rows;
    double ratio;
    if (height >= width) ratio = hlimit / double(height);
    else ratio = wlimit / double(width);
    cv::resize(src, dest, Size(), ratio, ratio, interpolation);
}

std::vector<ColorWithCount> ColorKmeansTool::getPixels(Mat &img)
noexcept {
    //get img's pixels count
    constexpr int R = 2;
    constexpr int G = 1;
    constexpr int B = 0;

    map<TheColor, unsigned int> cCounter;
    unsigned char r, g, b;//uint8_t

    Vec3b *pixel;
    cCounter.clear();
    for(int i = 0; i < img.rows; ++i) {
        pixel = img.ptr<Vec3b>(i);
        for (int j = 0; j < img.cols; ++j) {
            r = pixel[j][R];
            g = pixel[j][G];
            b = pixel[j][B];
            cCounter[TheColor(r, g, b)]++;
        }
    }
    //map ==> vector<ColorWidthCount>
    vector<ColorWithCount> pixels;
    for_each(cCounter.cbegin(), cCounter.cend(),
             //function do:
             [&pixels](const pair<TheColor, unsigned int>& pair) {
        pixels.emplace_back(ColorWithCount(pair.first, pair.second));}
    );    
    return pixels;
}

TheColor ColorKmeansTool::center(const std::vector<ColorWithCount> &colors)
noexcept {
    map<double, double> vals;
    double plen = 0;

    for_each(colors.cbegin(), colors.cend(),
             //function do:
             [&vals, &plen](const ColorWithCount& colorWithCount) {
        plen += colorWithCount.second;

        TheColor color = colorWithCount.first;
        for (int i = 0; i < 3; i++) {
            vals[i] += color[i] * colorWithCount.second;
        }
    });

    vals[0] /= plen;
    vals[1] /= plen;
    vals[2] /= plen;

    return TheColor(vals[0], vals[1], vals[2]);
}



TheColor::TheColor(double comp1, double comp2, double comp3)
noexcept {//0r, 1g, 2b;
    this->component[0] = comp1;
    this->component[1] = comp2;
    this->component[2] = comp3;
}

double TheColor::euclidean(const TheColor &color1, const TheColor &color2)
noexcept {
    double distance = 0;
    for(int i = 0; i < 3; i++) {
        distance += pow(color1[i] - color2[i], 2);
    }
    return sqrt(distance);
}

double TheColor::euclidean(const TheColor &color)
noexcept {
    return TheColor::euclidean(*this, color);
}

/*
const TheColor TheColor::RGB2HSV(const TheColor &color)
noexcept {
    double R = color[0]/255.0, G = color[1]/255.0, B = color[2]/255.0;
    double H, S, V;
    double min, max, delta,tmp;

    tmp = R>G?G:R; min = tmp>B?B:tmp;
    tmp = R>G?R:G;max = tmp>B?tmp:B;

    V = (max + min) / 2;
    delta = max - min;

    assert(max != 0);
    assert(delta != 0);
    S = delta / max;

    if (R == max) {
        if (G >= B) H = (G - B) / delta; // between yellow & magenta
        else H = (G - B) / delta + 6.0;
    } else if(G == max) {
        H = 2.0 + ( B - R ) / delta; // between cyan & yellow
    } else if (B == max) {
        H = 4.0 + ( R - G ) / delta; // between magenta & cyan
    }

    H *= 60.0; // degrees
    return TheColor(H, S, V);
}

const TheColor TheColor::HSV2RGB(const TheColor &color)
noexcept {
    double hue = color[0], p, q, t, ff;
    long i;
    TheColor rgb(0.0, 0.0, 0.0);

    assert(color[1] > 0.0);

    if(hue > 360.0) hue = 0.0;
    hue /= 60.0;
    i = (long)hue;
    ff = hue - i;
    p = color[2] * (1.0 - color[1]);
    q = color[2] * (1.0 - (color[1] * ff));
    t = color[2] * (1.0 - (color[1] * (1.0 - ff)));

    switch(i) {
    case 0: rgb[0] = color[2]; rgb[1] = t; rgb[2] = p; break;
    case 1: rgb[0] = q; rgb[1] = color[2]; rgb[2] = p; break;
    case 2: rgb[0] = p; rgb[1] = color[2]; rgb[2] = t; break;
    case 3: rgb[0] = p; rgb[1] = q; rgb[2] = color[2]; break;
    case 4: rgb[0] = t; rgb[1] = p; rgb[2] = color[2]; break;
    default:rgb[0] = color[2]; rgb[1] = p; rgb[2] = q; break;
    }
    return rgb;
}*/


