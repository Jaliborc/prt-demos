/*************************************************************************
 * sh.c -- Functions for evaluating spherical harmonics and Wigner
 *      matrices in the real SH basis.
 *
 * Last modified 20 November 2007 by Ian Lisle.
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

#if defined(WIN32)
#include <malloc.h>
#endif
#include <math.h>
#include <osg/Math>
#include "ShLight.h"

static int maxDegree = -1;
static float *cm = NULL, *sm = NULL;     /* for storing cos(m) and sin(m) */
static Sh normY = NULL, oneOverNormY = NULL; /* for norms of SH functions */
static Sh aan = NULL, bbn = NULL;        /* for coeffs in SH recurrence   */

static void normYUpdate(int lMax);
static void  aanUpdate(int lMax);

void v3ZyzEulerAngles(const float R[], EulerAngles *orientation) {
		/* Calculate Z-Y-Z Euler angles from the model view matrix
		 * R, whose top left 3x3 block is presumed to  be orthogonal.
		 * Since R is 4x4, the elements we want are:
		 * 0  4  8
		 * 1  5  9
		 * 2  6  10
		 * Answers alpha,beta,gamm are returned as side effects. */
		if (R[10] > 0.9999) {
			orientation->alpha = (float)atan2(R[4], R[5]);
			orientation->beta  = 0.0;
			orientation->gamma = 0.0;
			return;
		} else if (R[10] < -0.9999) {
			orientation->alpha = (float)atan2(R[4], R[5]);
			orientation->beta  = osg::PI;
			orientation->gamma = 0.0;
			return;
		}

		orientation->beta = (float)acos(R[10]);
		orientation->alpha = (float)atan2(R[6], R[2]);
		orientation->gamma = (float)atan2(R[9], -R[8]);
	}



/* Private -- maxDegree is tended automatically by shY etc.,
 *            no need for external user to do explicitly.
 */
static void shUpdateMaxDegree(int lMax) {
    /* Update maxDegree.  */
    if (lMax <= maxDegree) return;
    // If we got to here then maxDegree needs to be updated...
    maxDegree = lMax;

    normYUpdate(lMax);

    // Re-malloc arrays for holding cos(m*alpha) and sin(m*alpha)...
    free(cm);
    free(sm);
    cm = (float *)malloc((lMax + 1) * sizeof(float)),
         sm = (float *)malloc((lMax + 1) * sizeof(float));
    assert(cm != NULL);
    assert(sm != NULL);
    cm[0] = 1.0;
    sm[0] = 0.0;

    // Now do the same thing for the Sh structs holding
    // the coefficients in the SH recurrence.
    aanUpdate(lMax);

    return;
}



/* =============  CONSTRUCTOR FUNCTIONS  ============================= */

// #define shNewValue(lMax) (float *) calloc( ((lMax)+1) * ((lMax)+1), sizeof(float) )
ShValue shNewValue(int lMax) {
    float *A;
    A = (float *) calloc((lMax + 1) * (lMax + 1), sizeof(float));
    return A;
}

// #define shNewVecValue(lMax) (vector3 *) calloc( ((lMax)+1) * ((lMax)+1), sizeof(vector3) )
ShVecValue shNewVecValue(int lMax) {
    vector3 *A;
    A = (vector3 *) calloc((lMax + 1) * (lMax + 1), sizeof(vector3));
    return A;
}

ShValue *shNewValueArray(int n, int lMax) {
    /* Allocate and initialise an array of shValues.
     *   The struct allocated will be an array of pointers-to-float.
     *   but we also allocate an array of floats to hold the actual data.
     *   The crucial thing is initialising the pointers so that they point
     *   to the floats.  */
    ShValue *A;
    float   *x;
    int i;
    A = (ShValue *) malloc(n * sizeof(ShValue));
    x = (float *) calloc(n * (lMax + 1) * (lMax + 1), sizeof(float));
    if (A == NULL || x == NULL) {
        free(x);
        free(A);
        return NULL;
    }
    /* OK, got the memory, now attach pointers to the floats... */
    for (i = 0; i < n; i++)
        A[i] = &(x[i*(lMax+1)*(lMax+1)]);
    return A;
}

