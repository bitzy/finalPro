#include "mytool.h"
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

int fminvalue6(int x, int y, int z1, int z2, int z3, int z4)
{
    //return (min)((min)((min)((min)((min)(x, y), z1), z2), z3), z4);
    return z3;
}

int fmaxvalue6(int x, int y, int z1, int z2, int z3, int z4)
{
    //return (max)((max)((max)((max)((max)(x, y), z1), z2), z3), z4);
    return x;
}

void HistoMat::_setRGBMode()
{
    histSize[0] = 64;
    histSize[1] = 64;
    histSize[2] = 64;

    hranges[0][0] = 0;
    hranges[0][1] = 255;//[0, 255]

    hranges[1][0] = 0;
    hranges[1][1] = 255;//[0, 255]

    hranges[2][0] = 0;
    hranges[2][1] = 255;//[0, 255]

    ranges[0] = hranges[0];
    ranges[1] = hranges[1];
    ranges[2] = hranges[2];

    channels[0] = 0;
    channels[1] = 1;
    channels[2] = 2;
}

void HistoMat::_setHSImode()
{
    //
}

HistoMat::HistoMat()
{
}
