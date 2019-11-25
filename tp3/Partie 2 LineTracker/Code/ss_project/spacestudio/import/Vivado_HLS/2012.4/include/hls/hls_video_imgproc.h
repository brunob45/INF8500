/*****************************************************************************
 *
 *     Author: Xilinx, Inc.
 *
 *     This text contains proprietary, confidential information of
 *     Xilinx, Inc. , is distributed by under license from Xilinx,
 *     Inc., and may be used, copied and/or disclosed only pursuant to
 *     the terms of a valid license agreement with Xilinx, Inc.
 *
 *     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
 *     AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
 *     SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
 *     OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
 *     APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
 *     THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
 *     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
 *     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
 *     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
 *     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
 *     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
 *     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE.
 *
 *     Xilinx products are not intended for use in life support appliances,
 *     devices, or systems. Use in such applications is expressly prohibited.
 *
 *     (c) Copyright 2012 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

// This code is partially derived from OpenCV:
// opencv/modules/imgproc/src/morph.cpp

/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef ___HLS__VIDEO__
#warning You are using OpenCV Video library 1.0 which is in Beta. This library is still under development and may have issues in functionality and/or implementation.
#endif

/*
 * HLS Video Image Processing Functions Header File
 */


#ifndef _HLS_IMG_FILTER_H
#define _HLS_IMG_FILTER_H
#include "hls_video_imgbase.h"
#include "hls_video_types.h"
#include "hls_video_mem.h"
#include "hls_video_core.h"
#include "hls_stream.h"
#include "ap_fixed.h"
#include "ap_int.h"
#include <math.h>

