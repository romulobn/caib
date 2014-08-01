//
//  clis.h
//  imgproc
//
//  Created by Romulo Bourget on 2/18/14.
//  Copyright (c) 2014 CAIB. All rights reserved.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com
//
//  Note: This algorithm assumes an input image in sRGB color space
//  and follows the implemenation described in
//  "Color Image Segmentation Using Competitive Learning",
//  Toshio Uchiyama and Michael A. Arbib,
//  IEE Transactions on Pattern Analysis and Machine Intelligence,
//  Vol. 16, NO. 12, December 1994

#ifndef __imgproc__clis__
#define __imgproc__clis__

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "utilities.h"

namespace caib {
    
    CAIB_EXPORTS cv::Mat clis(const cv::Mat &inputImg, const double &alpha, const int &numcluster, const int &numwin = -1, const int &numiter = -1);
};

#endif /* defined(__imgproc__clis__) */