ShVecValue *shNewVecValueArray(int n, int lMax) {
    /* Allocate and initialise an array of shVecValues.
     *   The struct allocated will be an array of pointers-to-vector3.
     *   but we also allocate an array of vector3's to hold the actual data.
     *   The crucial thing is initialising the pointers so that they point
     *   to the vector3's.  */
    ShVecValue *A;
    vector3       *x;
    int i;
    A = (ShVecValue *) malloc(n * sizeof(ShVecValue));
    x = (vector3*)calloc(n * (lMax + 1) * (lMax + 1), sizeof(vector3));
    if (A == NULL || x == NULL) {
        free(x);
        free(A);
        return NULL;
    }
    /* OK, got the memory, now attach pointers to the floats... */
    for (i = 0; i < n; i++)
        A[i] = &(x[i*(lMax+1)*(lMax+1)]);
    return A;
}

Sh shNewSh(int lMax) {
    /* Constructor for an Sh struct, i.e. an array of pointers
     * to float, plus the array of floats themselves.
     * Such that a[l][m] shall the right SH coeff. */
    float **a;
    float  *x;
    int l;
    a = (float **) malloc((lMax + 1) * sizeof(float*));
    if (a == NULL)
        return NULL;
    x = shNewValue(lMax);
    if (x == NULL) {
        free(a);
        return NULL;
    }
    /* OK, got all the memory we need, now initialise pointers... */
    for (l = 0; l <= lMax; l++)
        a[l] = &(x[l*l+l]);
    return a;
}

ShVec shNewShVec(int lMax) {
    // Initialise a spherical harmonics vector3 struct
    //   -Allocate memory for it
    //   -Initialise pointers
    // This is mostly so we can store coeffs for [R,G,B] light components.
    // Only needed because we can't overload functions in C.
    int l;
    ShVec      a;  // Really a vector3**
    ShVecValue u;  // Really a vector3*
    a = (ShVec) malloc((lMax + 1) * sizeof(vector3 *));
    if (a == NULL) return NULL;
    u = (ShVecValue) calloc((lMax + 1) * (lMax + 1), sizeof(vector3));
    if (u == NULL) {
        free(a);
        return NULL;
    }
    /* OK, got all the memory we need, now initialise pointers... */
    for (l = 0; l <= lMax; l++)
        a[l] = &(u[l*l+l]);
    return a;
}


ShArray shNewShArray(int n, int lMax) {
    /* Constructor for an array of Sh structs, i.e. an array of Sh's,
     * plus an array of pointers to float, plus the floats themselves.
     * Such that a[j][l][m] shall the right SH coeff. */
    // float **a;
    Sh     *a;
    float  **aj;
    float  *x;
    int j, l;
    a = (Sh *) malloc(n * sizeof(Sh));
    aj = (float **) malloc(n * (lMax + 1) * sizeof(float *));
    x  = (float *)  calloc(n * (lMax + 1) * (lMax + 1), sizeof(float));
    if (a == NULL || aj == NULL || x == NULL) {
        free(a);
        free(aj);
        free(x);
        return NULL;
    }
    /* OK, got all the memory we need, now initialise pointers... */
    for (j = 0; j < n; j++) {
        a[j] = &(aj[j*(lMax+1)]);
        for (l = 0; l <= lMax; l++)
            a[j][l] = &(x[j*(lMax+1)*(lMax+1) + l*l+l]);
    }
    return a;
}

//<< JAN 03.02.10
Zh shNewZh(int lMax){
    float *A;
    A = (float *) calloc((lMax + 1), sizeof(float));
    return A;
}

ZhVec shNewZhVec(int lMax){
    vector3 *A;
    A = (vector3 *) calloc((lMax + 1), sizeof(vector3));
    return A;
}
//>>




/* --------  Construct and evaluate  ---------------------- */