#define HLS_SHAPE_RECT 		hls::MORPH_RECT
#define HLS_SHAPE_CROSS 	hls::MORPH_CROSS
#define HLS_SHAPE_ELLIPSE	hls::MORPH_ELLIPSE
namespace hls {
#define  ERODE 		1
#define  DILATE		2
#define  FILTER2D	3

class erode_kernel
{
        public:
template<int SRC_T,int DST_T,typename FILTER_T, int F_HEIGHT, int F_WIDTH>					
void apply(hls::Window<F_HEIGHT,F_WIDTH,FILTER_T>	&_kernel_filter,
		hls::Window<F_HEIGHT,F_WIDTH,HLS_TNAME(SRC_T)>	&_kernel_pixel,
		int  						maxval,
		int  						minval,
		HLS_TNAME(DST_T)				&out)
{
#pragma HLS INLINE
	out=maxval;
	for( int m=0;m< F_HEIGHT;m++)
	{
		for( int n=0;n<F_WIDTH;n++)
		{
			HLS_TNAME(DST_T) temp;
			if(_kernel_filter.val[F_HEIGHT-m-1][F_WIDTH-1-n]>0)
			{
				temp=_kernel_pixel.val[F_HEIGHT-m-1][(F_WIDTH-1-n)];
				if (temp<out)
				out=temp;
			}
		}
	}
}
};

class dilate_kernel
{
public:
template<int SRC_T,int DST_T,typename FILTER_T, int F_HEIGHT, int F_WIDTH>					
void apply(hls::Window<F_HEIGHT,F_WIDTH,FILTER_T>	&_kernel_filter,
		hls::Window<F_HEIGHT,F_WIDTH,HLS_TNAME(SRC_T)>	&_kernel_pixel,
		int 						maxval,
		int 						minval,
		HLS_TNAME(DST_T)				&out)
{
#pragma HLS INLINE
		out=minval;
		for( int m=0;m< F_HEIGHT;m++)
		{
			for( int n=0;n<F_WIDTH;n++)
			{
				HLS_TNAME(DST_T) temp;
				if(_kernel_filter.val[F_HEIGHT-m-1][F_WIDTH-1-n]>0)
				{
					temp=_kernel_pixel.val[F_HEIGHT-m-1][(F_WIDTH-1-n)];
					if (temp>out)
					out=temp;
            			}
			}
		}
	}
};

class filter2d_kernel
{
public:
template<int SRC_T,int DST_T,typename FILTER_T, int F_HEIGHT, int F_WIDTH>					
void apply(hls::Window<F_HEIGHT,F_WIDTH,FILTER_T>	&_kernel_filter,
		hls::Window<F_HEIGHT,F_WIDTH,HLS_TNAME(SRC_T)>	&_kernel_pixel,
		int						maxval,
		int 						minval,
		HLS_TNAME(DST_T)				&out)
{
#pragma HLS INLINE
	SUM_T sum=0;
	for( int m=0;m< F_HEIGHT;m++)
	{
		for( int n=0;n<F_WIDTH;n++)
		{
			sum+=_kernel_filter.val[m][n]*_kernel_pixel.val[F_HEIGHT-m-1][(F_WIDTH-1-n)];
		}
	}
	if(sum>0)
	{
		if(sum>maxval)
			sum=maxval;
		out=(HLS_TNAME(DST_T))sum;
	}
	else
		out=minval;
	}
};


template <typename SRC_T,typename SIZE_T,typename POINT_T,int HEIGHT,int WIDTH>
void getStructuringElement(
                        int 				shape,
			hls::Size_<SIZE_T> 	        ksize,
			hls::Point_<POINT_T>            anchor,
			hls::Window<HEIGHT,WIDTH,SRC_T> &result)
{
	int i, j;
	int r = 0, c = 0;
	ap_fixed<31,11,AP_RND> inv_r2 = 0;

if( ksize.width==1&&ksize.height == 1 )
	shape = MORPH_RECT;

	if( shape == MORPH_ELLIPSE )
	{
		r = ksize.height/2;
		c = ksize.width/2;
		if(r!=0)
		{
			inv_r2 =(ap_fixed<31,11,AP_RND>) 1/(r*r) ;
		}
		else
			inv_r2=0;
	}
	for( i = 0; i < ksize.height; i++ )
	{
		int j1 = 0, j2 = 0;
		if( shape == MORPH_RECT || (shape ==MORPH_CROSS && i == anchor.y) )
			j2 = ksize.width;
		else if( shape == MORPH_CROSS )
			j1 = anchor.x, j2 = j1 + 1;
		else
		{
			int dy = i - r;
			if( std::abs(dy) <= r )
			{
				ap_fixed<12,12,AP_RND> dxx = (c*sqrt(((r-dy)*(r+dy))*inv_r2));///////saturate_cast<int>  nearly rand
				int dx=dxx;
				j1 = (c - dx)> 0?( c - dx):0;
				j2 =  (c + dx + 1)< ksize.width?(c + dx + 1):ksize.width;
			}
		}

		for( j = 0; j < j1; j++ )
			result.val[i][j] = 0;
		for( ; j < j2; j++ )
			result.val[i][j] = 1;
		for( ; j < ksize.width; j++ )
			result.val[i][j] = 0;
	}
}

template<typename P_T,typename W_T,int W_HEIGHT, int W_WIDTH,typename BUF_T,int BUF_HEIGHT,int BUF_WIDTH >
void fill_pixelkernel(
                    LineBuffer<BUF_HEIGHT,BUF_WIDTH,BUF_T>  &col_buf,
                    Window<W_HEIGHT,W_WIDTH,W_T>            &kernel,
                    Point_<P_T>                             curp,
                   int borderType, int ref,int imgheight,int constval=0)
{
#pragma HLS INLINE
        for(int buf_row= 0;buf_row<W_HEIGHT;buf_row++)
	{
        int  locy;
	locy=ref-borderInterpolate(curp.y,imgheight,borderType);
	curp.y--;
	kernel.val[buf_row][0]=col_buf.val[locy][0];
        }
}
template<typename OPR_KERNEL,int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>//,int KN_HEIGHT,int KN_WIDTH
void filter_opr(
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>		&_src,
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>		&_dst,
		hls::Window<K_HEIGHT,K_WIDTH,KN_T>		&kernel,
		hls::Point_<POINT_T>				_anchor,
		int 						_boardtype,int rows,int cols)
                
{
	            OPR_KERNEL                                                      fk_opr;
src_kernel_win:     hls::Window<K_HEIGHT,K_WIDTH,HLS_TNAME(SRC_T)>		    src_kernel_win[HLS_MAT_CN(SRC_T)];
k_buffer:           hls::LineBuffer<K_HEIGHT,IMG_WIDTH,HLS_TNAME(SRC_T)>	    k_buf[HLS_MAT_CN(SRC_T)];
right_board:        hls::LineBuffer<1,K_WIDTH,HLS_TNAME(SRC_T)>		            right_board_buf[HLS_MAT_CN(SRC_T)];
kbuffer_col:        hls::LineBuffer<K_HEIGHT,1,HLS_TNAME(SRC_T)>		    col_buf[HLS_MAT_CN(SRC_T)];// the right board pixel

#pragma HLS ARRAY_PARTITION variable=col_buf complete dim=0
#pragma HLS ARRAY_PARTITION variable=kernel complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_win complete dim=0
#pragma HLS ARRAY_PARTITION variable=k_buf complete dim=1
//#pragma HLS ARRAY_PARTITION variable=k_buf complete dim=2
#pragma HLS ARRAY_PARTITION variable=right_board_buf complete dim=0
       int boardtype=_boardtype;

        // used for buffer image pixel
        hls::Point_<POINT_T> anchor;
        anchor.x=_anchor.x;
        anchor.y=_anchor.y;
	hls::Size_<POINT_T> kernel_size;
	kernel_size.width=K_WIDTH;
	kernel_size.height=K_HEIGHT;
	normalizeAnchor(anchor,kernel_size);
	
	int imgheight=rows;
	int imgwidth=cols;
        
	int preloadnum=K_HEIGHT;
	int heightloop= imgheight+K_HEIGHT-1+preloadnum;
	int widthloop = imgwidth+K_WIDTH-1;//one pixel overlap, so it should minus one
	hls::Point_<POINT_T> ImagLoc;
	///------------
	int cury=0;
	double maxval,minval;
	int depthtype;
	depthtype=_src.depth();

	GetMaxDepth(depthtype,maxval,minval);
	int channels=HLS_MAT_CN(SRC_T);
	for(int i= 0;i< heightloop;i++)
	{
#pragma HLS LOOP_TRIPCOUNT min=20 max=2000
		for (int j= 0;j< widthloop;j++)
		{
#pragma HLS LOOP_TRIPCOUNT min=20 max=2000
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE
        channelloop:	for(int k= 0;k< channels;k++)
			{
//fill data x,y are the coordinate in the image, it could be negative. For example (-1,-1) represents the interpolation pixel.
                                ImagLoc.x=j-anchor.x;
                                ImagLoc.y=i-preloadnum-anchor.y;//
                                POINT_T x=borderInterpolate(ImagLoc.x,imgwidth,boardtype);

                                for(int row= 0;row<K_HEIGHT;row++)
                                for(int col= K_WIDTH-1;col>=1;col--)
                                src_kernel_win[k].val[row][col]=src_kernel_win[k].val[row][col-1];//column left shift

                                if((i>=K_HEIGHT&&ImagLoc.y<K_HEIGHT-1)||(ImagLoc.y>=imgheight-1&&ImagLoc.y<heightloop))//top extend pixel bottom keep the buffer 0 with the data imgheight-1 content.w
                                {
                                for(int buf_row= 0;buf_row<K_HEIGHT;buf_row++)
                                {
                                col_buf[k].val[buf_row][0]=k_buf[k].val[buf_row][x];        
                                }
                                int ref=K_HEIGHT-1;
                                if(ImagLoc.y>=imgheight-1)
                                        ref=imgheight-1;
                                fill_pixelkernel(col_buf[k],src_kernel_win[k],ImagLoc,boardtype,ref,imgheight);
                                }
                                else if((i<K_HEIGHT)||(ImagLoc.y>=K_HEIGHT-1&&ImagLoc.y<imgheight-1))//Advance load and body process
                                {
                                if(ImagLoc.x>=0&&ImagLoc.x<imgwidth)
                                {
                                        HLS_TNAME(SRC_T) Toppixel=k_buf[k].val[K_HEIGHT-1][ImagLoc.x];
                                        src_kernel_win[k].val[K_HEIGHT-1][0]=Toppixel;
                                        if (ImagLoc.x>=imgwidth-K_WIDTH&& ImagLoc.x<imgwidth)
                                        {
                                                right_board_buf[k].val[0][ImagLoc.x-(imgwidth-K_WIDTH)]=Toppixel;
                                        }
                                        for(int buf_row= K_HEIGHT-1;buf_row>= 1;buf_row--)
                                        {
                                                HLS_TNAME(SRC_T) temp=k_buf[k].val[buf_row-1][ImagLoc.x];
                                                src_kernel_win[k].val[buf_row-1][0]=temp;
                                                k_buf[k].val[buf_row][ImagLoc.x]=temp;
                                                col_buf[k].val[buf_row][0]=temp;

                                                
                                        }

                                        HLS_TNAME(SRC_T) temp=0;
                                                _src.data_stream[k]>>temp;
                                                k_buf[k].val[0][ImagLoc.x]=temp;
                                }
                                else if(ImagLoc.x<0)
                                {
                                        for(int buf_row= 0;buf_row<K_HEIGHT;buf_row++)
                                        {
                                                src_kernel_win[k].val[buf_row][0]=k_buf[k].val[buf_row][x];
                                        }
                                }
                                else if (ImagLoc.x>=imgwidth&&x==imgwidth-1)//>=
                                {

                                        for(int buf_row= 0;buf_row<K_HEIGHT-1;buf_row++)
                                        {
                                               src_kernel_win[k].val[buf_row][0]=col_buf[k].val[buf_row+1][0];
                                        }
                                        src_kernel_win[k].val[K_HEIGHT-1][0]=right_board_buf[k].val[0][x-(imgwidth-K_WIDTH)];
                                }
                                else if (ImagLoc.x>=imgwidth&&x<imgwidth-1)//>=
                                {

                                        for(int buf_row= 0;buf_row<K_HEIGHT-1;buf_row++)
                                        {
                                             src_kernel_win[k].val[buf_row][0]=k_buf[k].val[buf_row+1][x];
                                        }
                                        src_kernel_win[k].val[K_HEIGHT-1][0]=right_board_buf[k].val[0][x-(imgwidth-K_WIDTH)];
                                }
                                }
                                 // figure out the output image pixel value
                                if(i>=K_HEIGHT-1+preloadnum&&i<heightloop&&j>=K_WIDTH-1&& j<widthloop)
                                {
                                HLS_TNAME(DST_T) temp;
                                fk_opr.template apply<SRC_T,DST_T,KN_T,K_HEIGHT,K_WIDTH>(kernel,src_kernel_win[k],(int)maxval,(int)minval,temp);
                                _dst.data_stream[k]<<temp;
                                }
                        }
		}
	}
}
template<typename OPR_KERNEL,int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>//,int KN_HEIGHT,int KN_WIDTH
void filter_opr_flow_more(
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>		&_src,
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>		&_dst,
		hls::Window<K_HEIGHT,K_WIDTH,KN_T>		&_kernel,
		hls::Point_<POINT_T>				&anchor,
                int &bordertype,int rows,int cols
                )
{/*
		hls::Window<K_HEIGHT,K_WIDTH,KN_T> kernel;
                for(int row=0;row<K_HEIGHT;row++)
                 {       for(int col=0;col<K_WIDTH;col++)
                        { 
                                kernel.val[row][col]=_kernel.val[row][col];
                        }
                 }
*/
                filter_opr<OPR_KERNEL>(_src,_dst,_kernel,anchor,bordertype,rows,cols);
}
template<typename OPR,int ITERATIONS,int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>//,int KN_HEIGHT,int KN_WIDTH
void filter_opr_iter(
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>		&_src,
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>		&_dst,
		hls::Window<K_HEIGHT,K_WIDTH,KN_T>		&_kernel,
		hls::Point_<POINT_T>				&anchor,
              int &bordertype, int rows,int cols )
{
    
#pragma HLS INTERFACE ap_stable port=_kernel
#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols
#pragma HLS INTERFACE ap_stable port=anchor
#pragma HLS INTERFACE ap_stable port=bordertype
//#pragma HLS STREAM variable=rows depth=10
#pragma HLS INLINE off        
                Mat<IMG_HEIGHT,IMG_WIDTH,DST_T> internal[ITERATIONS-1];
#pragma HLS array partition variable=internal dim=1
#pragma HLS DATAFLOW               
		filter_opr_flow_more<OPR,SRC_T,DST_T,KN_T,POINT_T>(_src,internal[0],_kernel,anchor,bordertype,rows,cols);
                dataflow_unroll:for(int iter=0;iter<ITERATIONS-2;iter++)
                {
#pragma HLS UNROLL
		filter_opr_flow_more<OPR,SRC_T,DST_T,KN_T,POINT_T>(internal[iter],internal[iter+1],_kernel,anchor,bordertype,rows, cols);
                }
		filter_opr_flow_more<OPR,SRC_T,DST_T,KN_T,POINT_T>(internal[ITERATIONS-2],_dst,_kernel,anchor,bordertype,rows,cols);
}
template<typename OPR,int SRC_T, int DST_T,
int IMG_HEIGHT,int IMG_WIDTH>//,int KN_HEIGHT,int KN_WIDTH 3x3
void morp_opr(
        	hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	        &_src,
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>  	&_dst,
		int						boardtype)
{  		int rows=_src.rows;
                int cols=_src.cols;
		hls::Size_<int> size;
		hls::Point_<int>	anchor;
		anchor.x=1;
		anchor.y=1;
		size.width=2*1+1;
		size.height=2*1+1;
		hls::Window<2*1+1,2*1+1,unsigned char>  temp_kernel;
		getStructuringElement<unsigned char,int,int,2*1+1,2*1+1>(MORPH_RECT, size, anchor,temp_kernel);
		//iterations=1;
		filter_opr<OPR>(_src,_dst,temp_kernel,anchor,boardtype,rows,cols);
}
template<typename OPR,int Shape_type,int ITERATIONS,int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>//,int KN_HEIGHT,int KN_WIDTH
void morp_opr(
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
		hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
		hls::Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel,
		hls::Point_<POINT_T>			anchor,
		int     				boardtype)
{
                int rows=_src.rows;
                int cols=_src.cols;
	if((Shape_type==MORPH_RECT)&&ITERATIONS>1)
	{
		hls::Size_<int> size;
		anchor.x=ITERATIONS*(anchor.x);
		anchor.y=ITERATIONS*(anchor.y);
		size.width=K_WIDTH+ITERATIONS*(K_WIDTH-1);
		size.height=K_HEIGHT+ITERATIONS*(K_HEIGHT-1);
		hls::Window<K_WIDTH+ITERATIONS*(K_WIDTH-1),K_HEIGHT+ITERATIONS*(K_HEIGHT-1),KN_T>  temp_kernel;
		getStructuringElement<KN_T,int,int,K_WIDTH+ITERATIONS*(K_WIDTH-1),K_HEIGHT+ITERATIONS*(K_HEIGHT-1)>(MORPH_RECT, size, anchor,temp_kernel);
		//iterations=1;
		filter_opr<OPR>(_src,_dst,temp_kernel,anchor,BORDER_REPLICATE,rows,cols);
	}
        else if(ITERATIONS==1)
	{
		filter_opr<OPR>(_src,_dst,_kernel,anchor,BORDER_REPLICATE,rows,cols);
	}
        else if(ITERATIONS>=2)
        {
                int board=BORDER_REPLICATE;
           	filter_opr_iter<OPR,ITERATIONS,SRC_T,DST_T,KN_T,POINT_T>(_src,_dst,_kernel,anchor,board,rows,cols);
        }       
}
//void cvErode(
//		const CvArr* src,
//		CvArr* dst,
//		IplConvKernel* element=NULL,
//		int iterations=1 );---OPencv

template<int SRC_T, int DST_T, int IMG_HEIGHT,int IMG_WIDTH>//,int KN_HEIGHT,int KN_WIDTH 3x3
void Erode(
        	hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
	        hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst)
{
       	morp_opr<erode_kernel>(_src,_dst,BORDER_REPLICATE);
}
template<int Shape_type,int ITERATIONS,int SRC_T, int DST_T, typename KN_T,int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>//,int KN_HEIGHT,int KN_WIDTH
void Erode(
        	hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
	        hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        	hls::Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel)
{
       	hls::Point_<int> anchor(K_WIDTH/2,K_HEIGHT/2);
        morp_opr<erode_kernel,Shape_type,ITERATIONS>(_src,_dst,_kernel,anchor,BORDER_REPLICATE);
}
//void cvDilate(
//				const CvArr* src,
//				CvArr* dst, IplConvKernel* element=NULL,
//				int iterations=1 );----OPencv
template<int SRC_T, int DST_T, int IMG_HEIGHT,int IMG_WIDTH>//,int KN_HEIGHT,int KN_WIDTH 3x3
void Dilate(
        	hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
	        hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst)
{
	morp_opr<dilate_kernel>(_src,_dst,BORDER_REPLICATE);
}
template<int Shape_type,int ITERATIONS,int SRC_T, int DST_T, typename KN_T,int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>//,int KN_HEIGHT,int KN_WIDTH
void Dilate(
        	hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
	        hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        	hls::Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel)
{
	hls::Point_<int> anchor(K_WIDTH/2,K_HEIGHT/2);
	morp_opr<dilate_kernel,Shape_type,ITERATIONS>(_src,_dst,_kernel,anchor,BORDER_REPLICATE);
}
//void cvFilter2D( 	const CvArr* src, CvArr* dst,
//					const CvMat* kernel,
//					CvPoint anchor=cvPoint(-1,-1));------oPencv
template<int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>//,int KN_HEIGHT,int KN_WIDTH
void Filter2D(
        	hls::Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
	        hls::Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>  &_dst,
        	hls::Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel,
	        hls::Point_<POINT_T>			anchor)
{
        int rows=_src.rows;
        int cols=_src.cols;
	filter_opr<filter2d_kernel>(_src,_dst,_kernel,anchor,BORDER_REFLECT,rows,cols);
}

}
#endif
