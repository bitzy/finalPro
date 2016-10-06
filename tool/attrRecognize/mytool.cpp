#include "mytool.h"
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

int fminvalue6(int x, int y, int z1, int z2, int z3, int z4)
{
    return min(min(min(min(min(x, y), z1), z2), z3), z4);
}

int fmaxvalue6(int x, int y, int z1, int z2, int z3, int z4)
{
    return max(max(max(max(max(x, y), z1), z2), z3), z4);
}