float *shNewHValues(int lMax) {
    // Return a pointer to an array of H values, as used in
    // unshadowed SH lighting.  These are coeffs of Legendre
    // poly expansion of (cos theta)_+
    // This should only need to be called once or twice, so compute
    // on-the-fly.  Caller can do this once and store.
    float *H;
    int    n;
    H = (float *) malloc((lMax + 1) * sizeof(float));
    if (H == NULL) return NULL;
    H[0] = 1;
    H[1] = 2.0 / 3.0 * 1;
    for (n = 0; 2*n + 2 <= lMax; n++)
        H[2*n+2] = (0.5 - n) / (2. + n) * H[2*n];
    for (n = 1; 2*n + 1 <= lMax; n++)
        H[2*n+1] = 0;
    return H;
}


Sh shNewOneOverNorms(int lMax) {
    // Returns values of one over norms of SH functions to degree lMax.
    // Caller should need this once or twice only -- once known, the norms
    // can just be reused again and again.  So no need for great efficiency.
    Sh norms;
    ShValue normValues;
    int j;
    norms = shNewNorms(lMax);
    if (norms == NULL) return NULL;
    // OK -- we got the norms, now invert the floating pt values...
    normValues = *norms;
    for (j = 0; j < (lMax + 1)*(lMax + 1); j++)
        normValues[j] = 1.0 / normValues[j];
    return norms;  // These are really 1/norms now.
}


/* ========  DESTRUCTORS  (all public)  ================= */
ShValue shDestroyValue(ShValue a) {
    free(a);
    return NULL;
}

ShVecValue shDestroyVecValue(ShVecValue a) {
    free(a);
    return NULL;
}

ShValueArray shDestroyValueArray(ShValueArray A) {
    if (A == NULL) return NULL;
    free(*A);
    free(A);
    return NULL;
}

ShVecValueArray shDestroyVecValueArray(ShVecValueArray A) {
    if (A == NULL) return NULL;
    free(*A);
    free(A);
    return NULL;
}

Sh shDestroySh(Sh A) {
    if (A == NULL) return NULL;
    free(*A);
    free(A);
    return NULL;
}

ShVec shDestroyShVec(ShVec A) {
    if (A == NULL) return NULL;
    free(*A);
    free(A);
    return NULL;
}

ShArray shDestroyShArray(ShArray A) {
    if (A == NULL) return NULL;
    free(**A);
    free(*A);
    free(A);
    return NULL;
}


float *shDestroyHValues(float *H) {
    free(H);
    return NULL;
}

//<< JAN 03.02.10
Zh shDestroyZh(Zh a){
	if (a == NULL) return NULL;
	free(a);
	return NULL;
}

ZhVec shDestroyZhVec(ZhVec a){
	if (a == NULL) return NULL;
	free(a);
	return NULL;
}
//>>

/* Public */
Sh  shNewNorms(int lMax) {
    // Compute norms of (unnormalised) SH basis functions and return
    // pointer to them.
    // No great need for efficiency, this only has to be done once or twice.
    // The norms of the associated Legendre functions are computed
    // locally and only half-fill the array since we need only m>=0.
    int l, m;
    Sh normY, normP;

    normP  = shNewSh(lMax);
    if (normP == NULL) return NULL;
    normY = shNewSh(lMax);
    if (normY == NULL) return NULL;
    for (l = 0; l <= lMax; l++) {
        normP[l][0]    = sqrt(2. / (2 * l + 1.));
        normY[l][0]    = sqrt(2.*osg::PI) * normP[l][0];
        for (m = 1; m <= l; m++) {
            normP[l][m]  = sqrt((float)(l + m) * (l - m + 1)) * normP[l][m-1];
            normY[l][m]  = sqrt(osg::PI) * normP[l][m];
            normY[l][-m] = normY[l][m];
        }
    }
    normP = shDestroySh(normP);
    return normY;
}

/* Private */
static void normYUpdate(int lMax) {
    /* Update the static arrays that hold normY and its inverses. */
    int i;
    normY = shDestroySh(normY);
    normY = shNewNorms(lMax);
    assert(normY != NULL);
    oneOverNormY = shDestroySh(oneOverNormY);
    oneOverNormY = shNewSh(lMax);
    assert(oneOverNormY != NULL);
    for (i = 0; i < (lMax + 1)*(lMax + 1); i++)
        (*oneOverNormY)[i] = 1.0 / (*normY)[i];
    return;
}


