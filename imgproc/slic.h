// SLIC.h: interface for using the SLIC algorithm.
//===========================================================================
// This code implements the zero parameter superpixel segmentation technique
// described in:
//
// "SLIC Superpixels Compared to State-of-the-art Superpixel Methods"
//
// Radhakrishna Achanta, Appu Shaji, Kevin Smith, Aurelien Lucchi, Pascal Fua,
// and Sabine Susstrunk,
//
// IEEE TPAMI, Volume 34, Issue 11, Pages 2274-2282, November 2012.
//
// NOTE: this code was adapted from the original version. The original version
// can be found at http://ivrg.epfl.ch/research/superpixels
//===========================================================================
// Copyright (c) 2013 Radhakrishna Achanta.
//
// For commercial use please contact the author:
//
// Email: radhakrishna.achanta@epfl.ch
//===========================================================================

#ifndef __imgproc__slic__
#define __imgproc__slic__

#include <vector>
#include <string>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <fstream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "utilities.h"

namespace caib {
    
    CAIB_EXPORTS cv::Mat slico(const cv::Mat &input, const int& K);
    
};


#endif /* defined(__imgproc__slic__) */
