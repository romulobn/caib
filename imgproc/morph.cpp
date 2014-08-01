//
//  morph.cpp
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

#include "morph.h"

/**
 Transform a grascale image to binary by applying an threshold operation
 @param input
 Input grayscale image
 @param r
 The threshold value
 @return Ouput binary image
 */
cv::Mat caib::morphBinary(const cv::Mat &input, const double &r) {
    CV_Assert( caib::isGray(input) );
    
    cv::Mat output = cv::Mat(input.size(), cv::DataType<uchar>::type);
    cv::threshold(input, output, r, 255, cv::THRESH_BINARY);
    
    return output;
}

/**
 Intersection of two grayscale images, to binary images this is equivalent 
 to apply the operation AND to each pixel
 @param input1
 Input grayscale image
 @param input2
 Input grayscale image
 @return Ouput grayscale image
 */
cv::Mat caib::morphIntersec(const cv::Mat &input1, const cv::Mat &input2) {
    CV_Assert( (caib::isGray(input1) || caib::isGray(input2)) && input1.cols == input2.cols && input1.rows == input2.rows );
    
    cv::Mat output = cv::Mat(input1.size(), cv::DataType<uchar>::type);
    cv::min(input1, input2, output);
    
    return output;
}

/**
 Union of two grayscale images, to binary images this is equivalent
 to apply the operation OR to each pixel
 @param input1
 Input grayscale image
 @param input2
 Input grayscale image
 @return Ouput grayscale image
 */
cv::Mat caib::morphUnion(const cv::Mat &input1, const cv::Mat &input2) {
    CV_Assert( (caib::isGray(input1) || caib::isGray(input2)) && input1.cols == input2.cols && input1.rows == input2.rows );
    
    cv::Mat output = cv::Mat(input1.size(), cv::DataType<uchar>::type);
    cv::max(input1, input2, output);
    
    return output;
    
}

/**
Invert the pixels values of a grayscale image
 @param input
 Input grayscale image
 @return Ouput grayscale image
 */
cv::Mat caib::morphNeg(const cv::Mat &input) {
    CV_Assert( caib::isGray(input) );

    return 255 - input;
}

/**
 Calculate the symetrical difference of two grayscale images
 @param input1
 Input grayscale image
 @param input2
 Input grayscale image
 @return Ouput grayscale image
 */
cv::Mat caib::morphSymdif( const cv::Mat &input1, const cv::Mat &input2) {
    return caib::morphUnion((input1 - input2), (input2 - input1));
}

/**
 Creates a structurant element with the form of a box
 @param size
 The size of the box
 @return Structurant element
 */
cv::Mat caib::morphSebox(const int &size) {
    return cv::getStructuringElement( cv::MORPH_RECT, cv::Size(2*size+1, 2*size+1),
                                     cv::Point( size, size ) );
}

/**
 Creates a structurant element with the form of a cross
 @param size
 The size of the cross
 @return Structurant element
 */
cv::Mat caib::morphSecross(const int &size) {
    return cv::getStructuringElement( cv::MORPH_CROSS, cv::Size(2*size+1, 2*size+1),
                                     cv::Point( size, size) );
}

/**
 Creates a structurant element with the form of a disk
 @param size
 The size of the disk
 @return Structurant element
 */
cv::Mat caib::morphSedisk(const int &size) {
    return cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(2*size+1, 2*size+1),
                                     cv::Point( size, size) );
}

/**
 Apply the mathematical morphology erosion on a grayscale image
 @param input
 Input grayscale image
 @param se
 Structurant Element
 @return Ouput grayscale image
 */
cv::Mat caib::morphEro(const cv::Mat &input, const cv::Mat &se) {
    CV_Assert( caib::isGray(input) );
    
    cv::Mat output = cv::Mat(input.size(), cv::DataType<uchar>::type);
    cv::erode( input, output, se);
    
    return output;
}

/**
 Apply the mathematical morphology dilatation on a grayscale image
 @param input
 Input grayscale image
 @param se
 Structurant Element
 @return Ouput grayscale image
 */
cv::Mat caib::morphDil(const cv::Mat &input, const cv::Mat &se) {
    CV_Assert( caib::isGray(input) );
    
    cv::Mat output = cv::Mat(input.size(), cv::DataType<uchar>::type);
    cv::dilate(input, output, se);
    
    return output;
}

