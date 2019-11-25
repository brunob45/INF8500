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

#ifndef X_HLS_UTILS_H
#define X_HLS_UTILS_H

#include "ap_fixed.h"

/*
*******************************************************************************
* HLS utilities
* -----------------------
*
* General utilities usable by all functions. Mainly contains classes to
* generate static values from recursive class declarations.
*
* pow2
* sqrtf - C declaration
* to_float
* set_to_one
* set_to_max
*
* fp_struct<float>
* fp_struct<double>
* dumpDouble
*
* Power - B^N
* BitWidth = calculate minimum # of bits to store given integer value
* LowerTri - converts the coordinates of a lower triangular matrix into a
*            sequential index
* LowerTriDim
*
* Table
*
*******************************************************************************
*/

static
const
uint32_t pow2(uint32_t e)
{
    switch(e) {
        case 0: return 1; break;
        case 1: return 2; break;
        case 2: return 4; break;
        case 3: return 8; break;
        case 4: return 16; break;
        case 5: return 32; break;
        case 6: return 64; break;
        case 7: return 128; break;
        case 8: return 256; break;
        case 9: return 512; break;
        case 10: return 1024; break;
        case 11: return 2048; break;
        default: return 0;
    }
}

template<class T>
T reg(T in)
{
    //#ifndef NO_FORCING
#pragma HLS INLINE self off
#pragma HLS INTERFACE ap_none port=return register
    //#endif
    return in;
}

/*
*------------------------------------------------------------------------------
* References C square root function for single precision square root.
* Maps to FPU sqrt core
*------------------------------------------------------------------------------
*/
extern "C" float sqrtf(float);

/*
*------------------------------------------------------------------------------
* Converts data to float type
*------------------------------------------------------------------------------
*/
static
float to_float(float v)
{
    return v;
}

template<int _W, int _I>
float to_float(ap_fixed<_W, _I> v)
{
    return v.to_float();
}


/*
*------------------------------------------------------------------------------
* Overloaded function to set variable to a 1. See fixed point case below.
*------------------------------------------------------------------------------
*/
template<class _T>
void set_to_one(_T &a)
{
    a = 1.0f;
}

/*
* Set the input variable a to 1.
* We approximate 1 by setting the maximum value in the fractional bits.
*/
template<int _W, int _I>
void set_to_one(ap_fixed<_W,_I> &a)
{
    ap_int<_W> tmp_sat = pow((double)2,(double)(_W-_I))-1; //SATURATION_LIMIT_17_BIT;
    a.range() = tmp_sat;
}

/*template<int W, int I, int EW>
void set_to_one(fp<W,I,EW> &a)
{
    a.mant = 1.0;
    a.exp = 0;
    a.flags = REG;
}
*/
template<int _W, int _I>
void set_to_max(ap_fixed<_W,_I> &a)
{
    ap_int<_W> tmp_sat = pow((double)2,(double)_W)-1;
    a.range() = tmp_sat;
}


/*
*------------------------------------------------------------------------------
* Single-precision floating point union to convert between 32-bit int and float
*------------------------------------------------------------------------------
*/

template <typename T>
class fp_struct
{
};

union single_cast {
    float f;
    uint32_t i;
};

template <>
class fp_struct<float>
{
public:
    const static unsigned int EXP_BIAS = 127;
    const static unsigned int EXP_BITS = 8;
    const static unsigned int SIG_BITS = 23;
    fp_struct() {
    }
    fp_struct(float f) {
#if 1
        //def __SYNTHESIS__
        union single_cast dc;
        dc.f = f;
        ap_uint<32> data = dc.i;
#else
        uint32_t t;
        memcpy(&t, &f, sizeof(uint32_t));
        ap_uint<32> data = t;
#endif
        sign[0] = data[31];
        exp(7,0)= data(30,23);
        sig(22,0)= data(22,0);
    }
    fp_struct(ap_uint<32> data) {
        sign[0] = data[31];
        exp(7,0)= data(30,23);
        sig(22,0)= data(22,0);
    }
    fp_struct(uint32_t i) {
        ap_uint<32> data = i;
        sign[0] = data[31];
        exp(7,0)= data(30,23);
        sig(22,0)= data(22,0);
    }
    inline ap_uint<32> data() const {
        ap_uint<32> t;
        t[31] = sign[0];
        t(30,23) = exp(7,0);
        t(22,0) = sig(22,0);
        return t;
    }
    inline int expv() const {
        return exp-127;
    }
    inline int to_int() const {
        return data().to_uint();
    }
    inline float to_float() const {
#if 1
        //def __SYNTHESIS__
        union single_cast ret;
        ret.i = data().to_uint();
        return ret.f;
#else

        uint32_t t = data().to_uint();
        float f;
        memcpy(&f, &t, sizeof(uint32_t));
        return f;
#endif
    }
    inline void set_mantissa(ap_ufixed<1+SIG_BITS,1> mantissa) {
        ap_ufixed<SIG_BITS,0> significand = mantissa;
        sig = significand(SIG_BITS-1,0);
    }
    inline ap_ufixed<1+SIG_BITS,1> mantissa() const {
        ap_ufixed<1+SIG_BITS,1> y = 0;
        y(y.wl()-1,0) = sig(SIG_BITS-1,0);
        y[y.wl()-1] = 1; // The implicit '1' in IEEE format.
        return y;
    }
    inline float to_ieee() const {
        return to_float();
    }
    inline int __signbit() const {
        return sign.to_int();
    }