/* Private */
static void aanUpdate(int lMax) {
    // Update storage and values for static Sh aan.
    // Evaluates the coeffs in the recurrence for NORMALISED SH functions.
    // First strip down the old values, then create new ones.
    // Presumes someone else has checked that this in fact needs to be done.
    int l, m;
    aan = shDestroySh(aan);
    bbn = shDestroySh(bbn);
    aan = shNewSh(lMax);
    bbn = shNewSh(lMax);
    assert(aan != NULL);
    assert(bbn != NULL);
    aan[0][0] = 1.0 / sqrt(4.0 * osg::PI);
    if (lMax == 0) return;
    aan[1][0] = aan[0][0] * sqrt(3.0);
    aan[1][1] = -aan[1][0];
    for (l = 2; l <= lMax; l++) {
        aan[l][l] =  -sqrt((2.0 * l + 1) / (2.0 * l));
        aan[l][l-1] = sqrt(2.0 * l + 1.0);
        aan[l][0]   = sqrt(4 * l * l - 1.0) / l;
        bbn[l][0]   = (1.0 / l - 1.0) * sqrt((float)(2 * l + 1) / (float)(2 * l - 3));
        for (m = 1; m < l; m++) {
            aan[l][m] =  sqrt((float)(4 * l * l - 1) / (float)(l * l - m * m));
            bbn[l][m] = -sqrt((float)((2 * l + 1) * ((l - 1) * (l - 1) - m * m)) /
                              (float)((2 * l - 3) * (l * l - m * m))
                             );
        }
    }
}


/* Public */
// This function takes a unit vector, number of basis functions and fills the
// Sh vector Y with the value of each basis function in direction u.
// In other words: It evaluates the value of each basis function in given direction
// and stores it in Y.
Sh shY(vector3 u, int lMax, Sh Y) {
    // Given a vector u, calculate (normalised) spherical harmonics
    // up to degree lMax, using cached values of recurrence coeffs.
    // Preconditions:
    //   + u is unit vector
    //   + lMax >= 0
    //   + Y is properly constructed Sh data structure with room
    //     for sh values up to degree lMax.
    float x = u[0], y = u[1], z = u[2];
    int l, m;

    if (lMax > maxDegree) shUpdateMaxDegree(lMax);
    Y[0][0] = aan[0][0];
    if (lMax == 0)
        return Y;
    // Might as well work out the degree 1 case immediately...
    Y[1][0]  =  aan[1][0] * z;
    Y[1][-1] =  aan[1][1] * y;
    Y[1][1]  =  aan[1][1] * x;
    // And then do DEGREES 2..L via the recurrences...
    for (l = 2; l <= lMax; l++) {
        Y[l][l]    = aan[l][l] * (x * Y[l-1][l-1] - y * Y[l-1][-l+1]);
        Y[l][-l]   = aan[l][l] * (y * Y[l-1][l-1] + x * Y[l-1][-l+1]);
        Y[l][l-1]  = aan[l][l-1] * z * Y[l-1][l-1];
        Y[l][-l+1] = aan[l][l-1] * z * Y[l-1][-l+1];
        Y[l][0]    = aan[l][0]   * z * Y[l-1][0] + bbn[l][0] * Y[l-2][0];
        for (m = 1; m <= l - 2; m++) {
            Y[l][m]  = aan[l][m] * z * Y[l-1][m]  + bbn[l][m] * Y[l-2][m] ;
            Y[l][-m] = aan[l][m] * z * Y[l-1][-m] + bbn[l][m] * Y[l-2][-m];
        }
    }
    return Y;
}

float l2Norm(Sh a, const int lMax, float norms[]) {
    /* Assuming that sh contains SH coefficients, compute
     * the l^2 norm of the function it represents by sqrt of SSQ.
     * The energy is broken down by degree l.*/
    float SSQ = 0.0;
    int l, m;

    for (l = 0; l <= lMax; l++) {
        norms[l] = 0.0;
        for (m = -l; m <= l; m++)
            norms[l] += a[l][m] * a[l][m];
        SSQ += norms[l];
        norms[l] = sqrt(norms[l]);
    }
    return (float) sqrt(SSQ);
}


