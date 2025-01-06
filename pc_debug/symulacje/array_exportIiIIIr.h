/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 24.1 and Signal Processing Toolbox 24.1.
 * Generated on: 05-Jan-2025 14:27:45
 */

/*
 * Discrete-Time IIR Filter (real)
 * -------------------------------
 * Filter Structure    : Direct-Form II, Second-Order Sections
 * Number of Sections  : 27
 * Stable              : Yes
 * Linear Phase        : No
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * C:\Program Files\MATLAB\R2024a\extern\include\tmwtypes.h 
 */
#define MWSPT_NSEC 55
const int NL[MWSPT_NSEC] = { 1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,
3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,2,1 };
const real64_T NUM[MWSPT_NSEC][3] = {
  {
    0.03867851905137,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03782070254629,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03700434268363,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03622922618167,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03549494823402,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03480095364585,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
     0.0341465726185,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03353105161849,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03295357979889,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03241331144694,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03190938492004,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03144093850832,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03100712363083,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03060711573699,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.03024012324812,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02990539483733,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02960222531068,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02932996031959,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02908800010376,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02887580243607,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02869288491573,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
     0.0285388267332,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02841327001046,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02831592080166,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02824654982334,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
    0.02820499296843,                 0,                 0 
  },
  {
                   1,                 2,                 1 
  },
  {
      0.167902208577,                 0,                 0 
  },
  {
                   1,                 1,                 0 
  },
  {
                   1,                 0,                 0 
  }
};
const int DL[MWSPT_NSEC] = { 1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,
3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,2,1 };
const real64_T DEN[MWSPT_NSEC][3] = {
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.822564883367,    0.977278959573 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.782143836314,   0.9334266464992 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.743676261692,   0.8916936324263 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.707152109485,   0.8520690142113 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.672552304868,   0.8145320978043 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.639850686589,    0.779054501172 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,    -1.60901569316,   0.7456019836338 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,    -1.58001181744,   0.7141360239139 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.552800851629,   0.6846151708241 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.527342945018,   0.6569961908054 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.503597496271,   0.6312350359513 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.481523900883,   0.6072876549166 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.461082172994,   0.5851106675178 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.442233459076,   0.5646619220241 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.424940459265,    0.545900952257 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.409167770394,   0.5287893497433 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.394882163123,    0.513291064366 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.382052803988,   0.4993726452663 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,    -1.37065143177,   0.4870034321849 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.360652496264,   0.4761557060083 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.352033266332,   0.4668048059948 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.344773913069,   0.4589292200021 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.338857572957,   0.4525106529991 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.334270395016,   0.4475340782231 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.331001575214,   0.4439877745072 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,   -1.329043380684,   0.4418633525576 
  },
  {
                   1,                 0,                 0 
  },
  {
                   1,  -0.6641955828461,                 0 
  },
  {
                   1,                 0,                 0 
  }
};
