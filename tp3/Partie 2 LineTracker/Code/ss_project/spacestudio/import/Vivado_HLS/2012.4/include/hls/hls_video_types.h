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

// This code is derived from OpenCV:
// opencv/modules/core/include/opencv2/core/types_c.h

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
 * HLS Video Types Header File
 */

#ifndef ___HLS__VIDEO_TYPES__
#define ___HLS__VIDEO_TYPES__

#include "ap_int.h"

#define __MIN(x,y) ( (x) < (y) ? (x) : (y) )
#define __MAX(x,y) ( (x) > (y) ? (x) : (y) )
#define __ABS(x)   ( (x) >= 0  ? (x) : (-x))
#define __INTERVAL(LB,x,RB) ( ( (x) > RB ) ? RB : ( (x) < LB ) ? LB : (x) )

#define HLS_CN_MAX     512
#define HLS_CN_SHIFT   3
#define HLS_DEPTH_MAX  (1 << HLS_CN_SHIFT)

#define HLS_8U   0
#define HLS_8S   1
#define HLS_16U  2
#define HLS_16S  3
#define HLS_32S  4
#define HLS_32F  5
#define HLS_64F  6
#define HLS_USRTYPE1 7

#define HLS_MAT_DEPTH_MASK       (HLS_DEPTH_MAX - 1)
#define HLS_MAT_DEPTH(flags)     ((flags) & HLS_MAT_DEPTH_MASK)

#define HLS_MAKETYPE(depth,cn) (HLS_MAT_DEPTH(depth) + (((cn)-1) << HLS_CN_SHIFT))
#define HLS_MAKE_TYPE HLS_MAKETYPE

#define HLS_8UC1 HLS_MAKETYPE(HLS_8U,1)
#define HLS_8UC2 HLS_MAKETYPE(HLS_8U,2)
#define HLS_8UC3 HLS_MAKETYPE(HLS_8U,3)
#define HLS_8UC4 HLS_MAKETYPE(HLS_8U,4)

#define HLS_8SC1 HLS_MAKETYPE(HLS_8S,1)
#define HLS_8SC2 HLS_MAKETYPE(HLS_8S,2)
#define HLS_8SC3 HLS_MAKETYPE(HLS_8S,3)
#define HLS_8SC4 HLS_MAKETYPE(HLS_8S,4)

#define HLS_16UC1 HLS_MAKETYPE(HLS_16U,1)
#define HLS_16UC2 HLS_MAKETYPE(HLS_16U,2)
#define HLS_16UC3 HLS_MAKETYPE(HLS_16U,3)
#define HLS_16UC4 HLS_MAKETYPE(HLS_16U,4)

#define HLS_16SC1 HLS_MAKETYPE(HLS_16S,1)
#define HLS_16SC2 HLS_MAKETYPE(HLS_16S,2)
#define HLS_16SC3 HLS_MAKETYPE(HLS_16S,3)
#define HLS_16SC4 HLS_MAKETYPE(HLS_16S,4)

#define HLS_32SC1 HLS_MAKETYPE(HLS_32S,1)
#define HLS_32SC2 HLS_MAKETYPE(HLS_32S,2)
#define HLS_32SC3 HLS_MAKETYPE(HLS_32S,3)
#define HLS_32SC4 HLS_MAKETYPE(HLS_32S,4)

#define HLS_32FC1 HLS_MAKETYPE(HLS_32F,1)
#define HLS_32FC2 HLS_MAKETYPE(HLS_32F,2)
#define HLS_32FC3 HLS_MAKETYPE(HLS_32F,3)
#define HLS_32FC4 HLS_MAKETYPE(HLS_32F,4)

#define HLS_64FC1 HLS_MAKETYPE(HLS_64F,1)
#define HLS_64FC2 HLS_MAKETYPE(HLS_64F,2)
#define HLS_64FC3 HLS_MAKETYPE(HLS_64F,3)
#define HLS_64FC4 HLS_MAKETYPE(HLS_64F,4)

