#include "waysInterface.h"

int ATTRWAYS::textureWay1()
{
    if(imgTarget->preprocessed() == false) {
        imgTarget->preprocess();
    }

    //texture recognize:

    return 1;
}

