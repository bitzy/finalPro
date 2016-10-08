#ifndef MYTOOL_H
#define MYTOOL_H

int fminvalue6(int x, int y, int z1=0x7fffffff, int z2=0x7fffffff,
               int z3 = 0x7fffffff, int z4 = 0x7fffffff);
int fmaxvalue6(int x, int y, int z1=-1, int z2=-1, int z3=-1, int z4=-1);

class HistoMat {
private:
    int histSize[3];
    float hranges[3][2];
    const float* ranges[3];
    int channels[3];

    void _setRGBmode();
    void _setHSImode();
public:
    HistoMat();
    void _setRGBMode();
};
#endif // MYTOOL_H
