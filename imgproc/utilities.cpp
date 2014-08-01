//
//  utilities.cpp
//  imgproc
//
//  Created by Romulo Bourget on 2/17/14.
//  Copyright (c) 2014 CAIB. All rights reserved.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com

#include "utilities.h"

/**
 Check if wheter an image is binary or not
 @param input
 The input image to be checked
 @return TRUE if the image is binary, FALSE if not
 */
bool caib::isBinary(const cv::Mat &input) {
    
    int val;
    if ( !caib::isGray(input) )
        return false;
    
    for ( int x = 0; x < input.cols; x++) {
        for ( int y = 0; y < input.rows; y++) {
            val = input.at<uchar>(cv::Point(x,y));
            
            if ( val != 0  && val != 255 ) {
                return false;
            }
        }
    }
    
    return true;
}

/**
 Check if an image is in grayscale or not
 @param input
 The input image to be checked
 @return TRUE if the image is in grayscale, FALSE if not
 */
bool caib::isGray(const cv::Mat &input) {
    if (input.type() != cv::DataType<uchar>::type)
        return false;
    
    return true;
}

/**
 Check if two images have their pixels intensities equal
 @param input1
 One of the images to be compared
 @param input2
 The other image to be compared
 @return TRUE if the images are equal, FALSE if not
 */
bool caib::isEqual(const cv::Mat &input1, const cv::Mat &input2) {
    if ( input1.cols != input2.cols || input1.rows != input2.rows )
        return false;
    
    cv::Mat absdiff = cv::Mat(input1.size(), input1.type());
    cv::absdiff(input1, input2, absdiff);
    
    if ( cv::countNonZero(absdiff) != 0 )
        return false;
    
    return true;
}

/**
 Check if one point is inside the range of a image
 @param input
 Input image
 @param pt
 Point
 @return TRUE if the point is inside the image range, FALSE if not
 */
bool caib::isInside(const cv::Mat &input, const cv::Point &pt) {
    if (pt.x >= 0 && pt.x < input.cols && pt.y >= 0 && pt.y < input.rows)
        return true;
    
    return false;
}

/**
 Draw the contour labels given by Labeled image
 @param input
 Input Image in which the contours will be drawn
 @param labels
 Labels that will be drawn
 @return Output image with contours of the labels drawn
 */
cv::Mat caib::drawLabelsContours(const cv::Mat &input, const cv::Mat& labels, const cv::Scalar &color) {
    
    CV_Assert (labels.type() == cv::DataType<int>::type && labels.cols == input.cols && labels.rows == input.rows && input.type() == cv::DataType<cv::Vec3b>::type );
    
    cv::Mat output = input.clone();
    
    const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
    
	int sz = labels.cols*labels.rows;
    
    std::vector<bool> istaken(sz, false);
    
	int mainindex(0);
	for( int j = 0; j < labels.rows; j++ )
	{
		for( int k = 0; k < labels.cols; k++ )
		{
			int np(0);
			for( int i = 0; i < 8; i++ )
			{
				int x = k + dx8[i];
				int y = j + dy8[i];
                
				if( (x >= 0 && x < labels.cols) && (y >= 0 && y < labels.rows) )
				{
					int index = y*labels.cols + x;
                    
					if( false == istaken[index] )//comment this to obtain internal contours
					{
						if( labels.ptr<int>()[mainindex] != labels.ptr<int>()[index] ) np++;
					}
				}
			}
			if( np > 1 )//change to 2 or 3 for thinner lines
			{
                for (int c = 0; c < output.channels(); c++) {
                    output.ptr()[output.channels()*mainindex + c] = color[c];
                }
				istaken[mainindex] = true;
			}
			mainindex++;
		}
	}
    
    return output;
  }

/**
 Paint every label passed on an labeled image
 @param labels
 Labeled image
 @return Output image with every label painted with a different color
 */
