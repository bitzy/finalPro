#include "waysInterface.h"

#include <opencv2/ximgproc.hpp>
//#include <opencv2/ximgproc/slic.hpp>
/**
 * @brief ATTRWAYS::sleeveBaseWay
 * Recognize the image sleeve.
 * @return
 */
using namespace cv::ximgproc;

int ATTRWAYS::sleeveBaseWay()
{
    //image preprocess:
    if(imgTarget->preprocessed() == false) {
        imgTarget->preprocess();
    }

    //extract superpixels
    cv::Ptr<SuperpixelSLIC> slic = createSuperpixelSLIC(
                imgTarget->src, SLICO);
    slic->iterate(25);
    cv::Mat labels;
    slic->getLabels(labels);

    return 1;
}