#define HLS_MAT_CN_MASK          ((HLS_CN_MAX - 1) << HLS_CN_SHIFT)
#define HLS_MAT_CN(flags)        ((((flags) & HLS_MAT_CN_MASK) >> HLS_CN_SHIFT) + 1)
#define HLS_MAT_TYPE_MASK        (HLS_DEPTH_MAX*HLS_CN_MAX - 1)
#define HLS_MAT_TYPE(flags)      ((flags) & HLS_MAT_TYPE_MASK)

#define HLS_ARE_TYPES_EQ(type1, type2) \
    (((type1 ^ type2) & HLS_MAT_TYPE_MASK) == 0)

#define HLS_ARE_SIZES_EQ(mat1, mat2) \
    ((mat1).rows == (mat2).rows && (mat1).cols == (mat2).cols)

template<int T> struct Type     { typedef unsigned char  name; };
template<> struct Type<HLS_8U>  { typedef unsigned char  name; };
template<> struct Type<HLS_8S>  { typedef char           name; };
template<> struct Type<HLS_16U> { typedef unsigned short name; };
template<> struct Type<HLS_16S> { typedef short          name; };
template<> struct Type<HLS_32S> { typedef int            name; };
template<> struct Type<HLS_32F> { typedef float          name; };
template<> struct Type<HLS_64F> { typedef double         name; };

#define HLS_TNAME(flags) \
    typename Type<HLS_MAT_DEPTH(flags)>::name   

#define HLS_8U_MIN   0
#define HLS_8U_MAX   255
#define HLS_8S_MIN   -127
#define HLS_8S_MAX   127
#define HLS_16U_MIN  0
#define HLS_16U_MAX  65535
#define HLS_16S_MIN  -32767
#define HLS_16S_MAX  32767
#define HLS_32S_MIN  -2147483647
#define HLS_32S_MAX  2147483647

template<typename T> struct Name
{ static const int _min = HLS_32S_MIN; static const int _max = HLS_32S_MAX; };
template<> struct Name<unsigned char> 
{ static const int _min = HLS_8U_MIN;  static const int _max = HLS_8U_MAX; };
template<> struct Name<char>
{ static const int _min = HLS_8S_MIN;  static const int _max = HLS_8U_MAX; };
template<> struct Name<unsigned short>
{ static const int _min = HLS_16U_MIN; static const int _max = HLS_16U_MAX; };
template<> struct Name<short>
{ static const int _min = HLS_16S_MIN; static const int _max = HLS_16S_MAX; };
template<> struct Name<int>
{ static const int _min = HLS_32S_MIN; static const int _max = HLS_32S_MAX; };

#define HLS_ROUND(v) \
    (int( (v) + ( (v) >= 0 ? 0.5 : -0.5 ) ) )

#define IPL_BORDER_CONSTANT       0
#define IPL_BORDER_REPLICATE      1
#define IPL_BORDER_REFLECT        2
#define IPL_BORDER_WRAP           3
#define IPL_BORDER_REFLECT_101    4
#define IPL_BORDER_TRANSPARENT    5