    typedef ap_uint<32> data_type;
    ap_uint<1>  sign;
    ap_uint<EXP_BITS> exp;
    ap_uint<SIG_BITS> sig;
};

static
std::ostream &operator << (std::ostream &os, const fp_struct<float> &s)
{
#ifndef __SYNTHESIS__
#ifndef __HLS_SYN__
    os << s.to_float();
    os << " (sign: " << s.sign << ", exp: " << s.exp << ", sig: ";
    os << s.sig.to_string(8);
    os << " " << s.sig.to_string(2);
    os << ")";
#endif
#endif
    return os;
}

static
void
castSingle(
    float din,
    fp_struct<float> &dout)
{
    fp_struct<float> t(din);
    dout = t;
}

static
float
castSingle(
    fp_struct<float> din)
{
    return din.to_float();
}

static
void
dumpSingle(
    float da,
    fp_struct<float> ds)
{
#ifdef DEBUG
    std::cout << ds << "\n";
    float sigd = ds.sig.to_int();
    sigd = 1.0 + sigd/8388608.0;
    sigd = sigd * (pow(2,ds.expv()));

    printf("single: %f (sign: %d, exp: %d(%x), sig: %e(%x))\n",da,
           ds.sign.to_int(),ds.expv(),ds.exp.to_int(),sigd,ds.sig.to_int());
#endif
}


/*
*------------------------------------------------------------------------------
* Double-precision floating point union to convert between 64-bit int and double
*------------------------------------------------------------------------------
*/
union double_cast {
    double d;
    uint64_t i;
};

template <>
class fp_struct<double>
{
public:
    const static unsigned int EXP_BIAS = 1023;
    const static unsigned int EXP_BITS = 11;
    const static unsigned int SIG_BITS = 52;
    fp_struct() {
    }
    fp_struct(double f) {
        union double_cast dc;
        dc.d = f;
        ap_uint<64> data   = dc.i;
        sign[0]       = data[63];
        exp(10,0)     = data(62,52);
        sig(51,0)     = data(51,0);
    }
    fp_struct(ap_uint<64> data) {
        sign[0]       = data[63];
        exp(10,0)     = data(62,52);
        sig(51,0)     = data(51,0);
    }
    fp_struct(uint64_t i) {
        ap_uint<64> data = i;
        sign[0]       = data[EXP_BITS+SIG_BITS+1-1];
        exp(EXP_BITS-1,0)     = data(EXP_BITS-1+SIG_BITS,SIG_BITS);
        sig(SIG_BITS-1,0)     = data(SIG_BITS-1,0);
    }
    inline ap_uint<64> data() const {
        ap_uint<64> t;
        t[EXP_BITS+SIG_BITS+1-1] = sign[0];
        t(EXP_BITS-1+SIG_BITS,SIG_BITS) = exp(EXP_BITS-1,0);
        t(SIG_BITS-1,0) = sig(SIG_BITS-1,0);
        return t;
    }
    inline long long to_int() const {
        return data().to_uint64();
    }
    inline int expv() const {
        return exp-1023;
    }
    inline ap_uint<20> sig_msb() const {
        return sig(51,32);
    }
    inline ap_uint<32> sig_lsb() const {
        return sig(31,0);
    }
    inline double to_double() const {
        union double_cast ret;
        ret.i = data().to_uint64();
        return ret.d;
    }
    inline void set_mantissa(ap_ufixed<1+SIG_BITS,1> mantissa) {
        ap_ufixed<SIG_BITS,0> significand = mantissa;
        sig = significand(SIG_BITS-1,0);
    }
    inline ap_ufixed<1+SIG_BITS,1> mantissa() const {
        ap_ufixed<1+SIG_BITS,1> y = 0;
        y(y.wl()-1,0) = sig(SIG_BITS-1,0);
        y[y.wl()-1] = 1; // The implicit '1' in IEEE format.
        return y;
    }
    inline double to_ieee() const {
        return to_double();
    }
    inline int __signbit() const {
        return sign.to_int();
    }
    typedef uint64_t inttype;
    typedef ap_uint<64> data_type;
    ap_uint<1>  sign;
    ap_uint<EXP_BITS> exp;
    ap_uint<SIG_BITS> sig;
};

