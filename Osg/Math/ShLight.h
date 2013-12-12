/*************************************************************************
 * sh.h -- data structures and functions for spherical harmonics and
 *      Wigner matrices.   See sh.c for details.
 *
 * Last modified 12 November 2007 by Ian Lisle.
 *
 * shLight release version 1.0.1 -- 29 November 2007
 *
 * Copyright (C) 2007  Ian Lisle, Tracy Huang
 *
 *      This file is part of shLight. shLight comes with ABSOLUTELY NO
 *      WARRANTY. This is free software, and you are welcome to
 *      redistribute it under certain conditions; please find main.c
 *      for details.
 ************************************************************************/
#pragma once
#ifndef __sh_h_
#define __sh_h_

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif   /* __cplusplus  */

    /* Simple structs for holding SH values, and pointers to them.
     * Also variants where the SH values come in vec3's e.g. RGB.
     *
     *  ShValue    ... float*
     *    Pointer to a flat array of floats -- e.g. values of sh functions.
     *    If A is an ShValue, then A[j] should be e.g. the value of the jth SH function.
     *
     *  ShVecValue ... vec3*
     *    Pointer to an array of sh vec3 values.
     *    If A is an ShVecValue, then A[j] should be a vec3 of values of jth SH functions.
     *
     *  ShValueArray ... ShValue*  ... float**
     *    Pointer to an array of ShValue's
     *    If A is an ShValueArray, then A[i] should be an ShValue, and
     *       A[i][j] should be e.g. the value of the jth SH function in array element i.
     *
     *  ShVecValueArray ... ShVecValue* ... vec3**
     *    Pointer to an array of ShVecValue's
     *    If A is an ShVecValue, then A[i] should be an ShVecValue, and
     *       A[i][j] should be a vec3 e.g. RGB values of j-th SH coeff for array item i.
     *
     *  Sh         ... float**
     *    Pointer to an array of pointers to sh values.
     *    If Y is an Sh, then Y[l][m] should be e.g. the value of Y_lm.
     *
     *  ShVec      ... vec3**
     *    Pointer to an array of pointers to sh vec values.
     *    If A is an ShVec, then A[l][m] should be a vec3 of values e.g. of a_lm [R,G,B]

     *  ShArray    ... Sh*    ...  float***
     *    Pointer to an array of Sh's.
     *    If Y is an ShArray, then Y[i] should be an Sh, and
     *       Y[i][l][m] should be a float, e.g. value of Y_lm for array item i.
     *
     *  ShVecArray  ... ShVec*    ...  vec3***
     *    Pointer to an array of ShVec's.
     *    If A is an ShVecArray, then A[i] should be an ShVec, and
     *       A[i][l][m] should be a vec3, e.g. RGB values for SH coeff a_lm for array item i.
     */

	// These are the actual arrays of values and vectors
	typedef float		vector3[3];
	// Build a vector u from 3 scalars...
	#define v3FloatToVec(a,b,c, u) { u[0]=a; u[1]=b; u[2]=c; }

	// Set u to zero vector...
	#define v3Zero(u) {u[0]=0.0; u[1]=0.0; u[2]=0.0;}

	// Copy a vector: effectively v = u...
	#define v3Copy(u, v) {v[0] = u[0]; v[1] = u[1]; v[2] = u[2];}

	/* ---------- Arithmetic Operations - In place ---------- */

	// Change sign of vec: effectively u = -u
	#define v3ChangeSign(u) {u[0] *= (-1); u[1] *= (-1); u[2] *= (-1);}

	// In-place scalar mult: effectively u = a*u...
	#define v3Scale(a, u) { u[0] *= (a); u[1] *= (a); u[2] *= (a); }

	// In-place addition : effectively v = v + u...
	#define v3AddToVec(u, v){ v[0] += u[0]; v[1] += u[1]; v[2] += u[2];}

	// In-place subtraction: effectively v = v - u...
	#define v3SubtractFromVec(u, v){ v[0] -= u[0]; v[1] -= u[1]; v[2] -= u[2];}

	// In-place accumulation of linear combination: effictively v = v + a*u...
	#define v3Accumulate(a, u, v)  {v[0] += (a)*u[0];  v[1] += (a)*u[1];  v[2] += (a)*u[2];}

	// In-place accumulation of linear combination: effictively v = v + a*u...
	#define v3Length(a)  sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2])

    typedef struct {
        float     alpha;
        float     beta;
        float     gamma;
    } EulerAngles;

	void v3ZyzEulerAngles(const float R[], EulerAngles *orientation);

    typedef float*      ShValue;
    typedef vector3*    ShVecValue;
    typedef ShValue*    ShValueArray;
    typedef ShVecValue* ShVecValueArray;
	// These are arrays of pointers for convenient accessing the actual values
    typedef float**     Sh;
    typedef vector3**   ShVec;
    typedef Sh*         ShArray;
    typedef ShVec*      ShVecArray;

	//<< JAN 03.02.10
	// In case of Zonal harmonics, we can access the values directly 
	// since the second index is always 0.
	typedef float*		Zh;
	typedef vector3*	ZhVec;
	typedef Zh*			ZhArray;
	typedef ZhVec*		ZhVecArray;
	//>>

    /* The struct for holding spherical harmonic values and coefficients
     * is a subtle one.  The wish is that we can use Y[l][m] to refer
     * to the value of an SH function.  The catch is that Y[l] is a vector
     * of size (2*l+1) so is not of fixed size.
     * The solution is to allocate a double-barrelled structure -- an array
     * to hold the actual SH values, and an array of pointers to the
     * locations of Y[0][0], Y[1][0], ..., Y[L][0].
     * For this to work it is essential that the struct be initialised
     * so that the pointers are pointing to the right place.
     * Caller should declare variables of type pointer-to-struct,
     * and call the constructor, so that they get a pointer to an
     * initialised struct.
     */

    /*
    typedef struct {
       int     lMax;
       Sh      Y;
       ShValue YValues;
    } ShCoeffs;
    */

    /* The struct for holding Wigner D-matrix coefficients is an
     * elaboration of the one for spherical harmonic values.
     * The idea is that D[l][m][n] can retrieve the appropriate
     * D-matrix coefficient.
     * There is a triple structure:
     *   Values of D[l][m][n] are stored in a big long vector.
     *     Its length is sum((2*l+1)^2, l=0..L) = (2*L+1)*(2*L+2)*(2*L+3)/6
     *   An array of pointers is created that points at the middle
     *      entry of each column of the matrices.  Its length is
     *      sum(2*l+1, l=0..L) = (L+1)^2.
     *   An array of pointers to pointers is created that points at
     *      the middle entry of each block of pointers.  Its length
     *      is (L+1).
     *  Thus
     *     D[l][m][n]   is a value of a D-matrix coeff.
     *     D[l][m]      is a pointer to row m of l-th Wigner matrix.
     *     D[l]         is a pointer to "the l-th Wigner matrix"
     *                  (really a pointer to a pointer to the middle element).
     * Thus the name D is a pointer to pointer to pointer.
     */


    typedef float*** ShWigner;

    /* Function prototypes */

    /* Constructor */
    ShValue       shNewValue(int lMax);
    ShVecValue    shNewVecValue(int lMax);
    ShValue      *shNewValueArray(int n, int lMax);
    ShVecValue   *shNewVecValueArray(int n, int lMax);
    Sh            shNewSh(int lMax);
    ShVec         shNewShVec(int lMax);
    ShArray       shNewShArray(int n, int lMax);

	//<< JAN 03.02.10
    Zh            shNewZh(int lMax);
    ZhVec         shNewZhVec(int lMax);
	//>>

    /* Construct and evaluate */
    float		 *shNewHValues(int lMax);
    Sh			  shNewNorms(int lMax);
    Sh			  shNewOneOverNorms(int lMax);

    /* Destructor */
    ShValue         shDestroyValue(ShValue a);
    ShVecValue      shDestroyVecValue(ShVecValue a);
    ShValueArray    shDestroyValueArray(ShValueArray A);
    ShVecValueArray shDestroyVecValueArray(ShVecValueArray A);
    Sh              shDestroySh(Sh A);
    ShVec           shDestroyShVec(ShVec A);
    ShArray         shDestroyShArray(ShArray A);
    float          *shDestroyHValues(float *H);

	//<< JAN 03.02.10
    Zh            shDestroyZh(Zh a);
    ZhVec         shDestroyZhVec(ZhVec a);
	//>>

    /* Evaluate */
    Sh shY(vector3 u, int lMax, Sh Y);

    /* For Wigner Matrix */
    ShWigner shNewWigner(int lMax);
    ShWigner shDestroyWigner(ShWigner wigner);
    void shRotToWigner(const float R[16], ShWigner wignerD, const int lMax);
    void shEulerToWigner(EulerAngles &angles, ShWigner, const int);
    void shEvalWignerYZ(ShWigner D, float beta, float gamm, int lMax);
    void shPreMultiplyByRZ(ShWigner D, float alpha, int lMax);

    void shRotateCoeffs(ShWigner wigner, int lMax, Sh    a, Sh    aprime);
	void shRotateFloatCoeffs(ShWigner D, int lMax, float    *a, float*    aprime);
    void shRotateVecCoeffs(ShWigner wigner, int lMax, ShVec a, ShVec aprime);
    void shPrintWigner(ShWigner wigner, int lMax);

	//<< JAN 03.02.10
	void shRotateZhCoeffs(vector3 u, const int lMax, Zh a, Sh aprime);
	void shRotateZhVecCoeffs(vector3 u, const int lMax, ZhVec a, ShVec aprime);
	//>>

#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif /* __sh_h_ */