void l2VecNorm(ShVec a, const int lMax, vector3 norms[], vector3 norm) {
    /* Assuming that sh contains SH coefficients, compute
     * the l^2 norm of the function it represents by sqrt of SSQ.
     * The energy is broken down by degree l.*/
    vector3 SSQ = {0.0, 0.0, 0.0};
    int l, m;
    static const int r = 0, g = 1, b = 2;

    for (l = 0; l <= maxDegree; l++) {
        v3Zero(norms[l]);
        for (m = -l; m <= l; m++) {
            norms[l][r] += a[l][m][r] * a[l][m][r];
            norms[l][g] += a[l][m][g] * a[l][m][g];
            norms[l][b] += a[l][m][b] * a[l][m][b];
        }
        v3AddToVec(norms[l], SSQ);
        norms[l][r] = sqrt(norms[l][r]);
        norms[l][g] = sqrt(norms[l][g]);
        norms[l][b] = sqrt(norms[l][b]);
    }
    norm[r] = sqrt(SSQ[r]);
    norm[g] = sqrt(SSQ[g]);
    norm[b] = sqrt(SSQ[b]);
    return;
}


/* ==============  WIGNER MATRIX FUNCTIONS  ================== */


/* Wigner Constructor ... */
ShWigner shNewWigner(int lMax) {
    // Create a Wigner D-matrix up to SH degree lMax:
    //   -Allocate memory for it
    //   -Initialise pointers (and ptrs-to-ptrs)
    //   -Initialise values to identity
    ShWigner D;
    float **DL;
    float  *DValues;
    int l, m, middle = -1;

    D  = (float ***) malloc((lMax + 1) * sizeof(float **));
    DL = (float **)  malloc((lMax + 1) * (lMax + 1) * sizeof(float *));
    DValues = (float *) calloc((2 * lMax + 1) * (2 * lMax + 2) * (2 * lMax + 3) / 6, sizeof(float));

    if (D == NULL || DL == NULL || DValues == NULL) {
        free(D);
        free(DL);
        free(DValues);
        return NULL;
    }
    for (l = 0; l <= lMax; l++) {
        D[l] = &(DL[l*l+l]);
        middle += ((2 * l - 1) * (2 * l - 1) + (2 * l + 1) * (2 * l + 1)) / 2;
        for (m = -l; m <= l; m++) {
            D[l][m] = &(DValues[middle+m*(2*l+1)]);
            D[l][m][m] = 1.0;   // Initialise to identity matrix
        }
    }
    return D;
}


/* Wigner Destructor... */
ShWigner shDestroyWigner(ShWigner D) {
    // Strip down a Wigner D-matrix and return NULL
    if (D == NULL) return NULL; // Nothing to strip down

    free(**D);  // Free the floats
    free(*D);   // Free the float*'s
    free(D);    // Free the float**'s
    return NULL;
}



