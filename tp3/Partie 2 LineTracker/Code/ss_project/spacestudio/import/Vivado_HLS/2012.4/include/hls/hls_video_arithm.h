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
// opencv/modules/core/src/arithm.cpp

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
 * HLS Video Arithmetic Functions Header File
 */


#ifndef ___HLS__VIDEO_ARITHM__
#define ___HLS__VIDEO_ARITHM__

#include "hls_video_core.h"

enum { 
    HLS_CMP_EQ=0,
    HLS_CMP_GT=1,
    HLS_CMP_GE=2,
    HLS_CMP_LT=3,
    HLS_CMP_LE=4,
    HLS_CMP_NE=5
};

#define HLS_REDUCE_SUM  1
#define HLS_REDUCE_AVG  2
#define HLS_REDUCE_MAX  3
#define HLS_REDUCE_MIN  4

namespace hls {

typedef  int  UP_T;

class kernel_min {
public: 
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = __MIN(src1, src2);
    }
};

class kernel_max {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = __MAX(src1, src2);
    }
};

class kernel_absdiff {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS INLINE
	dst = sr_cast<DST_T, UP_T>(__ABS((UP_T)src1-(UP_T)src2));
    }
};

class kernel_and {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = (sr_cast<DST_T, SRC1_T>(src1)) & (sr_cast<DST_T, SRC2_T>(src2));
    }
};

class kernel_not {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src, SRC2_T& val, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = ~(sr_cast<DST_T, SRC1_T>(src));
    }
};

class kernel_set {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src, SRC2_T& val, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = sr_cast<DST_T, SRC2_T>(val); 
    }
};

class kernel_scale {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src, SRC2_T& val, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = sr_cast<DST_T, P_T>(p0*src+p1); 
    }
};

class kernel_add {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = sr_cast<DST_T, UP_T>((UP_T)src1 + (UP_T)src2);
    }
};

class kernel_sub {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = sr_cast<DST_T, UP_T>((UP_T)src1 - (UP_T)src2);
    }
};

class kernel_subR {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = sr_cast<DST_T, UP_T>((UP_T)src2 - (UP_T)src1);
    }
};

class kernel_mul {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = sr_cast<DST_T, P_T>(p0*src1*src2); 
    }
};

class kernel_addWeighted {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = sr_cast<DST_T, P_T>(p0*src1+p1*src2+p2); 
    }
};

class kernel_cmp {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        switch (p0)
        {
            case HLS_CMP_EQ:
            dst = (src1==src2 ? 255 : 0);
            break;
            case HLS_CMP_GT:
            dst = (src1 >src2 ? 255 : 0);
            break;
            case HLS_CMP_GE:
            dst = (src1>=src2 ? 255 : 0);
            break;
            case HLS_CMP_LT:
            dst = (src1 <src2 ? 255 : 0);
            break;
            case HLS_CMP_LE:
            dst = (src1<=src2 ? 255 : 0);
            break;
            dst = (src1!=src2 ? 255 : 0);
            break;
            default:
            break;
        }
    }
};

class kernel_sum
{
        public:
        template<typename  SRC1_T,typename DST_T>					
        void apply(SRC1_T &src1,DST_T &dst,int& para1,int& para2)
	{
#pragma HLS INLINE
		dst+=src1;
	}
};

class kernel_mean
{
        public:
        template<typename  SRC1_T,typename DST_T>					
       inline void apply(SRC1_T &src1,DST_T &dst,int& para1,int& para2)
	{
#pragma HLS INLINE
		dst+=src1;
                para1++;
   
	}
};

