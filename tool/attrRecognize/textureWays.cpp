#include "waysInterface.h"
#include "imgdata.h"
#include <iostream>
using namespace std;

int ATTRWAYS::textureWay1()
{
#ifdef MYDEBUG
    cout << "\n\n*sleeve recognizing..." << endl;
#endif

    if(imgTarget->preprocessed() == false) {
#ifdef MYDEBUG
    cout << "\tpreprocess..." << endl;
#endif
        imgTarget->preprocess();
#ifdef MYDEBUG
    cout << "\tpreprocess ended!" << endl;
#endif
    }

    //texture recognize:

    return 1;
}

