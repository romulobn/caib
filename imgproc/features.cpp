//
//  features.cpp
//  imgproc
//
//  Created by Romulo Bourget on 3/21/14.
//  Copyright (c) 2014 Romulo Bourget. All rights reserved.
//

#include "features.h"

/**
 Calculates the maximum diameter of a contour
 @param contour
 The contour
 @return the maximum diameter
 */
double caib::getMaxCalliper(const std::vector<cv::Point> &contour) {
    
    std::vector<cv::Point> convex_hull;
    cv::convexHull(contour, convex_hull);
    
    int p_a = 0, p_b = 0;
    int y_max = INT_MIN;
    int y_min = INT_MAX;
    
    for (int i = 0; i < convex_hull.size(); i++) {
        if (convex_hull[i].y < y_min) {
            p_a = i;
            y_min = convex_hull[i].y;
        }
        if (convex_hull[i].y > y_max) {
            p_b = i;
            y_max = convex_hull[i].y;
        }
    }
    
    double max_diameter = DBL_MIN;
    double rotated_angle = 0;
    caib::vect2D caliper_a = caib::vect2D(1, 0), caliper_b = caib::vect2D(-1, 0);
    
    while (rotated_angle < M_PI) {
        caib::vect2D edge_a, edge_b;
        
        cv::Point pt_a = convex_hull[p_a];
        cv::Point nextpt_a = convex_hull[ (p_a+1) % convex_hull.size() ];
        cv::Point pt_b = convex_hull[p_b];
        cv::Point nextpt_b = convex_hull[ (p_b+1) % convex_hull.size() ];
        
        edge_a = caib::vect2D( (nextpt_a.x - pt_a.x), (nextpt_a.y - pt_a.y) );
        edge_b = caib::vect2D( (nextpt_b.x - pt_b.x), (nextpt_a.y - pt_b.y) );
        
        double angle_a = edge_a.angleBtwVect(caliper_a);
        double angle_b = edge_b.angleBtwVect(caliper_b);
        double diameter = 0;
        
        caliper_a.rotateVect( std::min(angle_a, angle_b) );
        caliper_b.rotateVect( std::min(angle_a, angle_b) );
        
        if(angle_a < angle_b){
			p_a = (p_a + 1) % convex_hull.size();
            diameter = edge_a.distance2Points(nextpt_b, nextpt_a);
		}else{
			p_b = (p_b + 1) % convex_hull.size();
            diameter = edge_b.distance2Points(nextpt_a, nextpt_b);
		}
        
        rotated_angle = rotated_angle + std::min(angle_a, angle_b);
        max_diameter = std::max(diameter, max_diameter);
    }
    
    return max_diameter;
}


/**
 Calculates the minimum diameter of a contour
 @param contour
 The contour
 @return the minimum diameter
 */
double caib::getMinCalliper(const std::vector<cv::Point> &contour) {
    
    std::vector<cv::Point> convex_hull;
    cv::convexHull(contour, convex_hull);
    
    int p_a = 0, p_b = 0;
    int y_max = INT_MIN;
    int y_min = INT_MAX;
    
    for (int i = 0; i < convex_hull.size(); i++) {
        if (convex_hull[i].y < y_min) {
            p_a = i;
            y_min = convex_hull[i].y;
        }
        if (convex_hull[i].y > y_max) {
            p_b = i;
            y_max = convex_hull[i].y;
        }
    }
    
    double min_diameter = DBL_MAX;
    double rotated_angle = 0;
    caib::vect2D caliper_a = caib::vect2D(1, 0), caliper_b = caib::vect2D(-1, 0);
    
    while (rotated_angle < M_PI) {
        caib::vect2D edge_a, edge_b;
        
        cv::Point pt_a = convex_hull[p_a];
        cv::Point nextpt_a = convex_hull[ (p_a+1) % convex_hull.size() ];
        cv::Point pt_b = convex_hull[p_b];
        cv::Point nextpt_b = convex_hull[ (p_b+1) % convex_hull.size() ];
        
        edge_a = caib::vect2D( (nextpt_a.x - pt_a.x), (nextpt_a.y - pt_a.y) );
        edge_b = caib::vect2D( (nextpt_b.x - pt_b.x), (nextpt_a.y - pt_b.y) );
        
        double angle_a = edge_a.angleBtwVect(caliper_a);
        double angle_b = edge_b.angleBtwVect(caliper_b);
        double diameter = 0;
        
        caliper_a.rotateVect( std::min(angle_a, angle_b) );
        caliper_b.rotateVect( std::min(angle_a, angle_b) );
        
        if(angle_a < angle_b){
			p_a = (p_a + 1) % convex_hull.size();
            diameter = edge_a.distance2Points(nextpt_b, nextpt_a);
		}else{
			p_b = (p_b + 1) % convex_hull.size();
            diameter = edge_b.distance2Points(nextpt_a, nextpt_b);
		}
        
        rotated_angle = rotated_angle + std::min(angle_a, angle_b);
        min_diameter = std::min(diameter, min_diameter);
    }
    
    return min_diameter;
}

/**
 Calculates the mean of an histogram
 @param hist
 Input grayscale histogram
 @return Mean
 */
float caib::histMean(const caib::histogram &hist) {
    float sum = 0;
    for (int bin = 0 ; hist.hist_bin.size(); bin++) {
        sum += hist.hist_bin[bin] * hist.v_bin[bin];
    }
    
    return sum / hist.hist_bin.size();
}

/**
 Calculates the asymmetry of an histogram
 @param hist
 Input grayscale histogram
 @return Asymetry
 */
float caib::histAsymmetry(const caib::histogram &hist) {
    return caib::histMean(hist) - caib::histMedian(hist)[0];
}

