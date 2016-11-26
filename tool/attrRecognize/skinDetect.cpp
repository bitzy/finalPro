#include "waysInterface.h"
#include "imgdata.h"    //imgTarget
#include <iostream>
using namespace std;

#include "planimetry_tools.h" //base tool

void ATTRWAYS::GetSkinnColor(
        const ImgData *img,
        unsigned char& r,
        unsigned char& g,
        unsigned char& b)
{
    r = g = b = 5;
}
