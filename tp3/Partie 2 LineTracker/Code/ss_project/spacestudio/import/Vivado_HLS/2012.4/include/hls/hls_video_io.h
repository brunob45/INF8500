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

#ifndef ___HLS__VIDEO__
#warning You are using OpenCV Video library 1.0 which is in Beta. This library is still under development and may have issues in functionality and/or implementation.
#endif

/*
 * HLS Video I/O Function Header File
 */

#ifndef ___HLS__VIDEO_IO__
#define ___HLS__VIDEO_IO__

#include "hls_video_core.h"
#include "utils/x_hls_utils.h"


namespace hls {

template<int W, typename T>
void AXIGetBitFields(AXI_Base<W> axi, int start, int w, T& val) {
#pragma HLS inline
    assert(start >= 0 && start+w <= W);
    val = (T)axi.data(start+w-1, start);
}

template<int W>
void AXIGetBitFields(AXI_Base<W> axi, int start, int w, float& val) {
#pragma HLS inline
    assert(w == 32 && start >= 0 && start+w <= W);
    fp_struct<float> temp((ap_uint<32>)axi.data(start+w-1, start));
    val =  temp.to_float();
}

template<int W>
void AXIGetBitFields(AXI_Base<W> axi, int start, int w, double& val) {
#pragma HLS inline
    assert(w == 64 && start >= 0 && start+w <= W);
    fp_struct<double> temp((ap_uint<64>)axi.data(start+w-1, start));
    val = temp.to_double();
}

template<int W, typename T>
void AXISetBitFields(AXI_Base<W>& axi, int start, int w, T val) {
#pragma HLS inline
    assert(start >= 0 && start+w <= W);
    axi.data(start+w-1, start) = val;
}

template<int W>
void AXISetBitFields(AXI_Base<W>& axi, int start, int w, float val) {
#pragma HLS inline
    assert(w == 32 && start >= 0 && start+w <= W);
    fp_struct<float> temp(val);
    axi.data(start+w-1, start) = temp.data();
}

template<int W>
void AXISetBitFields(AXI_Base<W>& axi, int start, int w, double val) {
#pragma HLS inline
    assert(w == 64 && start >= 0 && start+w <= W);
    fp_struct<double> temp(val);
    axi.data(start+w-1, start) = temp.data();
}

// Unpack a AXI video stream into a hls::Mat<> object

template<int W, int ROWS, int COLS, int T>
void AXIvideo2Mat(stream<AXI_Base<W> >& AXI_video_strm, 
                  Mat<ROWS, COLS, T>& img)
{
    AXI_Base<W> axi;
    Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pix;
    int depth = sizeof(HLS_TNAME(T))*8;
    assert(W >= depth*HLS_MAT_CN(T));
    int rows = img.rows;
    int cols = img.cols;
    for (int i = 0; i < rows; i++) {
#pragma HLS loop_tripcount max=1080
        for (int j = 0; j < cols; j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS pipeline
            AXI_video_strm >> axi;
            for (int k = 0; k < HLS_MAT_CN(T); k++) {
#pragma HLS unroll
                AXIGetBitFields(axi, k*depth, depth, pix.val[k]);
            }
            img << pix;
        }
    }
}

//Pack the data of a hls::Mat<> object into an AXI Video stream

template<int W, int ROWS, int COLS, int T>
void Mat2AXIvideo(Mat<ROWS, COLS, T>& img, 
                  stream<AXI_Base<W> >& AXI_video_strm)
{
    Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pix;
    AXI_Base<W> axi;
    int depth = sizeof(HLS_TNAME(T))*8;
    assert(W >= depth*HLS_MAT_CN(T));
    int rows = img.rows;
    int cols = img.cols;
    for (int i = 0; i < rows; i++) {
#pragma HLS loop_tripcount max=1080
        for (int j = 0; j < cols; j++) {
#pragma HLS loop_tripcount max=1920
#pragma HLS pipeline
            if ((i == 0) && (j == 0)) {
                axi.user = 1;
            } else {
                axi.user = 0;
            }
            if (j == (img.cols-1)) {
                axi.last = 1;
            } else {
                axi.last = 0;
            }
            img >> pix;
            axi.data = -1;
            for (int k = 0; k < HLS_MAT_CN(T); k++) {
#pragma HLS unroll
                AXISetBitFields(axi, k*depth, depth, pix.val[k]);
            }
            AXI_video_strm << axi;
        }
    }
}
     
} // namespace hls

#endif
