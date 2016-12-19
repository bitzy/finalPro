#include "svmpredict.h"
#include <cstdlib>

struct svm_node *x;
int max_nr_attr = 64;

void predictOne(INPUTPREDICT& input, OUTPUTPREDICT& output,
                struct svm_model* modelUsed) {
    int svm_type = svm_get_svm_type(modelUsed);
    int nr_class = svm_get_nr_class(modelUsed);

    double *prob_estimates = NULL;
    prob_estimates = (double*)malloc(nr_class*sizeof(double));
    int tmpsize = input.size();
    for(int i = 0; i < tmpsize; i++) {
        if(i >= max_nr_attr-1) {
            max_nr_attr *= 2;
            x = (struct svm_node*)realloc(x, max_nr_attr*sizeof(struct svm_node));
        }
        x[i].index = i+1;
        x[i].value = input[i];
    }
    x[tmpsize].index = -1;

    double predict_label;
    if(svm_type == C_SVC || svm_type == NU_SVC) {
        predict_label = svm_predict_probability(modelUsed, x, prob_estimates);
        output.n = (int)(predict_label + 0.5);
        output.conf1 = prob_estimates[0];
        output.conf2 = prob_estimates[1];
    }
    free(prob_estimates);
}

int svm_predict2b(
        INPUTPREDICT &input,
        OUTPUTPREDICT &output,
        svm_model *modelUsed) {
    //svm:
    x = (struct svm_node *)malloc(max_nr_attr*sizeof(struct svm_node));
    predictOne(input, output, modelUsed);
    free(x);
    return 0;
}
