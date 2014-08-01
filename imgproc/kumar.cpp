//
//  kumar.cpp
//  imgproc
//
//  Created by Romulo Bourget on 2/17/14.
//  Copyright (c) 2014 CAIB. All rights reserved.
//
//  Name: Rômulo Bourget Novas
//  Tel: +55 19 99735-7010
//  Email: romulo.bnovas@gmail.com
//
//  Note: This algorithm was implemented following the paper
//  S. Kumar, S.H. Ong, S. Ranganatha, T.C. Ong, F.T. Chew,
//  "A rule-based approach for robust clump splitting",
//  Pattern Recognition 39 (2006) 1088–1098

#include "kumar.h"

double k_cdThresh;  //CD threshold
double k_saThresh;  //SA threshold
double k_clThresh;  //CL threshold
double k_ccThresh;  //CC threshold
double k_caThresh;  //CA threshold
double k_crThresh;  //CR threshold

const double C1 = 1.72; //Constant C1
const double C2 = -4.70;//Constant C2

//split line
struct kLine {
    cv::Point start, end;
    double SA, CC, CL, X;
    kLine( cv::Point _start, cv::Point _end): start(_start), end(_end), SA(0), CC(0), CL(0), X(0) {}
    kLine(): start(cv::Point()), end(cv::Point()), SA(0), CC(0), CL(0), X(0) {}
};

/**
 Draw the direction line given a start point and an end point(the line will procced until has an black pixel be found
 @param start
 Start point for the direction
 @param end
 End point for the direction (the line starts here)
 */
void drawDirectionLine(const cv::Point &start, const cv::Point &end, cv::Mat& inputImg){
	
    int sx, sy, err, e2, dx, dy;
    
    dx = ( int ) sqrt( pow( ( ( double )( start.x - end.x ) ), 2 ) );
	dy = ( int ) sqrt( pow( ( ( double )( start.y - end.y ) ), 2 ) );
	err = dx - dy;
    
    if( start.x < end.x )
        sx = 1;
    else
        sx = -1;
	if( start.y < end.y )
        sy = 1;
    else
        sy = -1;
    
    cv::Point pt = end;
    
	while ( true ) {
        
		e2 = 2*err;
		
        if( e2 > -dy ) {
			err -= dy;
			pt.x += sx;
		}
		if( e2 < dx ) {
			err += dx;
			pt.y += sy;
		}
        
        if( inputImg.at<uchar>(pt) == 0 )
            break;
        
        inputImg.at<uchar>(pt) = 0;
	}

}

/**
 Draw all given lines in the input image
 @param inputImg
 Input binary image
 @param lines
 vector of split lines
 */
void drawLines(cv::Mat&	inputImg, const std::vector<kLine> &lines) {
    for(int i=0; i<lines.size(); i++) {
        cv::line( inputImg, lines[i].start, lines[i].end, cv::Scalar(0) );
	}
}

/**
 Get the mid point of a line
 @param start
 Start point of the line
 @param end
 End point of the line
 @return Mid point
 */
cv::Point getLineMidPoint( const cv::Point &start, const cv::Point &end) {
    std::vector<cv::Point> line;
    
    int sx, sy, err, e2, dx, dy;
    
    dx = ( int ) sqrt( pow( ( ( double )( start.x - end.x ) ), 2 ) );
	dy = ( int ) sqrt( pow( ( ( double )( start.y - end.y ) ), 2 ) );
	err = dx - dy;
    
    if( start.x < end.x )
        sx = 1;
    else
        sx = -1;
	if( start.y < end.y )
        sy = 1;
    else
        sy = -1;
    
    cv::Point pt = start;
    
	while ( true ) {
        line.push_back( pt );
		
        if( ( pt.x == end.x ) && ( pt.y == end.y ) )
            break;
        
		e2 = 2*err;
		
        if( e2 > -dy ) {
			err -= dy;
			pt.x += sx;
		}
		if( e2 < dx ) {
			err += dx;
			pt.y += sy;
		}
	}
    
    return line[ line.size()/2 ];
}

/**
 Measure the split line between two concavities
 @param start
 Starting concavity of the split line
 @param end
 Ending concavity of the split line
 @param contour
 Points of the contour
 @return Split line
 */