/**
 Apply the mathematical morphology erosion on a grayscale image condionated
 to another grayscale image
 @param input1
 Input grayscale image to be eroded
 @param input2
 Input grayscale image that will be used as condition
 @param se
 Structurant element
 @param n
 Number of interations
 @return Ouput grayscale image
 */
cv::Mat caib::morphCero(const cv::Mat &input1, const cv::Mat &input2, const cv::Mat &se, const int &n) {
    cv::Mat output = caib::morphUnion(input1, input2);
    
    for (int i=0; i<n; i++)
        output = caib::morphUnion(caib::morphEro(output, se), input2 );
    
    return output;
}

/**
 Apply the mathematical morphology dilatation on a grayscale image condionated
 to another grayscale image
 @param input1
 Input grayscale image to be dilatated
 @param input2
 Input grayscale image that will be used as condition
 @param se
 Structurant element
 @param n
 Number of interations
 @return Ouput grayscale image
 */
cv::Mat caib::morphCdil(const cv::Mat &input1, const cv::Mat &input2, const cv::Mat &se, const int &n) {
    cv::Mat output = caib::morphIntersec(input1, input2);
    
    for (int i=0; i<n; i++)
        output = caib::morphIntersec(caib::morphDil(output, se), input2 );
    
    return output;
}

/**
 Apply the mathematical morpholoy opening on a grayscale image
 @param input
 Input grayscale image
 @param se
 Structurant element
 @return Ouput grayscale image
 */
cv::Mat caib::morphOpen(const cv::Mat &input, const cv::Mat &se) {
    return caib::morphDil( caib::morphEro(input, se), se );
}

/**
 Apply the mathematical morphology close opearation on a grayscale image
 @param input
 Input grayscale image
 @param se
 Structurant element
 @return Ouput grayscale image
 */
cv::Mat caib::morphClose(const cv::Mat &input, const cv::Mat &se) {
    return caib::morphEro( caib::morphDil(input, se), se );
}

/**
 Apply the mathematical morphology superior reconstuction of a grayscale image form another grayscale image
 @param input1
 Grayscale image that will be used to recontruct another image
 @param input2
 Grasycale image that will be reconstructed from another image
 @param se
 Structurant element
 @return Ouput grayscale image
 */
cv::Mat caib::morphSuprec(const cv::Mat &input1, const cv::Mat &input2, const cv::Mat &se) {
    cv::Mat output = input1;
    
    while ( true ) {
        cv::Mat temp = output;
        output = caib::morphCero(temp, input2, se);
        
        if ( caib::isEqual(output, temp) )
            break;
    }
    
    return output;
}

/**
 Apply the mathematical morphology inferior reconstuction of a grayscale image form another grayscale image
 @param input1
 Grayscale image that will be used to recontruct another image
 @param input2
 Grasycale image that will be reconstructed from another image
 @param se
 Structurant element
 @return Output grayscale image
 */
cv::Mat caib::morphInfrec(const cv::Mat &input1, const cv::Mat &input2, const cv::Mat &se) {
    cv::Mat output = input1;
    
    while ( true ) {
        cv::Mat temp = output;
        output = caib::morphCdil(temp, input2, se);
        
        if ( caib::isEqual(output, temp) )
            break;
    }
    
    return output;
}

/**
 Apply the mathematical morphology opening by reconstrution operation to a grayscale image
 @param input
 Input grayscale image
 @param eroSe
 Structurant element of the erosion
 @param infrecSe
 Structurant element of the inferior reconstruction
 @return Ouput grayscale image
 */
cv::Mat caib::morphOpenrec(const cv::Mat &input, const cv::Mat &eroSe, const cv::Mat &infrecSe) {
    return caib::morphInfrec(caib::morphEro(input, eroSe), input, infrecSe);
}

/**
 Apply the mathematical morphology closening by reconstrution operation to a grayscale image
 @param input
 Input grayscale image
 @param eroSe
 Structurant element of the dilatation
 @param infrecSe
 Structurant element of the superior reconstruction
 @return Ouput grayscale image
 */
