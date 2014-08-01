//
//  kumar.h
//  imgproc
//
//  Created by Romulo Bourget on 2/17/14.
//  Copyright (c) 2014 CAIB. All rights reserved.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com
//
//  Note: This algorithm was implemented following the paper
//  S. Kumar, S.H. Ong, S. Ranganatha, T.C. Ong, F.T. Chew,
//  "A rule-based approach for robust clump splitting",
//  Pattern Recognition 39 (2006) 1088–1098

#ifndef __imgproc__kumar__
#define __imgproc__kumar__

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "utilities.h"

#include <cmath>

namespace caib {
    
    //Kumar algorithm thresholds
    const double STD_CDTHRESH = 3.00;
    const double STD_SATHRESH = 0.12;
    const double STD_CLTHRESH = (90 * M_PI)/180;
    const double STD_CCTHRESH = (105 * M_PI)/180;
    const double STD_CATHRESH = 90.00;
    const double STD_CRTHRESH = 6.00;

    //perform Kumar algorithm for clump splitting
    CAIB_EXPORTS cv::Mat kumar(const cv::Mat &inputImg, const double &cd = STD_CDTHRESH, const double &sa = STD_SATHRESH, const double &cc = STD_CCTHRESH, const double &cl = STD_CLTHRESH, const double &cr = STD_CRTHRESH, const double &ca = STD_CATHRESH);

};

#endif /* defined(__imgproc__kumar__) */
