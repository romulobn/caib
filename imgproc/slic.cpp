// slic.cpp: implementation of the SLIC algorithm.
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

#include "slic.h"

cv::Mat detectEdges(const std::vector<cv::Mat> &vec, const cv::Size &size) {
    
    cv::Mat edges = cv::Mat(size, cv::DataType<double>::type);
    
    for( int j = 1; j < size.height-1; j++ ) {
        for( int k = 1; k < size.width-1; k++ ) {
            int i = j*size.width+k;
            
            double dx = 0;
            double dy = 0;
            for (int c=0; c<vec.size(); c++) {
                dx += (vec[c].ptr<double>()[i-1] - vec[c].ptr<double>()[i+1]) * (vec[c].ptr<double>()[i-1] - vec[c].ptr<double>()[i+1]);
                dy += (vec[c].ptr<double>()[i-size.width] - vec[c].ptr<double>()[i+size.width]) * (vec[c].ptr<double>()[i-size.width] - vec[c].ptr<double>()[i+size.width]);
            }
            
            edges.ptr<double>()[i] = (dx + dy);
        }
    }
    
    return edges;
}

void perturbSeeds(std::vector<std::vector<double> > &kseeds, std::vector<double> &kseedsx, std::vector<double> &kseedsy, const int &nseeds, const cv::Mat &edges, const std::vector<cv::Mat> &vec, const cv::Size &size) {
    const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
	
	int numseeds = nseeds;
    
	for( int n = 0; n < numseeds; n++ ) {
		int ox = kseedsx[n];//original x
		int oy = kseedsy[n];//original y
		int oind = oy*size.width + ox;
        
		int storeind = oind;
		for( int i = 0; i < 8; i++ ) {
			int nx = ox+dx8[i];//new x
			int ny = oy+dy8[i];//new y
            
			if( nx >= 0 && nx < size.width && ny >= 0 && ny < size.height) {
				int nind = ny*size.width + nx;
				if( edges.ptr<double>()[nind] < edges.ptr<double>()[storeind]) {
					storeind = nind;
				}
			}
		}
		if(storeind != oind) {
			kseedsx[n] = storeind%size.width;
			kseedsy[n] = storeind/size.width;
            for (int c = 0; c < kseeds.size(); c++) {
                kseeds[c][n] = vec[c].ptr<double>()[storeind];
            }
		}
	}
}

int getSeeds_ForGivenK(std::vector<std::vector<double> > &kseeds, std::vector<double> &kseedsx, std::vector<double> &kseedsy, const int &K, const cv::Mat &edges, const std::vector<cv::Mat> vec,const cv::Size &size) {
    
    int sz = size.width*size.height;
	double step = sqrt(double(sz)/double(K));
	int xoff = step/2;
	int yoff = step/2;
	
	int n = 0;
    int r = 0;
	for( int y = 0; y < size.height; y++ ) {
		int Y = y*step + yoff;
		if( Y > size.height-1 ) break;
        
		for( int x = 0; x < size.width; x++ ) {
			//int X = x*step + xoff;//square grid
			int X = x*step + (xoff<<(r&0x1));//hex grid
			if(X > size.width-1) break;
            
			int i = Y*size.width + X;
 
            for (int c = 0; c < kseeds.size(); c++) {
                kseeds[c].push_back(vec[c].ptr<double>()[i]);
            }
			kseedsx.push_back(X);
			kseedsy.push_back(Y);
			n++;
		}
		r++;
	}
    
    perturbSeeds(kseeds, kseedsx, kseedsy, n, edges, vec, size);
    
    return n;
}