// This proc uses the recurrence  for real-basis y-Wigner
// to calculate all d-values up to degree maxDegree, stuffing them into
// an Array of "Matrices"..
static void shEvalRealWigner(ShWigner d, float beta, int lMax) {
    float c = cos(beta), s = sin(beta);
    int l, m, n, signl, signm, signlm, signmn;

    // Degree 0...
    d[0][0][0] = 1;
    if (lMax == 0) return;

    if (lMax > maxDegree) shUpdateMaxDegree(lMax);
    // Populate the blocks of zeros...
    for (l = 1; l <= lMax; l++) {
        for (m = 1; m <= l; m++) {
            d[l][0][-m] = 0;
            d[l][-m][0] = 0;
            for (n = 1; n <= l; n++) {
                d[l][m][-n] = 0;
                d[l][-n][m] = 0;
            }
        }
    }

    // Degree 1...
    // Corners...
    d[1][1][1]   = c;
    d[1][-1][-1] = 1;
    // Edges...
    d[1][0][1] = s;
    d[1][1][0] = -s;
    // Inner border (i.e. centre element)...
    d[1][0][0] = c;
    if (lMax == 1)
        return;

    // Now all degrees from 2 upwards...
    signl = -1; /* because l starts from 2 */
    for (l = 2; l <= lMax; l++) {
        signl = -signl; /* signl := (-1)^l */
        // Corners...
        d[l][l][l]   = 0.5 * c * d[l-1][l-1][l-1] + 0.5  * d[l-1][-l+1][-l+1];
        d[l][-l][-l] = 0.5  * d[l-1][l-1][l-1] + 0.5 * c * d[l-1][-l+1][-l+1];

        // Edges...
        d[l][0][l] = sqrt((l - 0.5) / l) * s * d[l-1][0][l-1];
        d[l][l][0] = signl * d[l][0][l];

        signlm = signl;
        for (m = 1; m < l; m++) {
            signlm = -signlm; /* signlm := (-1)^(l-m) */
            d[l][m][l]   =  sqrt(l * (l - 0.5) / (l * l - m * m)) * s * d[l-1][m][l-1];
            d[l][l][m]   =  signlm * d[l][m][l];
            d[l][-m][-l] =  sqrt(l * (l - 0.5) / (l * l - m * m)) * s * d[l-1][-m][-l+1];
            d[l][-l][-m] =  signlm * d[l][-m][-l];
        }

        // Inner border...
        // Corners...
        d[l][l-1][l-1]   =  l * c * d[l-1][l-1][l-1] + (1 - l) * d[l-1][-l+1][-l+1];
        d[l][-l+1][-l+1] = (1 - l) * d[l-1][l-1][l-1] +  l * c * d[l-1][-l+1][-l+1];

        // Ends of horizontal cross...
        d[l][0][l-1] = sqrt((float)2 * l - 1) * c * d[l-1][0][l-1];
        d[l][l-1][0] = -signl * d[l][0][l-1]; /* -signl:= (-1)^(l-1) */

        signlm = signl;
        for (m = 0; m < l - 1; m++) {
            signlm = -signlm; /* signlm := (-1)^(l-m-1) because m starts from 0 */
            d[l][m][l-1]   =  sqrt((2 * l - 1.0) / (l * l - m * m)) * l * c * d[l-1][m][l-1] - sqrt((2 * l - 1.0) / (l * l - m * m)) * m   * d[l-1][-m][-l+1];
            d[l][-m][-l+1] = -sqrt((2 * l - 1.0) / (l * l - m * m)) * m *   d[l-1][m][l-1] + sqrt((2 * l - 1.0) / (l * l - m * m)) * l * c * d[l-1][-m][-l+1];
            d[l][l-1][m]   = signlm * d[l][m][l-1];
            d[l][-l+1][-m] = signlm * d[l][-m][-l+1];
        }

        // The Interior..
        // Centre element
        d[l][0][0] = 1.0 / l * ((2 * l - 1) * c * d[l-1][0][0] - (l - 1) * d[l-2][0][0]);

        signm = 1;
        for (m = 1; m < l - 1; m++) {
            signm = -signm; /* signm := (-1)^m */
            // Horizontal cross elements...
            d[l][0][m] = 1 / sqrt((float)l * l - m * m) * ((2 * l - 1) * c * d[l-1][0][m] - sqrt((float)(l - 1) * (l - 1) - m * m) * d[l-2][0][m]);
            d[l][m][0] = signm * d[l][0][m];
            // Diagonal elements...
            d[l][m][m]   = -l * (2 * l - 1.0) / (l * l - m * m) * (-c * d[l-1][m][m] + m * m / (float)(l * (l - 1)) * d[l-1][-m][-m] + ((l - 1) * (l - 1) - m * m) / ((l - 1) * (2 * l - 1.0)) * d[l-2][m][m]);
            d[l][-m][-m] = -l * (2 * l - 1.0) / (l * l - m * m) * (-c * d[l-1][-m][-m] + m * m / (float)(l * (l - 1)) * d[l-1][m][m] + ((l - 1) * (l - 1) - m * m) / ((l - 1) * (2 * l - 1.0)) * d[l-2][-m][-m]);

            // All the rest...
            signmn = signm;
            for (n = 1; n < m; n++) {
                signmn = -signmn; /* signmn := (-1)^(m-n) */
                d[l][m][n]   = -l * (2 * l - 1) / sqrt((float)(l * l - m * m) * (l * l - n * n)) * (-c * d[l-1][m][n] + m * n / (float)(l * (l - 1)) * d[l-1][-m][-n] + sqrt((float)((l - 1) * (l - 1) - m * m) * ((l - 1) * (l - 1) - n * n)) / ((l - 1) * (2 * l - 1)) * d[l-2][m][n]);
                d[l][-m][-n] = -l * (2 * l - 1) / sqrt((float)(l * l - m * m) * (l * l - n * n)) * (-c * d[l-1][-m][-n] + m * n / (float)(l * (l - 1)) * d[l-1][m][n] + sqrt((float)((l - 1) * (l - 1) - m * m) * ((l - 1) * (l - 1) - n * n)) / ((l - 1) * (2 * l - 1)) * d[l-2][-m][-n]);
                d[l][n][m]   = signmn * d[l][m][n];
                d[l][-n][-m] = signmn * d[l][-m][-n];
            } //end n-loop
        } //end m-loop
    } //end l-loop
    return;
}

