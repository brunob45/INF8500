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
// opencv/modules/imgproc/src/filter.cpp

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
 * HLS Video Image Base Functions Header File
 */


#ifndef _BASE_FUN_
#define _BASE_FUN_

#include "hls_video_types.h"
#include "hls_video_mem.h"

#include "hls_video_core.h"

#include "hls_stream.h"
#include "ap_fixed.h"
#include "ap_int.h"
#include <limits.h>
#include <float.h>

namespace hls {
typedef ap_uint<4> BORDER_T;
typedef ap_fixed<23,14,AP_RND> 	SUM_T;


enum {MORPH_RECT,MORPH_CROSS,MORPH_ELLIPSE};
enum {BORDER_REPLICATE,BORDER_REFLECT,BORDER_REFLECT_101, BORDER_WRAP,BORDER_CONSTANT};
//enum {IPL_DEPTH_8U,IPL_DEPTH_8S,IPL_DEPTH_16U,IPL_DEPTH_16S,IPL_DEPTH_32S,IPL_DEPTH_32F,IPL_DEPTH_64F};

//According the data type to figure out the max and minimum value which is  used normalize the pixel value.
static void GetMaxDepth(int depth_type,double &maxval, double &minval)
{
	switch (depth_type)
	{
		case HLS_8U:
			maxval=UCHAR_MAX;
			minval=0;
		break;
		case HLS_8S:
			maxval=SCHAR_MAX;
			minval=SCHAR_MIN;
		break;
		case HLS_16U:
			maxval=USHRT_MAX;
			minval=0;
		break;
		case HLS_16S:
			maxval=SHRT_MAX;
			minval=SHRT_MIN;
		break;
		case HLS_32S:
			maxval=INT_MAX;
			minval=INT_MIN;
		break;
		case HLS_32F:
			maxval=FLT_MAX;
			minval=FLT_MIN;
		break;
		case HLS_64F:
			maxval=LDBL_MAX;
			minval=LDBL_MIN;
		break;
		default :
			maxval=UCHAR_MAX;
			minval=0;
		break;

	}
}
//Parameters: 0---based coordinate of the extrapolated pixel along one of the axes, likely <0 or >= len
//len - length of the array along the corresponding axis
 /*
 Various border types, image boundaries are denoted with '|'

 * BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
 * BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb
 * BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba
 * BORDER_WRAP:          cdefgh|abcdefgh|abcdefg//no support
 * BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'
 */
static int borderInterpolate( int p, int len, int borderType )
{
#pragma HLS INLINE

	if( (unsigned)p < (unsigned)len )
		;
	else if( borderType == BORDER_REPLICATE )
		p = p < 0 ? 0 : len - 1;
	else if( borderType == BORDER_REFLECT)
	{
		if( len == 1 )
		return 0;
		if(p<0)
		{
			p=-p-1;
		}
		if( p >= len )
		{
		p=2*len-p-1;
		}
	}
	else if(borderType == BORDER_REFLECT_101 )
	{
		if( len == 1 )
			return 0;
		if(p<0)
		{

			p=-p;
		}
		if( p >= len )
		{

			p=2*len-2;
		}
	}

	else if( borderType == BORDER_CONSTANT )
		p = -1;
	return p;
}
// Make sure the anchor in the right location
template <typename anchor_T,typename kernel_T>
inline void normalizeAnchor(hls::Point_<anchor_T> &anchor,
							hls::Size_<kernel_T>  kernel_size)
{
#pragma HLS INLINE

	if(anchor.x<0||anchor.y<0){
		if(anchor.x== -1&&anchor.y== -1)
		{
			anchor.x=kernel_size.width/2;
			anchor.y=kernel_size.height/2;
		}
		else{
			anchor.x=0;
			anchor.y=0;
		}
	}
	if(anchor.x>=kernel_size.width){
		anchor.x=kernel_size.width-1;
	}
	if(anchor.y>=kernel_size.height){
		anchor.y=kernel_size.height-1;
	}

}
//fiter2D kernel
}
#endif