cv::Mat performSuperpixelSegmentation_VariableSandM(std::vector<std::vector<double> > &kseeds, std::vector<double> &kseedsx, std::vector<double> &kseedsy, const std::vector<cv::Mat> &vec, const cv::Size &size, const int &STEP, const int &NUMITR = 10) {
    
    int sz = size.width*size.height;
	const int numk = (int) kseedsx.size();
	int numitr = 0;
    
	int offset = (STEP < 10) ? STEP*1.5 : STEP;
    
    cv::Mat klabels = cv::Mat(size, cv::DataType<int>::type);
    klabels = -1;
    
    std::vector<std::vector<double> > sigmac(0);
    for (int c = 0; c < vec.size(); c++) {
        sigmac.push_back(std::vector<double>(numk, 0));
    }
    std::vector<double> sigmax(numk, 0);
    std::vector<double> sigmay(numk, 0);
    std::vector<int> clustersize(numk, 0);
    std::vector<double> inv(numk, 0);//to store 1/clustersize[k] values
    std::vector<double> distxy(sz, DBL_MAX);
    std::vector<double> distc(sz, DBL_MAX);
    std::vector<double> distvec(sz, DBL_MAX);
    std::vector<double> maxc(numk, 10*10);//THIS IS THE VARIABLE VALUE OF M, just start with 10
    std::vector<double> maxxy(numk, STEP*STEP);//THIS IS THE VARIABLE VALUE OF M, just start with 10
    
	double invxywt = 1.0/(STEP*STEP);//NOTE: this is different from how usual SLIC/LKM works
    
	while( numitr < NUMITR )
	{
		//------
		//cumerr = 0;
		numitr++;
		//------
        
		distvec.assign(sz, DBL_MAX);
		for( int n = 0; n < numk; n++ )
		{
			int y1 = std::max(double(0), kseedsy[n]-offset);
			int y2 = std::min(double(size.height),	kseedsy[n]+offset);
			int x1 = std::max(double(0), kseedsx[n]-offset);
			int x2 = std::min(double(size.width),	kseedsx[n]+offset);
            
			for( int y = y1; y < y2; y++ )
			{
				for( int x = x1; x < x2; x++ )
				{
					int i = y*size.width + x;
					if( !(y < size.height && x < size.width && y >= 0 && x >= 0) ) {
                        throw cv::Exception();
                    }
                    
					distc[i] =	0;
                    for (int c = 0; c < vec.size(); c++) {
                        distc[i] += (vec[c].ptr<double>()[i] - kseeds[c][n]) * (vec[c].ptr<double>()[i] - kseeds[c][n]);
                    }
                    
					distxy[i] =		(x - kseedsx[n])*(x - kseedsx[n]) + (y - kseedsy[n])*(y - kseedsy[n]);
                    
					double dist = distc[i]/maxc[n] + distxy[i]*invxywt;//only varying m, prettier superpixels
					
					if( dist < distvec[i] )
					{
						distvec[i] = dist;
						klabels.ptr<int>()[i]  = n;
					}
				}
			}
		}
		//-----------------------------------------------------------------
		// Assign the max color distance for a cluster
		//-----------------------------------------------------------------
		if(0 == numitr)
		{
			maxc.assign(numk,1);
			maxxy.assign(numk,1);
		}
		{for( int i = 0; i < sz; i++ )
		{
			if(maxc[klabels.ptr<int>()[i]] < distc[i]) maxc[klabels.ptr<int>()[i]] = distc[i];
			if(maxxy[klabels.ptr<int>()[i]] < distxy[i]) maxxy[klabels.ptr<int>()[i]] = distxy[i];
		}}
		//-----------------------------------------------------------------
		// Recalculate the centroid and store in the seed values
		//-----------------------------------------------------------------
        
        for (int c = 0; c < sigmac.size(); c++) {
            sigmac[c].assign(numk, 0);
        }
		sigmax.assign(numk, 0);
		sigmay.assign(numk, 0);
		clustersize.assign(numk, 0);
        
		for( int j = 0; j < sz; j++ )
		{
			if(!(klabels.ptr<int>()[j] >= 0))
                throw cv::Exception();
            
            for (int c = 0; c < sigmac.size(); c++) {
                sigmac[c][klabels.ptr<int>()[j]] += vec[c].ptr<double>()[j];
            }
			sigmax[klabels.ptr<int>()[j]] += (j%size.width);
			sigmay[klabels.ptr<int>()[j]] += (j/size.width);
            
			clustersize[klabels.ptr<int>()[j]]++;
		}
        
		{for( int k = 0; k < numk; k++ )
		{
			if( clustersize[k] <= 0 ) clustersize[k] = 1;
			inv[k] = 1.0/double(clustersize[k]);//computing inverse now to multiply, than divide later
		}}
		
		{for( int k = 0; k < numk; k++ )
		{
            for (int c = 0; c < kseeds.size(); c++) {
                kseeds[c][k] = sigmac[c][k] * inv[k];
            }
			kseedsx[k] = sigmax[k]*inv[k];
			kseedsy[k] = sigmay[k]*inv[k];
		}}
	}
    
    return klabels;
}