// Min, Max, And(no mask), AbsDiff, Mul, AddWeighted, Cmp
template<typename KERNEL, int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T, typename P_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC1_T>& src1,
            Mat<ROWS, COLS, SRC2_T>& src2,
            Mat<ROWS, COLS, DST_T>& dst,
            P_T   p0=0,
            P_T   p1=0,
            P_T   p2=0)
{
    KERNEL opr;
    int rows = dst.rows;
    int cols = dst.cols;
    Scalar<HLS_MAT_CN(SRC1_T), HLS_TNAME(SRC1_T)> s1;
    Scalar<HLS_MAT_CN(SRC2_T), HLS_TNAME(SRC2_T)> s2;
    Scalar<HLS_MAT_CN(DST_T),  HLS_TNAME(DST_T) > d;
    for (int i = 0; i < rows; i++) {
#pragma HLS loop_tripcount max=1080
    for (int j = 0; j < cols; j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS pipeline II=1
            src1 >> s1;
            src2 >> s2;
            for (int k = 0; k < HLS_MAT_CN(DST_T); k++) {
                opr.template apply(s1.val[k], s2.val[k], d.val[k], p0, p1, p2);
            }
            dst << d;
        }
    }
}

// And (mask)
template<typename KERNEL, int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC1_T>& src1,
            Mat<ROWS, COLS, SRC2_T>& src2,
            Mat<ROWS, COLS, DST_T>& dst,
            Mat<ROWS, COLS, HLS_8UC1>& mask,
            Mat<ROWS, COLS, DST_T>& dst_ref)
{
    KERNEL opr;
    int rows = dst.rows;
    int cols = dst.cols;
    Scalar<HLS_MAT_CN(SRC1_T), HLS_TNAME(SRC1_T)> s1;
    Scalar<HLS_MAT_CN(SRC2_T), HLS_TNAME(SRC2_T)> s2;
    Scalar<HLS_MAT_CN(DST_T),  HLS_TNAME(DST_T)>  d_ref, d;
    Scalar<1, unsigned char> m;
    for (int i = 0; i < rows; i++) {
#pragma HLS loop_tripcount max=1080
        for (int j = 0; j < cols; j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS pipeline II=1
            src1 >> s1;
            src2 >> s2;
            mask >> m;
            dst_ref >> d_ref;
            for (int k = 0; k < HLS_MAT_CN(DST_T); k++) {
                if (m.val[0] == 0) {
                    d.val[k] = d_ref.val[k];
                } else {
                    opr.template apply(s1.val[k], s2.val[k], d.val[k]);
                }
            }
            dst << d;
        }
    }
}

// Set, Not, MinS, MaxS, Scale, CmpS
// AddS, SubS, SubRS (no-mask)
template<typename KERNEL, int ROWS, int COLS, int SRC_T, typename _T, int DST_T, typename P_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC_T>& src,
            Scalar<HLS_MAT_CN(SRC_T), _T> scl,
            Mat<ROWS, COLS, DST_T>& dst,
            P_T p0=0,
            P_T p1=0,
            P_T p2=0)
{
    KERNEL opr;
    int rows = dst.rows;
    int cols = dst.cols;
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(SRC_T), _T> _s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    _s = scl; 
    for (int i = 0; i < rows; i++) {
#pragma HLS loop_tripcount max=1080
        for (int j = 0; j < cols; j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS pipeline II=1
            src >> s;
            for (int k = 0; k < HLS_MAT_CN(DST_T); k++) {
                opr.template apply(s.val[k], _s.val[k], d.val[k], p0, p1, p2);
            }
            dst << d;
        }
    }
}

// AddS, SubS, SubRS (mask)
template<typename KERNEL, int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC_T>& src,
            Scalar<HLS_MAT_CN(SRC_T), _T> scl,
            Mat<ROWS, COLS, DST_T>& dst,
            Mat<ROWS, COLS, HLS_8UC1>& mask,
            Mat<ROWS, COLS, DST_T>& dst_ref)
{
    KERNEL opr;
    int rows = dst.rows;
    int cols = dst.cols;
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s; 
    Scalar<HLS_MAT_CN(SRC_T), _T> _s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d_ref, d;
    Scalar<1, unsigned char> m;
    _s = scl; 
    for (int i = 0; i < rows; i++) {
#pragma HLS loop_tripcount max=1080
        for (int j = 0; j < cols; j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS pipeline II=1
            src >> s;
            mask >> m;
            dst_ref >> d_ref;
            for (int k = 0; k < HLS_MAT_CN(DST_T); k++) {
                if (m.val[0] == 0) {
                    d.val[k] = d_ref.val[k];
                } else {
                    opr.template apply(s.val[k], _s.val[k], d.val[k]);
                }
            }
            dst << d;
        }
    }
}

