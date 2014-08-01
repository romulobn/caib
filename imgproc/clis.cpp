//
//  clis.cpp
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


#include "clis.h"

/**
 Choose a cluster as winner for a given pixel
 @param idata
 Data of the image
 @param point
 Location of the pixel
 @param cdata
 Data of the cluster
 @return Index of the winner in cdata vector
 */
int chooseWinner(const std::vector<cv::Mat> &idata, const cv::Point &point, const std::vector<std::vector<double> > &cdata) {
    int winner = 0;
    double mindist = DBL_MAX;
    
	for(int i=0; i<cdata.size(); i++) {
        double dist =  0;
        
        for(int c=0; c<idata.size(); c++)
			dist +=  std::pow( ( idata[c].ptr<double>()[point.x + (point.y * idata[c].cols)] - cdata[i][c] ), 2);
        
		if( dist < mindist ) {
			winner = i;
			mindist = dist;
		}
	}
    
    return winner;
}

/**
 Segment a image using the Uchyama algorithm
 @param inputImage
 Input image data
 @param alpha
 Learning rate
 @param numcluster
 Number of clusters
 @param numwin
 Win threshold
 @param numiter
 Number of interactions
 @return Labeled image with clusters
 */
cv::Mat caib::clis(const cv::Mat &inputImage, const double &alpha, const int &numcluster, const int &numwin, const int &numiter) {
    
    std::vector<int> wincount = std::vector<int>(numcluster, 0);
    std::vector<std::vector<double> > cdata (1, std::vector<double>(inputImage.channels(),0));
    
    cv::Mat labels = cv::Mat(inputImage.size(), cv::DataType<int>::type);
    
    std::vector<cv::Mat> imgChannels;
    cv::split(inputImage, imgChannels);
    
    for (int c = 0; c < imgChannels.size(); c++) {
        imgChannels[c].convertTo(imgChannels[c], cv::DataType<double>::type);
    }
    
    int nwin = (numwin < 0) ? sqrt(numcluster) * 400 : numwin;
    int niter = (numiter < 0) ? ( (2 * numcluster) - 3 ) * nwin * (numcluster + 7) : numiter;
    
    for (int i=0; i<niter; i++) {
        int rx = rand()%inputImage.cols;
        int ry = rand()%inputImage.rows;
        
        int winner = chooseWinner( imgChannels, cv::Point(rx,ry), cdata );
        
        for (int c=0; c<inputImage.channels(); c++) {
            int offstep = rx + (ry * inputImage.cols);
            cdata[winner][c] += alpha * ( imgChannels[c].ptr<double>()[offstep] - cdata[winner][c] );
        }
        
        wincount[winner]++;
        
        if (wincount[winner] == nwin && cdata.size() < numcluster) {
            cdata.push_back(cdata[winner]);
            wincount[winner] = 0;
        }
        
    }
    
    for (int x=0; x<inputImage.cols; x++) {
        for (int y=0; y<inputImage.rows; y++) {
            double mindist = DBL_MAX;
            
            for (int i=0; i<cdata.size(); i++) {
                double dist = 0;
                
                int offstep = x + (y * inputImage.cols);
                for (int c=0; c<inputImage.channels(); c++)
                    dist += std::pow( ( imgChannels[c].ptr<double>()[offstep] - cdata[i][c] ), 2);
                
                if( dist < mindist ) {
                    mindist = dist;
                    labels.at<int>(cv::Point(x,y)) = i;
                }
            }
        }
    }
    
    return labels;
}