cv::Mat caib::paintLabels(const cv::Mat &labels) {
    
    CV_Assert (labels.type() == cv::DataType<int>::type);
	
    cv::Mat output = cv::Mat(labels.size(), cv::DataType<cv::Vec3b>::type);
    
    std::vector<cv::Vec3b> colorTab;
    std::vector<int> label;
    cv::RNG rng(12345);
    cv::Vec3b color;
    
	for(int x=0; x<labels.cols; x++)
	{
		for(int y=0; y<labels.rows; y++)
		{
            std::vector<int>::iterator it = std::find(label.begin(), label.end(), labels.ptr<int>()[x + ( y * labels.cols)]);
            
            if (it != label.end()) {
                color = colorTab[it - label.begin()];
            } else {
                int b = rng.uniform(0, 255);
                int g = rng.uniform(0, 255);
                int r = rng.uniform(0, 255);
                color = cv::Vec3b((uchar)b, (uchar)g, (uchar)r);
                
                colorTab.push_back(color);
                label.push_back( labels.ptr<int>()[x + (y * labels.cols)] );
            }
            
            output.ptr()[( (x + (y * output.cols)) * output.channels() ) + 0 ] = color[0];
            output.ptr()[( (x + (y * output.cols)) * output.channels() ) + 1 ] = color[1];
            output.ptr()[( (x + (y * output.cols)) * output.channels() ) + 2 ] = color[2];
		}
	}
    
	return output;
}

/**
 Paint every label passed on an labeled image
 @param labels
 Labeled image
 @return Output image with every label painted with a different color
 */
cv::Mat caib::paintLabel(const cv::Mat &labels, const int &label, const cv::Scalar &color) {
    
    CV_Assert(labels.type() == cv::DataType<int>::type);
	
    cv::Mat output = cv::Mat::zeros(labels.size(), cv::DataType<cv::Vec3b>::type);
    
	for(int x=0; x<labels.cols; x++)
	{
		for(int y=0; y<labels.rows; y++)
		{
            if (labels.ptr<int>()[x + (y * labels.cols)] == label) {
                output.ptr()[( (x + (y * output.cols)) * output.channels() ) + 0 ] = color[0];
                output.ptr()[( (x + (y * output.cols)) * output.channels() ) + 1 ] = color[1];
                output.ptr()[( (x + (y * output.cols)) * output.channels() ) + 2 ] = color[2];
            }
		}
	}
    
	return output;
}

/**
 Calculates the histogram for a Grayscale image
 @param input
 Input grayscale image
 @param nbins
 Number of bins
 @param uniform
 Should histogram be uniform?
 @param accumulate
 Should histogram be accumulative?
 */
caib::histogram caib::calcHistogram(const cv::Mat &input, int nbins, bool uniform, bool accumulate) {
    
    CV_Assert( isGray(input) );
    
    caib::histogram hist;
    int histSize = nbins;
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    
    cv::Mat cv_hist;
    cv::calcHist( &input, 1, 0, cv::Mat(), cv_hist, 1, &histSize, &histRange, uniform, accumulate );
    
    for (int i = 0; i < histSize; i++) {
        hist.hist_bin.push_back( cv_hist.at<float>(i) );
        hist.v_bin.push_back( cvRound( i * (range[1]/histSize) ) );
    }
    
    return hist;
}


/**
 Calculates the histogram for each label
 @param input
 Input image from which data will be used
 @param labels
 Labeled image
 @param channels
 Channels that will be used to compute the histograms
 @param histSize
 Number of bins for each channel
 @param ranges
 Range of each channel
 @param uniform
 Uniform distribution
 @param accumulate
 Accumulate bin values
 @return Histogram of each label passed in labels
 */