cv::Mat caib::morphCloserec(const cv::Mat &input, const cv::Mat &dilSe, const cv::Mat &suprecSe) {
    return caib::morphSuprec(caib::morphDil(input, dilSe), input, suprecSe);
}

/**
 Apply the mathematical morphology close holes operation to a grayscale image
 @param input
 Input grayscale image
 @param se
 Structurant image
 @return Ouput grayscale image
 */
cv::Mat caib::morphClohole(const cv::Mat &input, const cv::Mat &se) {
    cv::Mat edgeFrame = cv::Mat(input.size(), input.type());
    
    edgeFrame = 255;
    edgeFrame.col(0).setTo(0);
    edgeFrame.col(edgeFrame.cols-1).setTo(0);
    edgeFrame.row(0).setTo(0);
    edgeFrame.row(edgeFrame.rows-1).setTo(0);
    
    return caib::morphSuprec(edgeFrame, input);
}

/**
 Find the regional maxima of an input grayscale image
 @param input
 Input grayscaleImage
 @return se
 Structurant element
 */
cv::Mat caib::morphRegmax(const cv::Mat &input, const cv::Mat &se) {
    cv::Mat output = input - caib::morphInfrec((input - 1), input, se);
    
    for (int x=0; x<output.cols; x++) {
        for (int y=0; y<output.rows; y++) {
            if ( output.at<uchar>(cv::Point(x,y)) > 0 || 255 <= input.at<uchar>(cv::Point(x,y)) )
                output.at<uchar>(cv::Point(x,y)) = 255;
            else
                output.at<uchar>(cv::Point(x,y)) = 0;
        }
    }
    
    return output;
}

/**
 Find the regional minima of an input grayscale image
 @param input
 Input grayscaleImage
 @return se
 Structurant element
 */
cv::Mat caib::morphRegmin(const cv::Mat &input, const cv::Mat &se) {
    cv::Mat output = caib::morphSuprec((input + 1), input, se) - input;
    
    for (int x=0; x<output.cols; x++) {
        for (int y=0; y<output.rows; y++) {
            if ( output.at<uchar>(cv::Point(x,y)) > 0 || input.at<uchar>(cv::Point(x,y)) <= 0 )
                output.at<uchar>(cv::Point(x,y)) = 255;
            else
                output.at<uchar>(cv::Point(x,y)) = 0;
        }
    }
    
    return output;
}

/**
 Apply the morphological area openning on a input grayscale image
 @param input
 Input grayscale image
 @param a
 Maximum admited area of connected componets
 @se
 Structurant element
 */