/**
 Calculates the Kurtosis of an histogram
 @param hist
 Input grayscale histogram
 @return Kurtosis
 */
float caib::histKurtosis(const caib::histogram &hist) {
    std::vector<float> quartiles = caib::histMedian(hist, 4);
    std::vector<float> deciles = caib::histMedian(hist, 10);
    return (quartiles[2] - quartiles[0]) / (deciles[8] - deciles[0]);
}

/**
 Caluculates the warp metric distance
 This metric is calculated as described in
 "Global Warp Metric Distance: Boosting Content-based Image Retrieval through Histograms",
 J. C. Felipe, A. J. M. Traina, and C. Jr. Traina,
 Proceedings of the Seventh IEEE International Symposium on Multimedia (ISM’05), 2005
 @param hist1
 Input grayscale histogram
 @param hist2
 Input grayscale histogram
 @return the warp metric distance
 */
float caib::warpMetricDistance( const caib::histogram &hist1, const caib::histogram &hist2) {
    CV_Assert(hist1.hist_bin.size() == hist2.hist_bin.size());
    
    float wmd = 0;
    cv::Mat wmdMat = cv::Mat( cv::Size(hist1.hist_bin.size(), hist2.hist_bin.size()), cv::DataType<float>::type );
    
    for (int y = 0; y < wmdMat.rows; y++) {
        for (int x = 0; x < wmdMat.cols; x++) {
            wmdMat.at<float>(cv::Point(x,y)) = sqrt( pow(hist1.hist_bin[y] - hist2.hist_bin[x], 2));
        }
    }
    
    for (int x = 0, y = 0; x < wmdMat.cols && wmdMat.rows; ) {
        float min = FLT_MAX;
        int mx,my;
        
        for (int xi = 0; xi <= 1; xi++) {
            for (int yi = 0 ; yi <= 1; yi++) {
                if ( caib::isInside(wmdMat, cv::Point(xi,yi)) && !( x == 0 && y == 0 ) ) {
                    if(min > wmdMat.at<float>(cv::Point(x+xi,y+yi))) {
                        min = wmdMat.at<float>(cv::Point(x+xi,y+yi));
                        mx=xi;
                        my=yi;
                    }
                }
            }
        }
        
        x += mx;
        y += my;
        wmd += min;
    }
    
    return wmd;
}

/**
 Caluculates the global warp metric distance
 This metric is calculated as described in
 "Global Warp Metric Distance: Boosting Content-based Image Retrieval through Histograms",
 J. C. Felipe, A. J. M. Traina, and C. Jr. Traina,
 Proceedings of the Seventh IEEE International Symposium on Multimedia (ISM’05), 2005
 @param hist1
 Input grayscale histogram
 @param hist2
 Input grayscale histogram
 @return the global warp metric distance
 */
float caib::globalWarpMetricDistance(const caib::histogram &hist1, const caib::histogram &hist2) {
    float ca = 1 + sqrt( pow( caib::histAsymmetry(hist1) - caib::histAsymmetry(hist2), 2) );
    float ck = 1 + sqrt( pow( caib::histKurtosis(hist1) - caib::histKurtosis(hist2), 2) );
    float cp = 1 + ( sqrt( pow( caib::histPeaks(hist1).size() - caib::histPeaks(hist2).size(), 2) ) * 0.1 );
    float wmd = caib::warpMetricDistance(hist1, hist2);
    
    return ca * ck * cp * wmd;
}

/**
 Calculates the peaks of an histogram
 @param hist
 Input grayscale histogram
 @return Peaks
 */
std::vector<float> caib::histPeaks(const caib::histogram &hist, float percentage) {
    std::vector<float> temphist = hist.hist_bin;
    std::sort( temphist.begin(), temphist.end(), std::greater_equal<float>() );
    float medianfreq = temphist[temphist.size()/2];
    float maxfreq = temphist[temphist.size()-1];
    float diffreq = maxfreq * percentage;
    
    std::vector<float> peaks;
    
    for (int i = 0 ; i < hist.hist_bin.size(); i++) {
        bool fzone = ( hist.hist_bin[i] <= (medianfreq - diffreq) ) || (hist.hist_bin[i] >= (medianfreq + diffreq) );
       
        if (i == 0 || i == hist.hist_bin.size()) {
            if (fzone) {
                peaks.push_back(hist.hist_bin[0]);
            }
        } else {
            bool invpoint = ( (hist.hist_bin[i-1] - hist.hist_bin[i]) * (hist.hist_bin[i] - hist.hist_bin[i+1]) ) < 0;
            
            if (invpoint && fzone) {
                if (peaks.size() > 0) {
                    bool prevdif = std::sqrt( std::pow(peaks[peaks.size()-1] - hist.hist_bin[i], 2) ) >= diffreq;
                    if (prevdif) {
                        peaks.push_back( hist.hist_bin[i] );
                    }
                } else {
                    peaks.push_back( hist.hist_bin[i] );
                }
            }
            
        }
        
    }
    return peaks;
}

/**
 Find the median of an histogram according to n. This function can be used
 to also find the quartiles, deciles, etc.
 @param hist
 Input grayscale histogram
 @return Median
 */
std::vector<float> caib::histMedian(const caib::histogram &hist, const int &n ) {
    float sum = 0;
    for (int bin = 0 ; bin < hist.hist_bin.size(); bin++) {
        sum += hist.hist_bin[bin] * hist.v_bin[bin];
    }
    
    int step = std::ceil( sum / n );
    int a = 0;
    
    std::vector<float> values;
    
    for (int bin =  0; bin < hist.hist_bin.size() && step != sum; bin++) {
        a += hist.hist_bin[bin];
        if ( a >= step ) {
            values.push_back( hist.v_bin[bin] );
            step += step;
        }
    }
    
    return values;
}