void shEvalWignerYZ(ShWigner D, float beta, float gamm, int lMax) {
    // Calculate WignerY(beta) . WignerZ(gamma)
    // n.b. beta, gamma in *RADIANS*!!
    float temp, c, s;
    int l, m, n;

    // First get the WignerY matrices...
    shEvalRealWigner(D, beta, lMax);

    // Nothing to do if deg 0...
    if (lMax == 0) return;

    /* Initialise array of cos(n*gamma), sin (n*gamma) values */
    c = cos(gamm);
    s = sin(gamm);
    cm[1] = c;
    sm[1] = s;
    for (l = 2; l <= lMax; l++) {
        cm[l] = c * cm[l-1] - s * sm[l-1];
        sm[l] = s * cm[l-1] + c * sm[l-1];
    }

    /* Now loop over elements to do post-mult by WignerZ(gamma)... */
    for (l = 1; l <= lMax; l++) {
        for (m = 1; m <= l; m++) {
            // Col n=0 of wigner is unchanged, but do row m=0.
            D[l][0][-m] = -sm[m] * D[l][0][m];
            D[l][0][m]  =  cm[m] * D[l][0][m];
            for (n = 1; n <= l; n++) {
                temp        =  cm[n] * D[l][m][n];
                D[l][m][-n] = -sm[n] * D[l][m][n];
                D[l][m][n]  = temp;
                temp        = cm[n] * D[l][-m][-n];
                D[l][-m][n] =  sm[n] * D[l][-m][-n];
                D[l][-m][-n] = temp;
            }
        }
    }
    return;
}



void shPostMultiplyByRZ(ShWigner D, float alpha, int lMax) {
    // In-place post-multiplication of wigner by z-rotation through alpha.
    // n.b. alpha in *RADIANS*!!
    float temp1, c, s;
    int l, m, n;

    if (lMax >= 1) {
        c = cos(alpha);
        s = sin(alpha);
        cm[1] = c;
        sm[1] = s;
    }
    for (l = 2; l <= lMax; l++) {
        cm[l] = c * cm[l-1] - s * sm[l-1];
        sm[l] = s * cm[l-1] + c * sm[l-1];
    }
    for (l = 0; l <= lMax; l++) {
        for (m = -l; m <= l; m++) {
            // Col n=0 of wigner is unchanged.
            for (n = 1; n <= l; n++) {
                temp1 =  D[l][m][n] * cm[n] + D[l][m][-n] * sm[n];
                D[l][m][-n] = -D[l][m][n] * sm[n] + D[l][m][-n] * cm[n];
                D[l][m][n]  = temp1;
            }
        }
    }
    return;
}


