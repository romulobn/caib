//
//  evaluation.cpp
//  imgproc
//
//  Created by Romulo Bourget on 2/19/14.
//  Copyright (c) 2014 CAIB. All rights reserved.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com

#include "evaluation.h"

/**
 Calculate the area overlap coefficient for a gold standar 
 binary input image and a segmentation binary input image
 @param inputGST
 Gold standard binary image
 @param inputS
 Segmented binary image
 @return Area overlap coefficient
 */
double caib::areaOverlap(const cv::Mat &inputGST, const cv::Mat &inputS) {
    CV_Assert ( caib::isBinary(inputGST) && caib::isBinary(inputS) && inputGST.cols == inputS.cols && inputGST.rows == inputS.rows);
    
    cv::Mat img_intersec = caib::morphIntersec(inputGST, inputS);
    cv::Mat img_union = caib::morphUnion(inputGST, inputS);
    
    int count_intersec = cv::countNonZero(img_intersec);
    int count_union = cv::countNonZero(img_union);
    
    return (double)count_intersec/(double)count_union;
}

/**
 Calculate the area similarity coefficient for a gold standar
 binary input image and a segmentation binary input image
 @param inputGST
 Gold standard binary image
 @param inputS
 Segmented binary image
 @return Area similarity coefficient
 */
double caib::areaSimilarity(const cv::Mat &inputGST, const cv::Mat &inputS) {
    CV_Assert ( caib::isBinary(inputGST) && caib::isBinary(inputS) && inputGST.cols == inputS.cols && inputGST.rows == inputS.rows);
    
    cv::Mat img_intersec = caib::morphIntersec(inputGST, inputS);
    
    int count_intersec = cv::countNonZero(img_intersec);;
    int count_inputGST = cv::countNonZero(inputGST);
    int count_inputS = cv::countNonZero(inputS);
    
    return ( 2.00 * (double)count_intersec )/( (double)count_inputGST + (double)count_inputS );
}

/**
 Calculate the area overlap error coefficient for a gold standar
 binary input image and a segmentation binary input image
 @param inputGST
 Gold standard binary image
 @param inputS
 Segmented binary image
 @return Area overlap error coefficient
 */
double caib::overlapError(const cv::Mat &inputGST, const cv::Mat &inputS) {
    CV_Assert ( caib::isBinary(inputGST) && caib::isBinary(inputS) && inputGST.cols == inputS.cols && inputGST.rows == inputS.rows);
    
    cv::Mat img_intersec = caib::morphIntersec(inputS, inputGST);
    cv::Mat img_union = caib::morphUnion(inputS, inputGST);
    
    int count_intersec = cv::countNonZero(img_intersec);
    int count_union = cv::countNonZero(img_union);
    int count_inputGST = cv::countNonZero(inputGST);
    
    return (double)(count_union-count_intersec)/(double)count_inputGST;
}

/**
 Calculate the sensitivity for a gold standar
 binary input image and a segmentation binary input image
 @param inputGST
 Gold standard binary image
 @param inputS
 Segmented binary image
 @return Sensitivity
 */
double caib::sensivity(const cv::Mat &inputGST, const cv::Mat &inputS) {
    double FN = getFN(inputGST, inputS);
    double TP = getTP(inputGST, inputS);
    
    return TP/(FN+TP);
}

/**
 Get the number of false negatives
 @param inputGST
 Gold standard binary image
 @param inputS
 Segmented binary image
 @return Number of false negatives
 */
int caib::getFN(const cv::Mat &inputGST, const cv::Mat &inputS) {
    
    cv::Mat img_FN = inputGST - caib::morphInfrec(caib::morphIntersec(inputGST, inputS), inputGST);
    
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    
    cv::findContours(img_FN, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    
    return (int)contours.size();
}

/**
 Get the number of true positives
 @param inputGST
 Gold standard binary image
 @param inputS
 Segmented binary image
 @return Number of true positives
 */
int caib::getTP(const cv::Mat &inputGST, const cv::Mat &inputS) {
    
    cv::Mat img_TP = caib::morphInfrec(caib::morphIntersec(inputGST, inputS), inputS);
    
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    
    cv::findContours(img_TP, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    
    return (int)contours.size();
}

/**
 Get the number of false positives
 @param inputGST
 Gold standard binary image
 @param inputS
 Segmented binary image
 @return Number of false positives
 */
int caib::getFP(const cv::Mat &inputGST, const cv::Mat &inputS) {
    
    cv::Mat img_FP = inputS - caib::morphInfrec(caib::morphIntersec(inputGST, inputS), inputS);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    
    cv::findContours(img_FP, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    
    return (int)contours.size();
}