static
std::ostream &operator << (std::ostream &os, const fp_struct<double> &s)
{
#ifndef __SYNTHESIS__
#ifndef __HLS_SYN__
    os << s.to_double() << " (sign: " << s.sign << ", exp: " << s.exp << ", sig: " << s.sig << " " << s.sig.to_string(2,true) << ")";
#endif
#endif
    return os;
}

static
void
castDouble(
    double din,
    fp_struct<double> &dout)
{
    fp_struct<double> t(din);
    dout = t;
}

static
double
castDouble(
    fp_struct<double> din)
{
    return din.to_double();
}

static
void
dumpDouble(
    double da,
    fp_struct<double> ds)
{
#ifdef DEBUG
    std::cout << ds << "\n";
    double sigd = (ds.sig_msb().to_int() * 4294967296.0) + ds.sig_lsb().to_int();
    sigd = 1.0 + sigd/4503599627370496.0;
    sigd = sigd * (pow(2,ds.expv()));

    printf("double: %f (sign: %d, exp: %d(%x), sig: %e(%x:%x))\n",da,
           ds.sign.to_int(),ds.expv(),ds.exp.to_int(),sigd,ds.sig_msb().to_int(),ds.sig_lsb().to_int());
#endif
}


/*
*------------------------------------------------------------------------------
* Raise a value to a certain power (B^N).
*
* This class is necessary in template arguments due to its static const nature
* and the fact that it performs the power operation as part of the recursive
* class declaration. A standard recursive power function would not return
* a static const value.
*
*------------------------------------------------------------------------------
*/
template < unsigned int _B, unsigned int _N >
class Power
{
public:
    static const unsigned int Value = _B * Power< _B, _N - 1 >::Value;
};

template < unsigned int _B >
class Power< _B, 0 >
{
public:
    static const unsigned int Value = 1;
};

/*
*------------------------------------------------------------------------------
* Calculate the minimum # of bits to store the given integer value.
*
* This class returns the bitwidth result as a static const so can be used
* within a template parameter as well.
*------------------------------------------------------------------------------
*/
template  < unsigned int _N, unsigned int _I=_N/2>
class BitWidth
{
public:
    static const unsigned int Value = 1 + BitWidth<_N,_I/2>::Value;
};

template <unsigned int _N>
class BitWidth<_N, 0>
{
public:
    static const unsigned int Value = 1;
};


/*
*------------------------------------------------------------------------------
* Lower triangular matrix index lookup function
*
* LowerTriDim<N> where N is square matrix dimension
* RowLookup[i]: returns row given sequential index i
* ColLookup[i]: returns row given sequential index i
* SizeLookup[i]: returns maximum index size
*
* Indexes are incremented as follows:
*
* 0
* 1 2
* 3 4 5
* 6 7 8 9
* ...
*
*------------------------------------------------------------------------------
*/

template < unsigned int _DIM, unsigned int _I=_DIM>
class LowerTri  : public LowerTri<_DIM, _I-1>
{
public:
    static const int dummy;
    static const unsigned int Size = _I + LowerTri<_DIM,_I-1>::Size;
};

template < unsigned int _DIM>
class LowerTri<_DIM, 0>
{
public:
    static const int dummy;
    static const unsigned int Size = 0;
};

class LowerTriApply
{
public:
    static int rowApply(int max, int i) {
        int index = 0;
        int row[max];
        int col[max];
        int dim = (int)(sqrtf(0.25+2*max)-0.5);
        for(int ii=0; ii<dim; ii++) {
            for(int jj=0; jj<ii+1; jj++) {
                row[index] = ii;
                col[index] = jj;
                index++;
            }
        }
        return row[i];
    }

    static int colApply(int max, int i) {
        int index = 0;
        int row[max];
        int col[max];
        int dim = (int)(sqrtf(0.25+2*max)-0.5);
        for(int ii=0; ii<dim; ii++) {
            for(int jj=0; jj<ii+1; jj++) {
                row[index] = ii;
                col[index] = jj;
                index++;
            }
        }
        return col[i];
    }