// Mean Avg (no mask)
template<typename DST_T,typename SUM_T,int ROWS,int COLS,int SRC1_T>
hls::Scalar<HLS_MAT_CN(SRC1_T),DST_T> mean_all(
            hls::Mat<ROWS,COLS,SRC1_T> &src1)
{
        kernel_mean alg_opr;
        int channels=HLS_MAT_CN(SRC1_T);
        hls::Scalar<HLS_MAT_CN(SRC1_T),DST_T> result;
        hls::Scalar<HLS_MAT_CN(SRC1_T),SUM_T> internal;
        hls::Scalar<HLS_MAT_CN(SRC1_T),int> count;
        for(int i=0; i<HLS_MAT_CN(SRC1_T);i++)
        {
                result.val[i]=0;
                internal.val[i]=0;
                count.val[i]=0;
                }
	Scalar<HLS_MAT_CN(SRC1_T),HLS_TNAME(SRC1_T)> _s;
	int para1=0;
        int rows=src1.rows;
        int cols=src1.cols;
	for(int i= 0;i< rows;i++) {
#pragma HLS loop_tripcount max=1080
			for (int j= 0;j< cols;j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS PIPELINE
			#pragma HLS DEPENDENCE array intra false
			#pragma HLS DEPENDENCE array inter false
				src1>>_s;
				for(int k= 0;k< channels;k++)
				{
					alg_opr.template apply(_s.val[k],internal.val[k],count.val[k],para1);

				}
			}
		}
        for(int k=0;k<channels&&count.val[k]!=0;k++)
        result.val[k]=(double (internal.val[k]))/count.val[k];
        return result;
}
// Mean Avg (mask)
// SUM_T is the internal sum type, if DST_t is unchar ,SUM_T should be unsigned int
template<typename DST_T,typename SUM_T,int ROWS,int COLS,int SRC1_T>
hls::Scalar<HLS_MAT_CN(SRC1_T),DST_T> mean_all(
            hls::Mat<ROWS, COLS,SRC1_T> &src1,
	    hls::Mat<ROWS, COLS,HLS_8UC1> &mask)
{
        int rows=src1.rows;
        int cols=src1.cols;
        kernel_mean alg_opr;
        int channels=HLS_MAT_CN(SRC1_T);
        hls::Scalar<HLS_MAT_CN(SRC1_T),DST_T>   result;
        hls::Scalar<HLS_MAT_CN(SRC1_T),SUM_T>   internal;
        hls::Scalar<HLS_MAT_CN(SRC1_T),int>     count;
        for(int i=0; i<HLS_MAT_CN(SRC1_T);i++)
        {
                result.val[i]=0;
                internal.val[i]=0;
                count.val[i]=0;
                }
	Scalar<HLS_MAT_CN(SRC1_T),HLS_TNAME(SRC1_T)> _s;
	int para1=0;
	for(int i= 0;i< rows;i++)
		{
			for (int j= 0;j<cols;j++)
			{
#pragma HLS PIPELINE
				 unsigned char   mask_val=0;
			   	 mask.data_stream[0]>>mask_val;
				 src1>>_s;
					for(int k= 0;k< channels;k++)
					{
						if (mask_val!=0)
						{
						alg_opr.template apply(_s.val[k],internal.val[k],count.val[k],para1);
						}
					}
			}
		}
        for(int k=0;k<channels&&count.val[k]!=0;k++)
        result.val[k]=(double (internal.val[k]))/count.val[k];
        return result;
}
// Reduce 
template<typename INTERSUM_T,int ROWS,int COLS,int SRC_T,int DST_ROWS,int DST_COLS, int DST_T>
void  reduce_opr(
                    hls::Mat<ROWS, COLS,SRC_T> &src,
                    hls::Mat<DST_ROWS,DST_COLS,DST_T> &dst,
                    int     dim, int op=HLS_REDUCE_SUM)
{
            hls::LineBuffer<1,DST_COLS,INTERSUM_T> dst_buffer[HLS_MAT_CN(DST_T)];

#pragma HLS ARRAY_PARTITION variable= dst_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable= dst_buffer complete dim=2
            int rows=src.rows;
            int cols=src.cols;
            hls::Scalar<HLS_MAT_CN(SRC_T),INTERSUM_T> internal;
            for(int i=0; i<HLS_MAT_CN(SRC_T);i++)
            {
                internal.val[i]=0;
            }
            int channels=HLS_MAT_CN(SRC_T);
            
	    for(int i= 0;i< rows;i++) {
#pragma HLS loop_tripcount max=1080
			for (int j= 0;j< cols;j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS PIPELINE
				for(int k= 0;k< channels;k++)
                                {
                                        HLS_TNAME(SRC_T) src_val;
					src.data_stream[k]>>src_val;
                                        if(dim==0)
                                        {
                                                if(i!=0)
                                                {
                                                        internal.val[k]=(INTERSUM_T)dst_buffer[k].val[i][0];
                                                }
                                                else
                                                {
                                                        if(op==HLS_REDUCE_SUM||op==HLS_REDUCE_AVG)
                                                                internal.val[k]=0;
                                                        else if(op==HLS_REDUCE_MAX||op==HLS_REDUCE_MIN)
                                                                internal.val[k]=src_val;

                                                }
                                        }
                                        switch (op)
                                        {
                                         case HLS_REDUCE_SUM:
                                         internal.val[k]+=src_val;
                                         break;
                                         case HLS_REDUCE_AVG:
                                         internal.val[k]+=src_val;
                                         break;
                                         case HLS_REDUCE_MAX:
                                         internal.val[k]=(internal.val[k]>(INTERSUM_T)src_val?internal.val[k]:src_val); 
                                         break;
                                         case HLS_REDUCE_MIN:
                                         internal.val[k]=(internal.val[k]<(INTERSUM_T)src_val?internal.val[k]:src_val); 
                                         break;
					}
                                        if(dim==0)
                                        {
                                                if(i=rows-1)
                                                {
                                                        if(op=HLS_REDUCE_AVG)
                                                          dst.data_stream[k] << ((HLS_TNAME(DST_T))internal.val[k])/rows;
                                                         else    
                                                         dst.data_stream[k]<< (HLS_TNAME(DST_T))internal.val[k];
                                                }
                                                else
                                                {
                                                        dst_buffer[k].val[i][0]=internal.val[k];
                                                }
                                        }
				
			  	 }
	                      
		             if(dim==1&&j==cols-1)
	                        {
	                       	     for(int k=0;k<channels;k++)
					{
			 			if(op==HLS_REDUCE_AVG)
						  {
                                                          dst.data_stream[k] << ((HLS_TNAME(DST_T))internal.val[k])/cols;
                                                  }
                                                else        
			  			   {
                                                           dst.data_stream[k]<< (HLS_TNAME(DST_T))internal.val[k];
                                                    }
			                        internal.val[k]=0;
					}			        
				}
                     }
 				
		}
} 

