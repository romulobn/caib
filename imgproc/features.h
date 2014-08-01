//
//  features.h
//  imgproc
//
//  Created by Romulo Bourget on 3/21/14.
//  Copyright (c) 2014 Romulo Bourget. All rights reserved.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com

#ifndef __imgproc__features__
#define __imgproc__features__

#include <iostream>
#include <vector>
#include <cmath>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "utilities.h"

namespace caib {
    CAIB_EXPORTS double getMaxCalliper( const std::vector<cv::Point> &contour);
    CAIB_EXPORTS double getMinCalliper( const std::vector<cv::Point> &contour);
    CAIB_EXPORTS float histMean( const caib::histogram &hist );
    CAIB_EXPORTS float histAsymmetry( const caib::histogram &hist );
    CAIB_EXPORTS float histKurtosis( const caib::histogram &hist );
    CAIB_EXPORTS float warpMetricDistance( const caib::histogram &hist1, const caib::histogram &hist2);
    CAIB_EXPORTS float globalWarpMetricDistance( const caib::histogram &hist1, const caib::histogram &hist2 );
    CAIB_EXPORTS std::vector<float> histPeaks( const caib::histogram &hist, float percentege = 0.2 );
    CAIB_EXPORTS std::vector<float> histMedian( const caib::histogram &hist, const int &n = 2 );
};



#endif /* defined(__imgproc__features__) */
