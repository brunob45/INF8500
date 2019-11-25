/* -*- c -*-*/
/*
 * Copyright (C) 2011 XILINX, Inc. 
 *
 *
 */


#ifndef __AESL_FPO_H__
#define __AESL_FPO_H__

#include <math.h>

#if (defined AESL_SYN)
#define HLS_FPO_ADDF(a,b)              ((a) + (b))
#define HLS_FPO_ADD(a,b)               ((a) + (b))
#define HLS_FPO_SUBF(a,b)              ((a) - (b))
#define HLS_FPO_SUB(a,b)               ((a) - (b))
#define HLS_FPO_MULF(a,b)              ((a) * (b))
#define HLS_FPO_MUL(a,b)               ((a) * (b))
#define HLS_FPO_DIVF(a,b)              ((a)/(b))
#define HLS_FPO_DIV(a,b)               ((a)/(b))
#define HLS_FPO_RECF(a)                (1/(a))
#define HLS_FPO_REC(a)                 (1/(a))
#define HLS_SQRTF(a)                   sqrtf(a)
#define HLS_SQRT(a)                    sqrt(a)
#define HLS_RECSQRTF(a)                (1/sqrtf(a))
#define HLS_RECSQRT(a)                 (1/sqrt(a))
#define HLS_FPO_ABSF(a)                fabsf(a)
#define HLS_FPO_ABS(a)                 fabs(a)
#define HLS_FPO_LOGF(a)                logf(a)
#define HLS_FPO_LOG(a)                 log(a)
//#define HLS_FPO_UNORDEREDF(a,b)
//#define HLS_FPO_UNORDERED(a,b)
#define HLS_FPO_EQUALF(a,b)            ((a) == (b))
#define HLS_FPO_EQUAL(a,b)             ((a) == (b))  
#define HLS_FPO_LESSF(a,b)             ((a) < (b))
#define HLS_FPO_LESS(a,b)              ((a) < (b))
#define HLS_FPO_LESSEQUALF(a,b)        ((a) <= (b)) 
#define HLS_FPO_LESSEQUAL(a,b)         ((a) <= (b))
#define HLS_FPO_GREATERF(a,b)          ((a) > (b))
#define HLS_FPO_GREATER(a,b)           ((a) > (b))  
#define HLS_FPO_GREATEREQUALF(a,b)     ((a) >= (b)) 
#define HLS_FPO_GREATEREQUAL(a,b)      ((a) >= (b))
#define HLS_FPO_NOTEQUALF(a,b)         ((a) != (b))
#define HLS_FPO_NOTEQUAL(a,b)          ((a) != (b))      
//#define HLS_FPO_CONDCODEF(a,b)
//#define HLS_FPO_CONDCODE(a,b)
#else
#define HLS_FPO_ADDF(a,b)              xil_fpo_add_flt(a,b)        
#define HLS_FPO_ADD(a,b)               xil_fpo_add_d(a,b)
#define HLS_FPO_SUBF(a,b)              xil_fpo_sub_flt(a,b)
#define HLS_FPO_SUB(a,b)               xil_fpo_sub_d(a,b)
#define HLS_FPO_MULF(a,b)              xil_fpo_mul_flt(a,b)
#define HLS_FPO_MUL(a,b)               xil_fpo_mul_d(a,b)
#define HLS_FPO_DIVF(a,b)              xil_fpo_div_flt(a,b)
#define HLS_FPO_DIV(a,b)               xil_fpo_div_d(a,b)
#define HLS_FPO_RECF(a)                xil_fpo_rec_flt(a)
#define HLS_FPO_REC(a)                 xil_fpo_rec_d(a)
#define HLS_SQRTF(a)                   xil_fpo_sqrt_flt(a)
#define HLS_SQRT(a)                    xil_fpo_sqrt_d(a) 
#define HLS_RECSQRTF(a)                xil_fpo_recsqrt_flt(a)      
#define HLS_RECSQRT(a)                 xil_fpo_recsqrt_d(a)
#define HLS_FPO_ABSF(a)                xil_fpo_abs_flt(a)
#define HLS_FPO_ABS(a)                 xil_fpo_abs_d(a)
#define HLS_FPO_LOGF(a)                xil_fpo_log_flt(a)
#define HLS_FPO_LOG(a)                 xil_fpo_log_d(a)
#define HLS_FPO_UNORDEREDF(a,b)        xil_fpo_unordered_flt(a,b)
#define HLS_FPO_UNORDERED(a,b)         xil_fpo_unordered_d(a,b)
#define HLS_FPO_EQUALF(a,b)            xil_fpo_equal_flt(a,b)
#define HLS_FPO_EQUAL(a,b)             xil_fpo_equal_d(a,b)
#define HLS_FPO_LESSF(a,b)             xil_fpo_less_flt(a,b)
#define HLS_FPO_LESS(a,b)              xil_fpo_less_d(a,b)
#define HLS_FPO_LESSEQUALF(a,b)        xil_fpo_lessequal_flt(a,b)
#define HLS_FPO_LESSEQUAL(a,b)         xil_fpo_lessequal_d(a,b)
#define HLS_FPO_GREATERF(a,b)          xil_fpo_greater_flt(a,b)
#define HLS_FPO_GREATER(a,b)           xil_fpo_greater_d(a,b)
#define HLS_FPO_GREATEREQUALF(a,b)     xil_fpo_greaterequal_flt(a,b)
#define HLS_FPO_GREATEREQUAL(a,b)      xil_fpo_greaterequal_d(a,b)
#define HLS_FPO_NOTEQUALF(a,b)         xil_fpo_notequal_flt(a,b)
#define HLS_FPO_NOTEQUAL(a,b)          xil_fpo_notequal_d(a,b)
#define HLS_FPO_CONDCODEF(a,b)         xil_fpo_condcode_flt(a,b)
#define HLS_FPO_CONDCODE(a,b)          xil_fpo_condcode_d(a,b)

