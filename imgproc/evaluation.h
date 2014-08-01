//
//  evaluation.h
//  imgproc
//
//  Created by Romulo Bourget on 2/19/14.
//  Copyright (c) 2014 CAIB. All rights reserved.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com

#ifndef __imgproc__evaluation__
#define __imgproc__evaluation__

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "morph.h"
#include "utilities.h"

namespace caib {
    
    //segmentation evaluation and precision coefficients
    CAIB_EXPORTS double areaOverlap(const cv::Mat &inputGST, const cv::Mat &inputS);
    CAIB_EXPORTS double areaSimilarity(const cv::Mat &inputGST, const cv::Mat &inputS);
    CAIB_EXPORTS double overlapError(const cv::Mat &inputGST, const cv::Mat &inputS);
    CAIB_EXPORTS double sensivity(const cv::Mat &inputGST, const cv::Mat &inputS);
    CAIB_EXPORTS int getFP(const cv::Mat &inputGST, const cv::Mat &inputS);
    CAIB_EXPORTS int getTP(const cv::Mat &inputGST, const cv::Mat &inputS);
    CAIB_EXPORTS int getFN(const cv::Mat &inputGST, const cv::Mat &inputS);

};


#endif /* defined(__imgproc__evaluation__) */