// Sum
template<typename OPR_KERNEL,typename DST_T,int SRC1_T,int ROWS,int COLS>
hls::Scalar<HLS_MAT_CN(SRC1_T),DST_T>  arithm_pro(
                    Mat<ROWS, COLS,SRC1_T> &src1,
                    int para1=0,int para2=0)
{
        int rows=src1.rows;
        int cols=src1.cols;
        OPR_KERNEL alg_opr;
        Scalar<HLS_MAT_CN(SRC1_T),DST_T> result;
        for(int i=0; i<HLS_MAT_CN(SRC1_T);i++)
        {
                result.val[i]=0;
        }
        int channels=HLS_MAT_CN(SRC1_T);
  	Scalar<HLS_MAT_CN(SRC1_T),HLS_TNAME(SRC1_T)> _s;
	for(int i= 0;i< rows;i++) {
#pragma HLS loop_tripcount max=1080
			for (int j= 0;j< cols;j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS PIPELINE		
				src1>>_s;
				for(int k= 0;k< channels;k++)
				{
					alg_opr.template apply(_s.val[k],result.val[k],para1,para2);
				}
			}
		}
        return result;
};


/* video arithmetic functions. */
template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void Min(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC1_T, DST_T) && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_TYPES_EQ(SRC2_T, DST_T) && HLS_ARE_SIZES_EQ(src2, dst));
    arithm_pro<kernel_min>(src1, src2, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void Max(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC1_T, DST_T) && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_TYPES_EQ(SRC2_T, DST_T) && HLS_ARE_SIZES_EQ(src2, dst));
    arithm_pro<kernel_max>(src1, src2, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void MinS(
        Mat<ROWS, COLS, SRC_T>& src,
        _T value,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    Scalar<HLS_MAT_CN(SRC_T), _T> scl;
    for (int k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        scl.val[k] = (value);
    }
    arithm_pro<kernel_min>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void MaxS(

        Mat<ROWS, COLS, SRC_T>& src,
        _T value,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    Scalar<HLS_MAT_CN(SRC_T), _T> scl;
    for (int k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        scl.val[k] = (value);
    }
    arithm_pro<kernel_max>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void AddS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    arithm_pro<kernel_add>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void AddS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst,
        Mat<ROWS, COLS, HLS_8UC1>& mask,
        Mat<ROWS, COLS, DST_T>& dst_ref)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    arithm_pro<kernel_add>(src, scl, dst, mask, dst_ref);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void SubS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    arithm_pro<kernel_sub>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void SubS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst,
        Mat<ROWS, COLS, HLS_8UC1>& mask,
        Mat<ROWS, COLS, DST_T>& dst_ref)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    arithm_pro<kernel_sub>(src, scl, dst, mask, dst_ref);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void SubRS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    arithm_pro<kernel_subR>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void SubRS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst,
        Mat<ROWS, COLS, HLS_8UC1>& mask,
        Mat<ROWS, COLS, DST_T>& dst_ref)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    arithm_pro<kernel_subR>(src, scl, dst, mask, dst_ref);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T, typename P_T>