kLine measureLine(const cv::Vec4i &start, const cv::Vec4i &end, const std::vector<cv::Point> &contour) {
    kLine line = kLine(contour[start[2]], contour[end[2]]);
    
	double startcd(start[3]/256.0), endcd(end[3]/256.0);
    double dist = sqrt ( pow ( ( double ) ( line.start.x - line.end.x ), 2 ) + pow( ( double ) ( line.start.y - line.end.y ), 2 ) );
	cv::Point startmid = getLineMidPoint(contour[start[0]], contour[start[1]]);
    cv::Point endmid = getLineMidPoint(contour[end[0]], contour[end[0]]);
    
	line.SA = std::min(startcd,endcd)/( std::min(startcd,endcd) + dist );
    
    caib::vect2D v1( ( contour[start[2]].x - startmid.x ), ( contour[start[2]].y - startmid.y ) ),
    v2( ( contour[end[2]].x - endmid.x ), ( contour[end[2]].y - endmid.y ) ),
    v3( ( contour[end[2]].x - contour[start[2]].x ), ( contour[end[2]].y - contour[start[2]].y ) );
	v1.turnToUniVect();
	v2.turnToUniVect();
	v3.turnToUniVect();
    
	line.CC = M_PI - v1.angleBtwVect( v2 );
	line.CL = std::max( v1.angleBtwVect( v3 ), M_PI - v2.angleBtwVect( v3 ) );
    
    line.X = ( C1 * ( start[3]/256.0 + end[3]/256.0 ) + C2 )/( dist + C1 * ( start[3]/256.0 + end[3]/256.0 ) + C2 );
    
    return line;
}

/**
 Select the split lines for a given contours and their defects
 @param contours
 Contours of all the objects
 @param defects
 Defects of all the objects
 @param lines
 Split lines
 @return TRUE if one or more split lines are found FALSE if none are found
 */
bool selectSplitLines(std::vector<std::vector<cv::Point> > &contours, std::vector<std::vector<cv::Vec4i> > &defects, std::vector<kLine> &lines) {
    
    bool foundlines = false;
    
	for (int i=0; i<defects.size(); i++) {
		
        kLine bestLine = kLine();
        
		for (int j=0; j<defects[i].size(); j++) {
            for (int k=j+1; k<defects[i].size(); k++) {
                
				kLine line = measureLine(defects[i][j], defects[i][k], contours[i]);
				
				if ( ( ( line.X > 0.8 && line.SA > k_saThresh ) || ( line.X > 0.5 && line.CC < k_ccThresh && line.CL < k_clThresh && line.SA > k_saThresh) ) && ( line.X > bestLine.X ) ) {
					bestLine = line;
                    foundlines = true;
                   
                }
			}
		}
        
		if ( foundlines )
			lines.push_back(bestLine);
        
	}
    
    return foundlines;
}

/**
 Select the valid concavities
 @param inputImg
 Input binary image
 @param contours
 Contours of the inputImg
 @param hierarchy
 Hierarchy of contours 
 @param defects
 Defects of the contours
 @param CRs
 Concavities rate of each contour
 @param CAs
 Concavities angles of each defects
 */