cv::Mat caib::morphAreaopen(const cv::Mat &input, const double &a) {
    cv::Mat output = input.clone();
    
    if (caib::isBinary(input)) {
        
        output = caib::morphRegmax(input);

        std::vector<cv::Vec4i> hierarchy;
        std::vector<std::vector<cv::Point> > contours;
        
        cv::findContours(output.clone(), contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
        cv::Mat temp = cv::Mat::zeros(input.size(), cv::DataType<uchar>::type);
        
        for (int c=0; !(c<0) && (c<contours.size()); c=hierarchy[c][0]) {
            
            double area = cv::contourArea(contours[c]);
            
            if ( area < a ) {
                cv::drawContours(temp, contours, c, cv::Scalar(255), CV_FILLED, 8, hierarchy);
            }
            
        }
        
        output -= temp;
        
        
    } else {
        
        std::vector<uchar> values;
        
        for (int x=0; x<input.cols; x++) {
            for (int y=0; y<input.rows; y++) {
                uchar value = input.data[x + (y * input.cols)];
                if ( std::find(values.begin(), values.end(), value) == values.end() )
                    values.push_back(value);
            }
        }
        
        std::sort(values.begin(), values.end());
        
        for (long i = values.size()-1; i > 0; i--) {
            std::vector<cv::Vec4i> hierarchy;
            std::vector<std::vector<cv::Point> > contours;
            cv::Mat layer;
            
            
            cv::threshold(output, layer, values[i]-1, 255,  cv::THRESH_BINARY);
            cv::findContours(layer.clone(), contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
            
            for (int c=0; !(c<0) && (c<contours.size()); c=hierarchy[c][0]) {
                
                double area = cv::contourArea(contours[c]);
                
                if ( area < a ) {
                    cv::floodFill(output, contours[c][0], values[i-1], 0, 1, 0, 8);
                }
                
            }
            
        }
        
    }
    
    
    return output;
}


/**
 Apply the morphological area closing on a input grayscale image
 @param input
 Input grayscale image
 @param a
 Maximum admited area of connected componets
 @se
 Structurant element
 */
cv::Mat caib::morphAreaclose(const cv::Mat &input, const double &a) {
    return caib::morphNeg(caib::morphAreaopen(caib::morphNeg(input), a));
}

/**
 Apply the mathematical morphology open top hat operation to a grayscale image
 @param input
 Input grayscale image
 @param se
 Structurant image
 @return Ouput grayscale image
 */
cv::Mat caib::morphOpenth(const cv::Mat &input, const cv::Mat &se) {
    return input - caib::morphOpen(input, se);
}

/**
 Close all the holes inside an binary image
 @param input
 Input grayscale image
 @param se
 Structurant image
 @return Ouput grayscale image
 */
cv::Mat caib::morphCloseth(const cv::Mat &input, const cv::Mat &se) {
    return caib::morphClose(input, se) - input;
}

/**
 Apply the mathematical morphology open top hat by reconstruction operation to a grayscale image
 @param input
 Input grayscale image
 @param eroSe
 Structurant element of the erosion
 @param infrecSe
 Structurant element of the inferior reconstruction
 @return Output grayscale image
 */
cv::Mat caib::morphOpenrecth(const cv::Mat &input, const cv::Mat &eroSe, const cv::Mat &infrecSe) {
    return input - caib::morphOpenrec(input, eroSe, infrecSe);
}

/**
 Apply the mathematical morphology close top hat by reconstruction operation to a grayscale image
 @param input
 Input grayscale image
 @param dilSe
 Structurant element of the dilatation
 @param suprecSe
 Structurant element of the superior reconstruction
 @return Output grayscale image
 */
cv::Mat caib::morphCloserecth(const cv::Mat &input, const cv::Mat &dilSe, const cv::Mat &suprecSe) {
    return caib::morphCloserec(input, dilSe, suprecSe) - input;
}

/**
 Remove all the connected components touching a binary image frame
 @param input
 Input binary image
 @param se
 Structurant element
 @return Output binary image
 */
cv::Mat caib::morphEdgeoff(const cv::Mat &input, const cv::Mat &se) {
    CV_Assert(caib::isBinary(input));
    
    cv::Mat edgeFrame = cv::Mat::zeros(input.size(), input.type());
    
    edgeFrame.col(0).setTo(255);
    edgeFrame.col(edgeFrame.cols-1).setTo(255);
    edgeFrame.row(0).setTo(255);
    edgeFrame.row(edgeFrame.rows-1).setTo(255);
    
    return input - caib::morphInfrec( edgeFrame, input, se);
}

/**
 Apply the mathematical morphology gradient to a grayscale image
 @param input
 Input grayscale image
 @param dilSe
 Structurant element of the dilatation
 @param eroSe
 Structurant element of the erosion
 @return Output grayscale image
 */
cv::Mat caib::morphGradm(const cv::Mat &input, const cv::Mat &dilSe, const cv::Mat &eroSe) {
    return caib::morphDil(input, dilSe) - caib::morphEro(input, eroSe);
}

/**
 Label a binary image according to a structurant element
 @param input
 Input binary image
 @param se
 Structurant element
 @return labeled image
 */
cv::Mat caib::morphLabel(const cv::Mat &input, const cv::Mat &se) {
    CV_Assert(caib::isBinary(input));
    
    cv::Mat temp, output = cv::Mat::zeros(input.size(), cv::DataType<int>::type);
    
    int label = 1;
    
    for (int x=0; x<input.cols; x++) {
        for (int y=0; y<input.rows; y++) {
            if ( input.ptr()[x + (y * input.cols)] == 255 && output.ptr<int>()[x + (y *output.cols)] == 0) {
                temp = cv::Mat::zeros(input.size(), input.type());
                temp.ptr()[x + (y * temp.cols)] = 255;
                temp = caib::morphInfrec(temp, input, se);
                temp.convertTo(temp, cv::DataType<int>::type);
                
                output += (temp/255)*(label++);
            }
            
        }
    }
    
    return output;
}