void shPreMultiplyByRZ(ShWigner D, float alpha, int lMax) {
    // In-place pre-multiplication of wigner by z-rotation through alpha.
    // n.b. alpha in *RADIANS*!!
    float temp1, c, s;
    int l, m, n;

    if (lMax >= 1) {
        c = cos(alpha);
        s = sin(alpha);
        cm[1] = c;
        sm[1] = s;
    }
    for (l = 2; l <= lMax; l++) {
        cm[l] = c * cm[l-1] - s * sm[l-1];
        sm[l] = s * cm[l-1] + c * sm[l-1];
    }
    for (l = 0; l <= lMax; l++) {
        for (m = 1; m <= l; m++) {
            // Row m=0 of wigner is unchanged.
            for (n = -l; n <= l; n++) {
                temp1 =  D[l][m][n] * cm[m] - D[l][-m][n] * sm[m];
                D[l][-m][n] =  D[l][m][n] * sm[m] + D[l][-m][n] * cm[m];
                D[l][m][n]  = temp1;
            }
        }
    }
    return;
}


void shRotateCoeffs(ShWigner D, int lMax, Sh a, Sh aprime) {
    /* Multiply spherical harmonic coefficient vector by
     * a Wigner matrix, returning the result in another SH
     * coefficient vector. */
    int l, m, n;
    for (l = 0; l <= lMax; l++) {
        for (m = -l; m <= l; m++) {
            aprime[l][m] = 0.0;
            for (n = -l; n <= l; n++) {
                aprime[l][m] += D[l][m][n] * a[l][n];
            }
        }
    }
    return;
}

void shRotateFloatCoeffs(ShWigner D, int lMax, float *a, float* aprime) {
    /* Multiply spherical harmonic coefficient vector by
     * a Wigner matrix, returning the result in another SH
     * coefficient vector. */
    int l, m, n;
    for (l = 0; l <= lMax; l++) {
        for (m = -l; m <= l; m++) {
			int single_index = l * (l+1) + m;
            aprime[single_index] = 0.0;
            for (n = -l; n <= l; n++) {
                aprime[single_index] += D[l][m][n] * a[l * (l+1) + n];
            }
        }
    }
    return;
}


void shRotateVecCoeffs(ShWigner D, int lMax, ShVec a, ShVec aprime) {
    /* Multiply spherical harmonic coefficient vector by
     * a Wigner matrix, returning the result in another SH
     * coefficient vector. */
    int l, m, n;
    for (l = 0; l <= lMax; l++) {
        for (m = -l; m <= l; m++) {
            v3Zero(aprime[l][m]);
            for (n = -l; n <= l; n++)
                v3Accumulate(D[l][m][n], a[l][n], aprime[l][m]);
        }
    }
    return;
}

void shRotToWigner(const float R[16], ShWigner wignerD, const int lMax) {
    // Construct a Wigner matrix from a 4x4 rotation R.
    EulerAngles euler;

    if (lMax > maxDegree) shUpdateMaxDegree(lMax);

    v3ZyzEulerAngles(R, &euler);
    shEvalWignerYZ(wignerD, euler.beta, euler.gamma, lMax);
    shPreMultiplyByRZ(wignerD, euler.alpha, lMax);

    return;
}

void shEulerToWigner(EulerAngles &euler, ShWigner wignerD, const int lMax) {
    if (lMax > maxDegree) shUpdateMaxDegree(lMax);
    shEvalWignerYZ(wignerD, euler.beta, euler.gamma, lMax);
    shPreMultiplyByRZ(wignerD, euler.alpha, lMax);
}

void shRotateZhCoeffs(vector3 u, const int lMax, Zh a, Sh aprime){
    int l, m;
	// Evaluate basis functions in given direction
	aprime = shY(u, lMax, aprime);

	for (l = 0; l <= lMax; l++)
		for (m = -l; m <= l; m++)
			aprime[l][m] *= a[l] * sqrt(4.0 * osg::PI / (2*l + 1));

	return;
}

void shRotateZhVecCoeffs(vector3 u, const int lMax, ZhVec a, ShVec aprime){
    int l, m;
	// This is rather inefficient, but it's just for experimenting
    Sh Y = shNewSh(lMax);
	// Evaluate basis functions in given direction
	Y = shY(u, lMax, Y);

	for (l = 0; l <= lMax; l++){
		for (m = -l; m <= l; m++){
			const float c = Y[l][m] * sqrt(4.0 * osg::PI / (2*l + 1));
			aprime[l][m][0] = a[l][0] * c;
			aprime[l][m][1] = a[l][1] * c;
			aprime[l][m][2] = a[l][2] * c;
		}
	}
	return;
}