void selectConcavities(const cv::Mat &inputImg, std::vector<std::vector<cv::Point> > &contours, std::vector<cv::Vec4i> &hierarchy, std::vector<std::vector<cv::Vec4i> > &defects, std::vector<double> &CRs, std::vector<std::vector<double> > &CAs) {
    
    double cd1 = 0, cd2 = 0;
    
    cv::findContours( inputImg.clone(), contours, hierarchy, CV_RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0) );

    std::vector<std::vector<int> > hull( contours.size() );
    
	defects = std::vector<std::vector<cv::Vec4i> >( contours.size() );
	CAs = std::vector<std::vector<double> >( contours.size() );
	CRs = std::vector<double>( contours.size() );

	for (int i=0; !(i<0); i = hierarchy[i][0]) {
        
		if(contours[i].size() > 3) {
			cv::convexHull( contours[i], hull[i], CV_CLOCKWISE );
            if (hull[i].size() > 2) {
            
			cv::convexityDefects( contours[i], hull[i], defects[i] );
        
                CAs[i] = std::vector<double>( defects[i].size() );
		
                for (int j=0; j<defects[i].size(); j++) {
            
                    double cd = defects[i][j][3] / 256.0;
            
                    if (cd>cd1) {
                        cd2 = cd1;
                        cd1 = cd;
                    }
			
                    if (cd > k_cdThresh) {
                
                        double a = std::sqrt( std::pow( double( contours[i][defects[i][j][2]].x - contours[i][defects[i][j][0]].x ), 2 ) + std::pow( (double) ( contours[i][defects[i][j][2]].y - contours[i][defects[i][j][0]].y ), 2 ) );
                        double b = std::sqrt( std::pow( double( contours[i][defects[i][j][2]].x - contours[i][defects[i][j][1]].x ), 2 ) + std::pow( (double) ( contours[i][defects[i][j][2]].y - contours[i][defects[i][j][1]].y ), 2 ) );
                        double c = std::sqrt( std::pow( double( contours[i][defects[i][j][0]].x - contours[i][defects[i][j][1]].x ), 2 ) + std::pow( (double) ( contours[i][defects[i][j][0]].y - contours[i][defects[i][j][1]].y ), 2 ) );
                
                        CAs[i][j] = ( acos( ( ( pow( b, 2 )  + pow( a, 2 ) - pow( c, 2) ) / ( 2 * b * a ) ) ) * ( 180.0 / M_PI ) );
                
                    } else
                        defects[i].erase( defects[i].begin() + j-- );
                }
        
                CRs[i] = cd1/cd2;
            
            } else {
                CAs[i] = std::vector<double>();
                CRs[i] = 0;
            }
            
		} else {
			CAs[i] = std::vector<double>();
			CRs[i] = 0;
		}
	}
}

/**
 Perform the Kumar clump splitting algorithm
 @param inputImg
 Input binary image
 @param cd
 Concavity depth threshold
 @param sa
 Saliency threshold
 @param cc
 Concacivity-Concavity angle threshold
 @param cl
 Concavity-Line angle threshold
 @param cr
 Concavity rate threshold
 @param ca
 Concavity angle threshold
 @return Splitted binary image
 */
cv::Mat caib::kumar(const cv::Mat &inputImg, const double &cd, const double &sa, const double &cc, const double &cl, const double &cr, const double &ca) {
    
    CV_Assert (caib::isBinary(inputImg));
    
    k_cdThresh = cd;
    k_saThresh = sa;
    k_clThresh = cl;
    k_ccThresh = cc;
    k_caThresh = ca;
    k_crThresh = cr;
    

    cv::Mat outputImg = inputImg.clone();
    
    std::vector< std::vector< cv::Point > > contours;
    std::vector< kLine > lines;
    std::vector< std::vector< cv::Vec4i > > defects;
    std::vector< cv::Vec4i > hierarchy;
    std::vector< std::vector< double > > CAs;
    std::vector< double > CRs;
    
	while ( true ) {
        lines = std::vector< kLine >();
        selectConcavities( outputImg.clone(), contours, hierarchy, defects, CRs, CAs );
        bool foundlines = selectSplitLines(contours, defects, lines);
        if(!foundlines){
            
            for (int i = 0; i < defects.size(); i++) {
                
                if (defects[i].size() == 1) {
                    if (CAs[i][0] < k_caThresh) {
                        cv::Point pt = getLineMidPoint(contours[i][defects[i][0][0]], contours[i][defects[i][0][1]]);
                        drawDirectionLine(pt, contours[i][defects[i][0][2]], outputImg);
                    }
                }
                
                else if( defects[i].size() > 1 && CRs[i] > k_crThresh) {
                    int bestIdx;
                    double bestCD = 0;
                    
                    for (int j = 0; j < defects[i].size(); j++) {
                        double cd = defects[i][j][3]/256.0;
                        if (bestCD < cd) {
                            bestIdx = j;
                            bestCD = cd;
                        }

                    }
                    
                    if (CAs[i][bestIdx] < k_caThresh) {
                        cv::Point pt = getLineMidPoint(contours[i][defects[i][bestIdx][0]], contours[i][defects[i][bestIdx][1]]);
                        drawDirectionLine(pt, contours[i][defects[i][bestIdx][2]], outputImg);
                    }
    
                    
                }
            }
            
            break;
        }
        drawLines(outputImg, lines);
	}
    
    return outputImg;
}