    static int sizeApply(int max, int i) {
        int size[max];
        size[0] = 0;
        for(int ii=1; ii<max; ii++) {
            size[ii] = size[ii-1]+ii;
            if(i == ii) { return size[i]; }
        }
        return size[i];
    }
};

template < int _DIM, int _N=LowerTri<_DIM>::Size, int _I=_N-1>
class LowerTriDim  : public LowerTriDim<_DIM,_N,_I-1>
{
public:
    static const int dummy;
    static const int dummy2;
    static const int dummy3;
};

template < int _DIM, int _N>
class LowerTriDim<_DIM, _N, 0>
{
public:
    static const int dummy;
    static const int dummy2;
    static const int dummy3;
    static int RowLookup[_N];
    static int ColLookup[_N];
    static int SizeLookup[_DIM];
};

template < int _DIM, int _N, int _I>
const int LowerTriDim<_DIM, _N, _I>::dummy =
    LowerTriDim<_DIM,_N,0>::RowLookup[_I] = LowerTriApply::rowApply(_N,_I)
            + 0*LowerTriDim<_DIM,_N,_I-1>::dummy;

template < int _DIM, int _N>
const int LowerTriDim<_DIM, _N, 0>::dummy =
    LowerTriDim<_DIM,_N,0>::RowLookup[0] = LowerTriApply::rowApply(_N,0)
            + 0*LowerTriDim<_DIM,_N,_N>::dummy2;


template < int _DIM, int _N, int _I>
const int LowerTriDim<_DIM, _N, _I>::dummy2 =
    LowerTriDim<_DIM,_N,0>::ColLookup[_I] = LowerTriApply::colApply(_N,_I)
            + 0*LowerTriDim<_DIM,_N,_I-1>::dummy2;

template < int _DIM, int _N>
const int LowerTriDim<_DIM, _N, 0>::dummy2 =
    LowerTriDim<_DIM,_N,0>::ColLookup[0] = LowerTriApply::colApply(_N,0)
            + 0*LowerTriDim<_DIM,_N,_N>::dummy3;

template < int _DIM, int _N, int _I>
const int LowerTriDim<_DIM, _N, _I>::dummy3 =
    LowerTriDim<_DIM,_N,0>::SizeLookup[_I] = LowerTriApply::sizeApply(_N,_I)
            + 0*LowerTriDim<_DIM,_N,_I-1>::dummy3;

template < int _DIM, int _N>
const int LowerTriDim<_DIM, _N, 0>::dummy3 =
    LowerTriDim<_DIM,_N,0>::SizeLookup[0] = LowerTriApply::sizeApply(_N,0);


template<int _DIM, int _N>
int LowerTriDim<_DIM, _N, 0>::RowLookup[_N];

template<int _DIM, int _N>
int LowerTriDim<_DIM, _N, 0>::ColLookup[_N];

template<int _DIM, int _N>
int LowerTriDim<_DIM, _N, 0>::SizeLookup[_DIM];

/*
*******************************************************************************
* Table class
*
* T: table data type
* N: Depth of table entries
*
* public:
* 	dummy			Table data type
*		array[N]	Table array
*******************************************************************************
*/

template <typename _T, int _N, int _I=_N-1>
class Table : public Table<_T, _N, _I-1>
{
public:
    typedef typename _T::TableType TableType;
    static const typename _T::TableType dummy;
    static const int size = _N;

};

template <typename _T, int _N>
class Table<_T, _N, 0>
{
public:
    static const typename _T::TableType dummy;
    static typename _T::TableType array[_N];
};

/*
* Declares the templatized and static Table dummy variable
*
* Assign the global static const Table::dummy variable to the
* associated Table entry by invoking T::apply, assigning it to array[I]
* and adding smaller templates of Table::dummy to it, thereby recursively
* assigning the smaller templates for Tables..
*/

template <typename _T, int _N, int _I>
const typename _T::TableType Table<_T, _N, _I>::dummy
    = Table<_T, _N, 0>::array[_I] = _T::apply(_I,_N) + 0*Table<_T, _N, _I-1>::dummy;

template <typename _T, int _N>
const typename _T::TableType Table<_T, _N, 0>::dummy
    = Table<_T, _N, 0>::array[0] = _T::apply(0,_N);

/*
* Declares the templatized and static Table array
*/
template <typename _T, int _N>
typename _T::TableType Table<_T, _N, 0>::array[_N];



#endif