/*
std::vector<cv::MatND> caib::calcLabelHist(const cv::Mat &input, cv::Mat labels, const std::vector<int> &channels, const std::vector<int> &histSize, const float *ranges[], bool uniform, bool accumulate) {
    
    CV_Assert (labels.type() == cv::DataType<int>::type && labels.cols == input.cols && labels.rows == input.rows );
    
    std::vector<int> id_labels;
    for (int i = 0; i < labels.cols * labels.rows; i++) {
        if (std::find(id_labels.begin(), id_labels.end(), labels.ptr<int>()[i]) == id_labels.end() ) {
            id_labels.push_back(labels.ptr<int>()[i]);
        }
    }
    
    std::vector<cv::MatND> hists = std::vector<cv::MatND>(id_labels.size(), cv::MatND());
    
    for (long i=0 ; i < id_labels.size(); i++) {
        hists[i] = caib::calcLabelHist(input, labels, id_labels[i], channels, histSize, ranges, uniform, accumulate);
    }
    
    return hists;
    
}*/

/**
 Calculates the histogram for a specific label
 @param input
 Input image from which data will be used
 @param labels
 Labeled image
 @param label
 Label for which the histogram shall be computed
 @param channels
 Channels that will be used to compute the histograms
 @param histSize
 Number of bins for each channel
 @param ranges
 Range of each channel
 @param uniform
 Uniform distribution
 @param accumulate
 Accumulate bin values
 @return Histogram of each label passed in labels
 */

/*
cv::MatND caib::calcLabelHist(const cv::Mat &input, cv::Mat labels, const int &label, const std::vector<int> &channels, const std::vector<int> &histSize, const float *ranges[], bool uniform, bool accumulate) {
    
    CV_Assert (labels.type() == cv::DataType<int>::type && labels.cols == input.cols && labels.rows == input.rows );
    
    cv::MatND hist;
    cv::Mat image = input.clone();
    cv::Mat mask = cv::Mat::zeros(labels.size(), cv::DataType<uchar>::type);
    
    for (int i=0; i < labels.cols*labels.rows; i++) {
        if (labels.ptr<int>()[i] == label) {
            mask.ptr()[i] = 255;
        }
    }
    
    cv::calcHist(&image, 1, channels.data(), mask, hist, (int) channels.size(), histSize.data(), ranges, uniform, accumulate);
    
    
    return hist;
} */

/**
 Retrieves the labels adjancy list
 @param labels
 Labeled image
 @param id_labels
 Id of each label in the labeled image (pass initially empty)
 @return The adjancy list of each label in the image
 */
/*
std::vector<std::vector<int> > getLabelsAdjList(const cv::Mat &labels, std::vector<int> &id_labels) {
    
    const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
    
    std::vector<std::vector<int> > labelsAdjList;
    
	for( int j = 0; j < labels.rows; j++ )
	{
		for( int k = 0; k < labels.cols; k++ )
		{
            
            int mainindex = k + (j * labels.cols);
            std::vector<int>::iterator it = std::find(id_labels.begin(), id_labels.end(), labels.ptr<int>()[mainindex]);
            if ( it == id_labels.end()) {
                id_labels.push_back(labels.ptr<int>()[mainindex]);
                labelsAdjList.push_back(std::vector<int>());
            }
            
			for( int i = 0; i < 8; i++ )
			{
				int x = k + dx8[i];
				int y = j + dy8[i];
                
				if( (x >= 0 && x < labels.cols) && (y >= 0 && y < labels.rows) )
				{
					int index = y*labels.cols + x;
                    
                    if( labels.ptr<int>()[mainindex] != labels.ptr<int>()[index] ) {
                        if(it != id_labels.end()){
                            if(std::find(labelsAdjList[id_labels.begin() - it].begin(), labelsAdjList[id_labels.begin() - it].end(), labels.ptr<int>()[mainindex]) ==labelsAdjList[id_labels.begin() - it].end()) {
                                labelsAdjList[id_labels.begin() - it].push_back(labels.ptr<int>()[mainindex]);
                            }
                        } else {
                            labelsAdjList[id_labels.size() -1].push_back(labels.ptr<int>()[mainindex]);
                        }
                    }
				}
			}
           
            
		}
	}
    
    return labelsAdjList;
    
}*/