void Mul(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst,
        P_T scale=1)
{
    assert(HLS_ARE_TYPES_EQ(SRC1_T, DST_T) && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_TYPES_EQ(SRC2_T, DST_T) && HLS_ARE_SIZES_EQ(src2, dst));
    arithm_pro<kernel_mul>(src1, src2, dst, scale, (P_T)0, (P_T)0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T, typename P_T>
void AddWeighted(
        Mat<ROWS, COLS, SRC1_T>& src1,
        P_T alpha,
        Mat<ROWS, COLS, SRC2_T>& src2,
        P_T beta,
        P_T gamma,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC1_T, DST_T) && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_TYPES_EQ(SRC2_T, DST_T) && HLS_ARE_SIZES_EQ(src2, dst));
    arithm_pro<kernel_addWeighted>(src1, src2, dst, alpha, beta, gamma);
}

template<int ROWS, int COLS, int SRC_T, int DST_T, typename P_T>
void Scale(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst,
        P_T scale=1.0,
        P_T shift=0.0)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> scl;
    arithm_pro<kernel_scale>(src, scl, dst, scale, shift, (P_T)0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void Set(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    arithm_pro<kernel_set>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Zero(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst)
{
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s0;
    for (int k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        s0.val[k] = 0;
    }
    arithm_pro<kernel_set>(src, s0, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void AbsDiff(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC1_T, DST_T) && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_TYPES_EQ(SRC2_T, DST_T) && HLS_ARE_SIZES_EQ(src2, dst));
    arithm_pro<kernel_absdiff>(src1, src2, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Not(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> scl;
    arithm_pro<kernel_not>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void Cmp(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst,
        int cmp_op)
{
    assert(HLS_ARE_TYPES_EQ(SRC1_T, DST_T) && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_TYPES_EQ(SRC2_T, DST_T) && HLS_ARE_SIZES_EQ(src2, dst));
    arithm_pro<kernel_cmp>(src1, src2, dst, cmp_op, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename P_T, int DST_T>
void CmpS(
        Mat<ROWS, COLS, SRC_T>& src,
        P_T value,
        Mat<ROWS, COLS, DST_T>& dst,
        int cmp_op)
{
    assert(HLS_ARE_TYPES_EQ(SRC_T, DST_T) && HLS_ARE_SIZES_EQ(src, dst));
    Scalar<HLS_MAT_CN(SRC_T), P_T> scl;
    for (int k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        scl.val[k] = (value);
    }
    arithm_pro<kernel_cmp>(src, scl, dst, cmp_op, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void And(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>&  dst)
{
    assert(HLS_ARE_TYPES_EQ(SRC1_T, DST_T) && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_TYPES_EQ(SRC2_T, DST_T) && HLS_ARE_SIZES_EQ(src2, dst));
    arithm_pro<kernel_and>(src1, src2, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void And(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst,
        Mat<ROWS, COLS, HLS_8UC1>& mask,
        Mat<ROWS, COLS, DST_T>& dst_ref)
{
    assert(HLS_ARE_TYPES_EQ(SRC1_T, DST_T) && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_TYPES_EQ(SRC2_T, DST_T) && HLS_ARE_SIZES_EQ(src2, dst));
    arithm_pro<kernel_sub>(src1, src2, dst, mask, dst_ref);
}

template<typename DST_T,int ROWS,int COLS,int SRC1_T>
hls::Scalar<HLS_MAT_CN(SRC1_T),DST_T> Sum(
            hls::Mat<ROWS, COLS,SRC1_T> &src1)
{
        return arithm_pro<kernel_sum,DST_T>(src1);
}
 
template<typename INTERSUM_T,int ROWS,int COLS,int SRC_T,int DST_ROWS,int DST_COLS, int DST_T>
void  Reduce(
         hls::Mat<ROWS, COLS,SRC_T> &src,
         hls::Mat<DST_ROWS,DST_COLS,DST_T> &dst,
         int     dim,int op=HLS_REDUCE_SUM)
{
    reduce_opr<INTERSUM_T> (src,dst,dim,op);
}

template<typename DST_T,typename SUM_T,int ROWS, int COLS, int SRC1_T>
hls::Scalar<HLS_MAT_CN(SRC1_T),DST_T> Avg(
            hls::Mat<ROWS, COLS,SRC1_T> &src1)
{
    return  mean_all<DST_T,SUM_T>(src1);
}

template<typename DST_T,typename SUM_T,int ROWS, int COLS, int SRC1_T>
hls::Scalar<HLS_MAT_CN(SRC1_T),DST_T> Avg(
      	hls::Mat<ROWS, COLS,SRC1_T> &src1,
    	hls::Mat<ROWS, COLS,HLS_8UC1> &mask)
{
    return  mean_all<DST_T,SUM_T>(src1,mask);
}

template<typename DST_T,typename SUM_T,int ROWS, int COLS, int SRC1_T>
DST_T Mean(
        hls::Mat<ROWS,COLS,SRC1_T> &src1,
        hls::Mat<ROWS,COLS,HLS_8UC1> &mask)
{
    return  (mean_all<DST_T,SUM_T>(src1,mask)).val[0];
}

template<typename DST_T,typename SUM_T,int ROWS, int COLS, int SRC1_T>
DST_T Mean(
        hls::Mat<ROWS, COLS,SRC1_T> &src1)
{
    return  (mean_all<DST_T,SUM_T>(src1)).val[0];
}

} // namespace hls

#endif 

