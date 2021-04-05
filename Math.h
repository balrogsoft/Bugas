
#include <exec/execbase.h>
#include <exec/memory.h>  

#define FIXED_POINT 1

#define FP_TYPE LONG

#define FP 10
#define FP_UNIT 1024
#define FP_2D_UNIT5 320
#define FP_2D 6
#define FP_2D_UNIT 64

//#define IEEESP 1

#ifdef IEEESP

#define float FLOAT
#define int LONG
#include <libraries/mathieeesp.h>
#include <clib/mathieeesingbas_protos.h>
#include <clib/mathieeesingtrans_protos.h>
#define fix IEEESPFix
#define flt IEEESPFlt

#else

#include <libraries/mathffp.h>
#include <clib/mathffp_protos.h> 
#include <clib/mathtrans_protos.h>
#define fix SPFix
#define flt SPFlt

#endif

#define k_pi            3.14159265358979f
#define k_pi_inverse    0.31830988618379f
#define k_pi_half       1.57079632679f


FP_TYPE fpsin[1024];
FP_TYPE fpcos[1024];

ULONG random_v = 0x9c33fe02;

ULONG random(void)
{
  random_v = (random_v * (ULONG)2654435761);
  return random_v>>16;
}


void initMathLibs(void) {
#ifdef IEEESP
    if (MathIeeeSingBasBase = OpenLibrary("mathieeesingbas.library",34)) 
    {
    }
    
    if (MathIeeeSingTransBase = OpenLibrary("mathieeesingtrans.library",34))
    {
    }
#else
    if (MathBase = OpenLibrary("mathffp.library", 0))
    {
    }
    
    if (MathTransBase = OpenLibrary("mathtrans.library",0))
    {
    }
#endif
}

void closeMathLibs(void) {
    #ifdef IEEESP
    if (MathIeeeSingBasBase != NULL)
        CloseLibrary(MathIeeeSingBasBase);
    if (MathIeeeSingTransBase != NULL) 
        CloseLibrary(MathIeeeSingTransBase);
#else
    if (MathBase != NULL)
        CloseLibrary(MathBase);
    if (MathTransBase != NULL) 
        CloseLibrary(MathTransBase);
#endif
}


void generateSineCosTable(void) {
#ifdef FIXED_POINT
    WORD k;
    FLOAT prc, fsi;
    for (k = 0; k < 1024; k++) {
        prc = 2*k_pi*(k/1024.0);
        fsi = sin(prc);
        fpsin[k] = fsi*FP_UNIT;
        fsi = cos(prc);
        fpcos[k] = fsi*FP_UNIT;
    }
#endif
}

void matrixIdentity(FP_TYPE m[4][4]) {
    m[0][0] = FP_UNIT; m[0][1] = 0;       m[0][2] = 0;       m[0][3] = 0;
    m[1][0] = 0;       m[1][1] = FP_UNIT; m[1][2] = 0;       m[1][3] = 0;
    m[2][0] = 0;       m[2][1] = 0;       m[2][2] = FP_UNIT; m[2][3] = 0;
    m[3][0] = 0;       m[3][1] = 0;       m[3][2] = 0;       m[3][3] = FP_UNIT;
}

void matrixMultiply1(FP_TYPE m1[4], FP_TYPE m2[4][4]) {
    WORD i, k;
    FP_TYPE result[4];
    for(i = 0; i < 4; i++) {
        result[i] = 0;
        for(k = 0; k < 4; k++) {
            result[i] += (m1[k]*m2[i][k])>>FP;
        }
    }
    CopyMem(result, m1, 16);
}

void matrixTranslation(FP_TYPE m[4][4], FP_TYPE tx, FP_TYPE ty, FP_TYPE tz) {
    m[0][3]=tx;
    m[1][3]=ty;
    m[2][3]=tz;
}

void matrixMultiply(FP_TYPE m1[4][4], FP_TYPE m2[4][4]) {
    WORD i, j, k, result[4][4];
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            result[i][j] = 0;
            for(k = 0; k < 4; k++) {
                result[i][j] += (m1[i][k]*m2[k][j]) >> FP;
            }
        }
    }
    CopyMem(result, m1, 64);
}
void matrixScale(FP_TYPE m[4][4], FP_TYPE sx, FP_TYPE sy, FP_TYPE sz) {
    m[0][0] = sx; m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
    m[1][0] = 0;  m[1][1] = sy; m[1][2] = 0;  m[1][3] = 0;
    m[2][0] = 0;  m[2][1] = 0;  m[2][2] = sz; m[2][3] = 0;
    m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = FP_UNIT;
}
void matrixYRotation(FP_TYPE m[4][4], FP_TYPE angle) {
    FP_TYPE sin = fpsin[angle], cos = fpcos[angle];
    
    m[0][0]=cos;
    m[0][2]=sin;
    m[2][0]=-sin;
    m[2][2]=cos;
}

void matrixZRotation(FP_TYPE m[4][4], FP_TYPE angle) {
    FP_TYPE sin = fpsin[angle], cos = fpcos[angle];
    
    m[0][0]=cos;
    m[0][1]=-sin;
    m[1][0]=sin;
    m[1][1]=cos;
}

void matrixXYZRotation(FP_TYPE m[4][4], FP_TYPE x, FP_TYPE y, FP_TYPE z) {
    m[0][0] = (fpcos[y]*fpcos[z])>>FP;
    m[0][1] = (fpcos[y]*fpsin[z])>>FP;
    m[0][2] = -fpsin[y];
    m[1][0] = ((((fpsin[x]*fpsin[y])>>FP)*fpcos[z])>>FP)-((fpcos[x]*fpsin[z])>>FP);
    m[1][1] = ((((fpsin[x]*fpsin[y])>>FP)*fpsin[z])>>FP)+((fpcos[x]*fpcos[z])>>FP);
    m[1][2] = (fpsin[x]*fpcos[y])>>FP;
    m[2][0] = ((((fpcos[x]*fpsin[y])>>FP)*fpcos[z])>>FP)+((fpsin[x]*fpsin[z])>>FP);
    m[2][1] = ((((fpcos[x]*fpsin[y])>>FP)*fpsin[z])>>FP)-((fpsin[x]*fpcos[z])>>FP);
    m[2][2] = ((fpcos[x]*fpcos[y])>>FP);
}


