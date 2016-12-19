#ifndef SVMPREDICT_H
#define SVMPREDICT_H
#include <vector>
#include "libsvm/svm.h"

using INPUTPREDICT = std::vector<float>;

typedef struct outputPredict {
    double conf1;
    double conf2;
    int n;
}OUTPUTPREDICT;

int svm_predict2b(INPUTPREDICT& input, OUTPUTPREDICT& output,
                  struct svm_model *modelUsed);

#endif // SVMPREDICT_H
