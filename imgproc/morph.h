//
//  morph.h
//  imgproc
//
//  Created by Romulo Bourget on 2/11/14.
//  Copyright (c) 2014 CAIB. All rights reserved.
//
//  Note: All the morphological operations bellow that support grayscale images
//  also support binary images.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com

#ifndef __imgproc__morph__
#define __imgproc__morph__

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "utilities.h"

#include <vector>
#include <queue>
#include <algorithm>


namespace caib {
    
    //Convertions
    CAIB_EXPORTS cv::Mat morphBinary(const cv::Mat &input, const double &r = 1);
    
    //Operations
    CAIB_EXPORTS cv::Mat morphIntersec(const cv::Mat &input1, const cv::Mat &input2);
    CAIB_EXPORTS cv::Mat morphUnion(const cv::Mat &input1, const cv::Mat &input2);
    CAIB_EXPORTS cv::Mat morphNeg(const cv::Mat &input);
    CAIB_EXPORTS cv::Mat morphSymdif(const cv::Mat &input1, const cv::Mat &input2);

    //Structuring Elements
    CAIB_EXPORTS cv::Mat morphSebox(const int &size = 1);
    CAIB_EXPORTS cv::Mat morphSecross(const int &size = 1);
    CAIB_EXPORTS cv::Mat morphSedisk(const int &size = 1);
    
    //Dilatations and Erosions
    CAIB_EXPORTS cv::Mat morphEro(const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphDil(const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphCero(const cv::Mat &input1, const cv::Mat &input2, const cv::Mat &se = morphSebox(), const int &n = 1);
    CAIB_EXPORTS cv::Mat morphCdil(const cv::Mat &input1, const cv::Mat &input2, const cv::Mat &se = morphSebox(), const int &n = 1);
    
    //Morphological Filters
    CAIB_EXPORTS cv::Mat morphOpen(const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphClose(const cv::Mat &input, const cv::Mat &se = morphSebox());
    
    //Conected Operators
    CAIB_EXPORTS cv::Mat morphSuprec(const cv::Mat &input1, const cv::Mat &input2, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphInfrec(const cv::Mat &input1, const cv::Mat &input2, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphOpenrec(const cv::Mat &input, const cv::Mat &eroSe = morphSebox(), const cv::Mat &infrecSe = morphSebox());
    CAIB_EXPORTS cv::Mat morphCloserec(const cv::Mat &input, const cv::Mat &dilSe = morphSebox(), const cv::Mat &supreSe = morphSebox());
    CAIB_EXPORTS cv::Mat morphClohole(const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphRegmax(const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphRegmin(const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphAreaopen(const cv::Mat &input, const double &a);
    CAIB_EXPORTS cv::Mat morphAreaclose(const cv::Mat &input, const double &a);
    
    //Residues
    CAIB_EXPORTS cv::Mat morphOpenth(const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphCloseth(const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphOpenrecth(const cv::Mat &input, const cv::Mat &eroSe = morphSebox(), const cv::Mat &infrecSe = morphSebox());
    CAIB_EXPORTS cv::Mat morphCloserecth(const cv::Mat &input, const cv::Mat &dilSe = morphSebox(), const cv::Mat &supreSe = morphSebox());
    CAIB_EXPORTS cv::Mat morphEdgeoff( const cv::Mat &input, const cv::Mat &se = morphSebox());
    CAIB_EXPORTS cv::Mat morphGradm(const cv::Mat &input, const cv::Mat &dilSe = morphSebox(), const cv::Mat &eroSe = morphSebox());
    
    //Measurements
    CAIB_EXPORTS cv::Mat morphLabel(const cv::Mat &input, const cv::Mat &se = morphSebox());
    
};

#endif /* defined(__imgproc__morph__) */