cv::Mat enforceLabelConnectivity(const cv::Mat &labels, int &numlabels, const cv::Size &size, const int &K) {
    
    const int dx4[4] = {-1,  0,  1,  0};
	const int dy4[4] = { 0, -1,  0,  1};
    
	const int sz = size.width*size.height;
	const int SUPSZ = sz/K;
	
    cv::Mat nlabels = cv::Mat(size, cv::DataType<int>::type);
    nlabels = -1;
    
	int label = 0;
	int* xvec = new int[sz];
	int* yvec = new int[sz];
	int oindex = 0;
	int adjlabel = 0;//adjacent label
    
	for( int j = 0; j < size.height; j++ )
	{
		for( int k = 0; k < size.width; k++ )
		{
			if( 0 > nlabels.ptr<int>()[oindex] )
			{
				nlabels.ptr<int>()[oindex] = label;
				//--------------------
				// Start a new segment
				//--------------------
				xvec[0] = k;
				yvec[0] = j;
				//-------------------------------------------------------
				// Quickly find an adjacent label for use later if needed
				//-------------------------------------------------------
				{for( int n = 0; n < 4; n++ )
				{
					int x = xvec[0] + dx4[n];
					int y = yvec[0] + dy4[n];
					if( (x >= 0 && x < size.width) && (y >= 0 && y < size.height) )
					{
						int nindex = y * size.width + x;
						if(nlabels.ptr<int>()[nindex] >= 0) adjlabel = nlabels.ptr<int>()[nindex];
					}
				}}
                
				int count = 1;
				for( int c = 0; c < count; c++ )
				{
					for( int n = 0; n < 4; n++ )
					{
						int x = xvec[c] + dx4[n];
						int y = yvec[c] + dy4[n];
                        
						if( (x >= 0 && x < size.width) && (y >= 0 && y < size.height) )
						{
							int nindex = y * size.width + x;
                            
							if( 0 > nlabels.ptr<int>()[nindex] && labels.ptr<int>()[oindex] == labels.ptr<int>()[nindex] )
							{
								xvec[count] = x;
								yvec[count] = y;
								nlabels.ptr<int>()[nindex] = label;
								count++;
							}
						}
                        
					}
				}
				//-------------------------------------------------------
				// If segment size is less then a limit, assign an
				// adjacent label found before, and decrement label count.
				//-------------------------------------------------------
				if(count <= SUPSZ >> 2)
				{
					for( int c = 0; c < count; c++ )
					{
						int ind = yvec[c] * size.width + xvec[c];
						nlabels.ptr<int>()[ind] = adjlabel;
					}
					label--;
				}
				label++;
			}
			oindex++;
		}
	}
	numlabels = label;
    
	if(xvec) delete [] xvec;
	if(yvec) delete [] yvec;
    
    return nlabels;
}

cv::Mat caib::slico(const cv::Mat &input, const int& K) {
    
    std::vector<cv::Mat> vec;
    cv::Size size;
    
    int STEP, numlabels;
    cv::Mat edgemag, labels;
    cv::vector<double> kseedsx, kseedsy;
    cv::vector<cv::vector<double> > kseeds;
    
    size = input.size();
    cv::split(input, vec);
    
    for(int c=0; c<input.channels(); c++) {
        vec[c].convertTo(vec[c], cv::DataType<double>::type);
    }
    
    STEP = sqrt( double(size.width * size.height) / double(K) ) + 2.0; //adding a small value in case the STEP size is too small.
   
    kseedsx = std::vector<double>();
    kseedsy = std::vector<double>();
    kseeds = std::vector<std::vector<double> >(input.channels(), std::vector<double>() );
    
    edgemag = detectEdges(vec, size); //get edges to find better perturb seeds
	numlabels = getSeeds_ForGivenK(kseeds, kseedsx, kseedsy, K, edgemag, vec, size); //get seeds for k
	labels = performSuperpixelSegmentation_VariableSandM(kseeds, kseedsx, kseedsy, vec, size, STEP); //perform the super pixel segmentation
    labels = enforceLabelConnectivity(labels, numlabels, size, K); //enforce the connectivity of these superpixels
    
    return labels;
}