#include <stdio.h>
#include "floating_point_v6_1_bitacc_cmodel.h"  // Must include before GMP and MPFR 
#include "gmp.h"
#include "mpfr.h"

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: add
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_add_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  float res_flt;

  // xip_fpo_add_flt
  exc = xip_fpo_add_flt(&res_flt, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_add_flt: float = a + b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_flt;
}

double xil_fpo_add_d(double a, double b)
{
  xip_fpo_exc_t exc;
  double res_d;

  // xip_fpo_add_d
  exc = xip_fpo_add_d(&res_d, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_add_d: float = a + b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: subtract
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_sub_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  float res_flt;

  // xip_fpo_sub_flt
  exc = xip_fpo_sub_flt(&res_flt, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_sub_flt: float = a - b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_flt;
}

double xil_fpo_sub_d(double a, double b)
{
  xip_fpo_exc_t exc;
  double res_d;

  // xip_fpo_sub_d
  exc = xip_fpo_sub_d(&res_d, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_sub_flt: double = a - b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: multiply
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_mul_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  float res_flt;
 
  // xip_fpo_mul_flt
  exc = xip_fpo_mul_flt(&res_flt, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_mul_flt: float = 17.6 * -3.2: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_flt;
}

double xil_fpo_mul_d(double a, double b)
{
  xip_fpo_exc_t exc;
  double res_d;

  // xip_fpo_mul_d
  exc = xip_fpo_mul_d(&res_d, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_mul_flt: float = a * b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: divide
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_div_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  float res_flt;

  // xip_fpo_div_flt
  exc = xip_fpo_div_flt(&res_flt, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_div_flt: float = a / b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_flt;
}

double xil_fpo_div_d(double a, double b)
{
  xip_fpo_exc_t exc;
  double res_d;

  // xip_fpo_div_d
  exc = xip_fpo_div_d(&res_d, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_div_d: double = a / b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: reciprocal
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_rec_flt(float a)
{
  xip_fpo_exc_t exc;
  float res_flt;

  // xip_fpo_rec_flt
  exc = xip_fpo_rec_flt(&res_flt, a);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_rec_flt: float = 1 / a: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_flt;
}

double xil_fpo_rec_d(double a)
{
  xip_fpo_exc_t exc;
  double res_d;

  // xip_fpo_rec_d
  exc = xip_fpo_rec_d(&res_d, a);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_rec_d: double = 1 / a: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: square root
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_sqrt_flt(float a)
{
//  printf("Testing operation functions: square root\n");
  xip_fpo_exc_t exc;
  float res_flt;

  // xip_fpo_sqrt_flt
  exc = xip_fpo_sqrt_flt(&res_flt, a);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_sqrt_flt: float = sqrt(a): no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
//  printf("float = sqrtf(a), and got res_flt=%f\n", res_flt);
  return res_flt;
}

double xil_fpo_sqrt_d(double a)
{
  xip_fpo_exc_t exc;
  double res_d;

  // xip_fpo_sqrt_d
  exc = xip_fpo_sqrt_d(&res_d, a);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_sqrt_d: double = sqrt(a): no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
//  printf("double = sqrt(a), and got res_d=%f\n", res_d);
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: reciprocal square root
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_recsqrt_flt(float a)      
{
  xip_fpo_exc_t exc;
  float res_flt;

  // xip_fpo_recsqrt_flt
  exc = xip_fpo_recsqrt_flt(&res_flt, a);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_recsqrt_flt: float = 1 / a: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_flt;
}

double xil_fpo_recsqrt_d(double a)
{
  xip_fpo_exc_t exc;
  double res_d;

  // xip_fpo_recsqrt_d
  exc = xip_fpo_recsqrt_d(&res_d, a);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_recsqrt_flt: float = 1 / a: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: absolute value
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_abs_flt(float a)
{
  xip_fpo_exc_t exc;
  float res_flt;

  exc = xip_fpo_abs_flt(&res_flt, a);
  if (exc != 0) {
    printf("ERROR: xip_fpo_abs : FP(8.24) = abs(a): no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_flt;
}

double xil_fpo_abs_d(double a)
{
  xip_fpo_exc_t exc;
  double res_d;

  exc = xip_fpo_abs_d(&res_d, a);
  if (exc != 0) {
    printf("ERROR: xip_fpo_abs_d : FP(11.53) = abs(a): no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: logarithm
  ////////////////////////////////////////////////////////////////////////

float xil_fpo_log_flt(float a)
{
  xip_fpo_exc_t exc;
  float res_flt;

  // xip_fpo_log_flt
  exc = xip_fpo_log_flt(&res_flt, a);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_log_flt: float = log(a): no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_flt;
}

double xil_fpo_log_d(double a)
{
  xip_fpo_exc_t exc;
  double res_d;

  // xip_fpo_log_d
  exc = xip_fpo_log_d(&res_d, a);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_log_d: double = log(a): no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_d;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: compare unordered
  ////////////////////////////////////////////////////////////////////////

int xil_fpo_unordered_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_unordered_flt
  exc = xip_fpo_unordered_flt(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_unordered_flt: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

int xil_fpo_unordered_d(double a, double b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_unordered_d
  exc = xip_fpo_unordered_d(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_unordered_d: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: compare equal
  ////////////////////////////////////////////////////////////////////////


int xil_fpo_equal_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_equal_flt
  exc = xip_fpo_equal_flt(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_equal_flt: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

int xil_fpo_equal_d(double a, double b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_equal_d
  exc = xip_fpo_equal_d(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_equal_d: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: compare less than
  ////////////////////////////////////////////////////////////////////////

int xil_fpo_less_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_less_flt
  exc = xip_fpo_less_flt(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_less_flt: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

int xil_fpo_less_d(double a, double b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_less_d
  exc = xip_fpo_less_d(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_less_d: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: compare less than or equal
  ////////////////////////////////////////////////////////////////////////

int xil_fpo_lessequal_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_lessequal_flt
  exc = xip_fpo_lessequal_flt(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_lessequal_flt: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

int xil_fpo_lessequal_d(double a, double b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_lessequal_d
  exc = xip_fpo_lessequal_d(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_lessequal_d: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: compare greater than
  ////////////////////////////////////////////////////////////////////////

int xil_fpo_greater_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_greater_flt
  exc = xip_fpo_greater_flt(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_greater_flt: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

int xil_fpo_greater_d(double a, double b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_greater_d
  exc = xip_fpo_greater_d(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_greater_d: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: compare greater than or equal
  ////////////////////////////////////////////////////////////////////////

int xil_fpo_greaterequal_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_greaterequal_flt
  exc = xip_fpo_greaterequal_flt(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_greaterequal_flt: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

int xil_fpo_greaterequal_d(double a, double b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_greaterequal_d
  exc = xip_fpo_greaterequal_d(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_greaterequal_d: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: compare not equal
  ////////////////////////////////////////////////////////////////////////

int xil_fpo_notequal_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_notequal_flt
  exc = xip_fpo_notequal_flt(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_notequal_flt: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

int xil_fpo_notequal_d(double a, double b)
{
  xip_fpo_exc_t exc;
  int res_int;

  exc = xip_fpo_notequal_d(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_notequal_d: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

  ////////////////////////////////////////////////////////////////////////
  // Operation functions: compare condition code
  ////////////////////////////////////////////////////////////////////////

int xil_fpo_condcode_flt(float a, float b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_condcode_flt
  exc = xip_fpo_condcode_flt(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_condcode_flt: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

int xil_fpo_condcode_d(double a, double b)
{
  xip_fpo_exc_t exc;
  int res_int;

  // xip_fpo_condcode_d
  exc = xip_fpo_condcode_d(&res_int, a, b);  // normal operation
  if (exc != 0) {
    printf("ERROR: xip_fpo_condcode_d: compare a and b: no exceptions expected (exc=0), but got exc=%d\n", exc);
    return -1;
  }
  return res_int;
}

#endif
#endif /* #ifndef __AESL_FPO_H__*/