namespace hls {

/* sr_cast: saturate and round cast: T1 -> T2 */

// TODO: ap_ type to float/double cast

template<typename T2, typename T1> inline T2 sr_cast(T1 v) 
{ return (T2)v; }
//{ return __INTERVAL(Name<T2>::_min, int(v), Name<T2>::_max); }

template<> inline unsigned char sr_cast<unsigned char, unsigned short>(unsigned short v)
{ return (unsigned char)(v > HLS_8U_MAX ? HLS_8U_MAX : v); }
template<> inline unsigned char sr_cast<unsigned char, short>(short v)
{ return (unsigned char)__INTERVAL(HLS_8U_MIN, int(v), HLS_8U_MAX); }
template<> inline unsigned char sr_cast<unsigned char, int>(int v)
{ return (unsigned char)__INTERVAL(HLS_8U_MIN, v, HLS_8U_MAX); }
template<> inline unsigned char sr_cast<unsigned char, float>(float v)
{ return (unsigned char)__INTERVAL(HLS_8U_MIN, HLS_ROUND(v), HLS_8U_MAX); }
template<> inline unsigned char sr_cast<unsigned char, double>(double v)
{ return (unsigned char)__INTERVAL(HLS_8U_MIN, HLS_ROUND(v), HLS_8U_MAX); }

template<> inline char sr_cast<char, unsigned short>(unsigned short v)
{ return (char)(v > HLS_8S_MAX ? HLS_8S_MAX : v); }
template<> inline char sr_cast<char, short>(short v)
{ return (char)__INTERVAL(HLS_8S_MIN, int(v), HLS_8S_MAX); }
template<> inline char sr_cast<char, int>(int v)
{ return (char)__INTERVAL(HLS_8S_MIN, int(v), HLS_8S_MAX); }
template<> inline char sr_cast<char, float>(float v)
{ return (char)__INTERVAL(HLS_8S_MIN, HLS_ROUND(v), HLS_8S_MAX); }
template<> inline char sr_cast<char, double>(double v)
{ return (char)__INTERVAL(HLS_8S_MIN, HLS_ROUND(v), HLS_8S_MAX); }

template<> inline unsigned short sr_cast<unsigned short, int>(int v)
{ return (unsigned short)__INTERVAL(HLS_16U_MIN, int(v), HLS_16U_MAX); }
template<> inline unsigned short sr_cast<unsigned short, float>(float v)
{ return (unsigned short)__INTERVAL(HLS_16U_MIN, HLS_ROUND(v), HLS_16U_MAX); }
template<> inline unsigned short sr_cast<unsigned short, double>(double v)
{ return (unsigned short)__INTERVAL(HLS_16U_MIN, HLS_ROUND(v), HLS_16U_MAX); }

template<> inline short sr_cast<short, int>(int v)
{ return (short)__INTERVAL(HLS_16S_MIN, int(v), HLS_16S_MAX); }
template<> inline short sr_cast<short, float>(float v)
{ return (short)__INTERVAL(HLS_16S_MIN, HLS_ROUND(v), HLS_16S_MAX); }
template<> inline short sr_cast<short, double>(double v)
{ return (short)__INTERVAL(HLS_16S_MIN, HLS_ROUND(v), HLS_16S_MAX); }

template<> inline int sr_cast<int, float>(float v)
{ return (int)__INTERVAL(HLS_32S_MIN, HLS_ROUND(v), HLS_32S_MAX); }
template<> inline int sr_cast<int, double>(double v)
{ return (int)__INTERVAL(HLS_32S_MIN, HLS_ROUND(v), HLS_32S_MAX); }


/* Video Format Types defined on 8-bit boundaries */
typedef struct{
  unsigned char Y;
  char UV;
}yuv422_8;

typedef struct{
  unsigned char Y;
  char U;
  char V;
}yuv444_8;

typedef struct{
  unsigned char R;
  unsigned char G;
  unsigned char B;
}rgb_8;

typedef yuv422_8 yuv420_8;

typedef struct{
  char A;
  unsigned char Y;
  char UV;
}yuva422_8;

typedef struct{
  char A;
  unsigned char Y;
  char U;
  char V;
}yuva444_8;

typedef struct{
  char A;
  unsigned char R;
  unsigned char G;
  unsigned char B;
}rgba_8;

typedef struct{
  char AUV;
  unsigned char Y;
}yuva420_8;

typedef struct{
  unsigned char D;
  unsigned char Y;
  char UV;
}yuvd422_8;

typedef struct{
  unsigned char D;
  unsigned char Y;
  char U;
  char V;
}yuvd444_8;

typedef struct{
  unsigned char D;
  unsigned char R;
  unsigned char G;
  unsigned char B;
}rgbd_8;

typedef yuvd422_8 yuvd420_8;

typedef struct{
  unsigned char CMY;
}bayer_8;

typedef struct{
  unsigned char Y;
}luma_8;

/* General AXI pixel datatype */
template<int W>
struct AXI_Base {
    ap_uint<W> data;
    ap_uint<(W+7)/8> strb;
    ap_uint<1> user;
    ap_uint<1> last;
    ap_uint<1> tdest;
};

} // namespace hls


#endif
