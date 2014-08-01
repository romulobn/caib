//
//  utilities.h
//  imgproc
//
//  Created by Romulo Bourget on 2/17/14.
//  Copyright (c) 2014 CAIB. All rights reserved.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com

#ifndef __imgproc__utilities__
#define __imgproc__utilities__

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <algorithm>
#include <vector>
#include <cmath>

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#define M_PI 3.14159265359
#endif

#if (defined WIN32 || defined _WIN32 || defined WINCE) && defined CAIBAPI_EXPORTS
# define CAIB_EXPORTS __declspec(dllexport)
#else
# define CAIB_EXPORTS
#endif

namespace caib {
    
    //2D vector
    struct vect2D {
        double x, y;
        vect2D(): x(0), y(0) {}
        vect2D( double x1, double y1 ): x(x1), y(y1) {}
        double getMod(){ return sqrt( pow(x,2) + pow(y,2) ); } //mod
        double dotProduct( vect2D vec ){ return x*vec.x + y*vec.y; } //dot product
        double angleBtwVect( vect2D vect){ return ( ( acos( dotProduct(vect) / ( getMod()*vect.getMod() ) ) ) ); } //angle between two vectors
        double distance2Points( cv::Point pt, cv::Point o ) {
            vect2D T = vect2D( pt.x - o.x, pt.y - o.y);
            
            double t = dotProduct(T);
            double d2;
            
            if (t <= 0.0f) {
                d2 = T.getMod();
            } else {
                double qx = o.x + ( t / dotProduct(*(this)) ) * x;
                double qy = o.y + ( t / dotProduct(*(this)) ) * y;
                
                T.x = pt.x - qx;
                T.y = pt.y - qy;
                d2 = T.getMod();
            }
            
            return d2;
        }
        void prodByScalar(double k){ x *= k;y *= k; } //scalar product
        void turnToUniVect(){ prodByScalar( getMod() ); } //transform to unitary vector
        void rotateVect(double angle, double scale = 1.0) {
            angle = angle*(180.0/M_PI);
            
            x = ( scale * std::cos(angle) ) * x + ( scale * std::sin(angle) ) * y + ( (1 - scale * std::cos(angle) ) * x - ( scale * std::sin(angle) ) * y );
            y = ( (-1) * scale * std::sin(angle) ) * x + ( scale * std::cos(angle) ) * y + ( ( scale * std::sin(angle) ) * x - ( 1 - scale * std::cos(angle) ) * y );
        }
    };
    
    //Histogram
    struct histogram {
        std::vector<float> hist_bin;
        std::vector<float> v_bin;
    };
    CAIB_EXPORTS histogram calcHistogram(const cv::Mat &input, int nbins, bool uniform = true, bool accumulate = false );
    
    //Relations
    CAIB_EXPORTS bool isBinary(const cv::Mat &input);
    CAIB_EXPORTS bool isGray(const cv::Mat &input);
    CAIB_EXPORTS bool isEqual(const cv::Mat &input1, const cv::Mat &input2);
    CAIB_EXPORTS bool isInside(const cv::Mat &input, const cv::Point &pt);
    
    //Labeled Image Handling
    CAIB_EXPORTS cv::Mat drawLabelsContours(const cv::Mat &input, const cv::Mat &labels, const cv::Scalar &color);
    CAIB_EXPORTS cv::Mat paintLabels(const cv::Mat &labels);
    CAIB_EXPORTS cv::Mat paintLabel(const cv::Mat &labels, const int &label, const cv::Scalar &color);
    
    //Deprecated
    //CAIB_EXPORTS std::vector<cv::MatND> calcLabelHist(const cv::Mat &input, cv::Mat labels, const std::vector<int> &channels, const std::vector<int> &histSize, const float *ranges[], bool uniform = true, bool accumulate = false);
    //CAIB_EXPORTS cv::MatND calcLabelHist(const cv::Mat &input, cv::Mat labels, const int &label, const std::vector<int> &channels, const std::vector<int> &histSize, const float *ranges[], bool uniform = true, bool accumulate = false);
    
};

#endif /* defined(__imgproc__utilities__) */
