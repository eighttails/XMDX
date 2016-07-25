#if !defined(__FMXDRVG_DOWNSAMPLE_GLOBAL_H__)
#define __FMXDRVG_DOWNSAMPLE_GLOBAL_H__

#if defined(__GNUC__)
 #pragma GCC diagnostic push
 #pragma GCC diagnostic ignored "-Wmissing-braces"
 #define ALIGNED(x) __attribute__ ((aligned (x)))
#elif defined(_MSC_VER)
 #define ALIGNED(x) __declspec(align(x))
#else
 #define ALIGNED(x)
#endif

#define LPF_ROW_44 441
ALIGNED(8) static int16 LOWPASS_44[LPF_ROW_44][LPF_COL] = {
	#include "lowpass_44.dat"
};

#define LPF_ROW_48 96
ALIGNED(8) static int16 LOWPASS_48[LPF_ROW_48][LPF_COL] = {
	#include "lowpass_48.dat"
};

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

static int LPF_ROW = LPF_ROW_44;
static int16 *LOWPASS = (int16 *)LOWPASS_44;


inline void FirMatrix(sint16 *p, sint16 *buf0, sint16 *buf1, sint32 *result) {
	sint32 r;
	r =  (int)buf0[0]*p[0]
	    +(int)buf0[1]*p[1]
	    +(int)buf0[2]*p[2]
	    +(int)buf0[3]*p[3]
	    +(int)buf0[4]*p[4]
	    +(int)buf0[5]*p[5]
	    +(int)buf0[6]*p[6]
	    +(int)buf0[7]*p[7]
	    +(int)buf0[8]*p[8]
	    +(int)buf0[9]*p[9]
	    +(int)buf0[10]*p[10]
	    +(int)buf0[11]*p[11]
	    +(int)buf0[12]*p[12]
	    +(int)buf0[13]*p[13]
	    +(int)buf0[14]*p[14]
	    +(int)buf0[15]*p[15]
	    +(int)buf0[16]*p[16]
	    +(int)buf0[17]*p[17]
	    +(int)buf0[18]*p[18]
	    +(int)buf0[19]*p[19]
	    +(int)buf0[20]*p[20]
	    +(int)buf0[21]*p[21]
	    +(int)buf0[22]*p[22]
	    +(int)buf0[23]*p[23]
	    +(int)buf0[24]*p[24]
	    +(int)buf0[25]*p[25]
	    +(int)buf0[26]*p[26]
	    +(int)buf0[27]*p[27]
	    +(int)buf0[28]*p[28]
	    +(int)buf0[29]*p[29]
	    +(int)buf0[30]*p[30]
	    +(int)buf0[31]*p[31]
	    +(int)buf0[32]*p[32]
	    +(int)buf0[33]*p[33]
	    +(int)buf0[34]*p[34]
	    +(int)buf0[35]*p[35]
	    +(int)buf0[36]*p[36]
	    +(int)buf0[37]*p[37]
	    +(int)buf0[38]*p[38]
	    +(int)buf0[39]*p[39]
	    +(int)buf0[40]*p[40]
	    +(int)buf0[41]*p[41]
	    +(int)buf0[42]*p[42]
	    +(int)buf0[43]*p[43]
	    +(int)buf0[44]*p[44]
	    +(int)buf0[45]*p[45]
	    +(int)buf0[46]*p[46]
	    +(int)buf0[47]*p[47]
	    +(int)buf0[48]*p[48]
	    +(int)buf0[49]*p[49]
	    +(int)buf0[50]*p[50]
	    +(int)buf0[51]*p[51]
	    +(int)buf0[52]*p[52]
	    +(int)buf0[53]*p[53]
	    +(int)buf0[54]*p[54]
	    +(int)buf0[55]*p[55]
	    +(int)buf0[56]*p[56]
	    +(int)buf0[57]*p[57]
	    +(int)buf0[58]*p[58]
	    +(int)buf0[59]*p[59]
	    +(int)buf0[60]*p[60]
	    +(int)buf0[61]*p[61]
	    +(int)buf0[62]*p[62]
	    +(int)buf0[63]*p[63];
	result[0] = r >> (16-1);
	r =  (int)buf1[0]*p[0]
	    +(int)buf1[1]*p[1]
	    +(int)buf1[2]*p[2]
	    +(int)buf1[3]*p[3]
	    +(int)buf1[4]*p[4]
	    +(int)buf1[5]*p[5]
	    +(int)buf1[6]*p[6]
	    +(int)buf1[7]*p[7]
	    +(int)buf1[8]*p[8]
	    +(int)buf1[9]*p[9]
	    +(int)buf1[10]*p[10]
	    +(int)buf1[11]*p[11]
	    +(int)buf1[12]*p[12]
	    +(int)buf1[13]*p[13]
	    +(int)buf1[14]*p[14]
	    +(int)buf1[15]*p[15]
	    +(int)buf1[16]*p[16]
	    +(int)buf1[17]*p[17]
	    +(int)buf1[18]*p[18]
	    +(int)buf1[19]*p[19]
	    +(int)buf1[20]*p[20]
	    +(int)buf1[21]*p[21]
	    +(int)buf1[22]*p[22]
	    +(int)buf1[23]*p[23]
	    +(int)buf1[24]*p[24]
	    +(int)buf1[25]*p[25]
	    +(int)buf1[26]*p[26]
	    +(int)buf1[27]*p[27]
	    +(int)buf1[28]*p[28]
	    +(int)buf1[29]*p[29]
	    +(int)buf1[30]*p[30]
	    +(int)buf1[31]*p[31]
	    +(int)buf1[32]*p[32]
	    +(int)buf1[33]*p[33]
	    +(int)buf1[34]*p[34]
	    +(int)buf1[35]*p[35]
	    +(int)buf1[36]*p[36]
	    +(int)buf1[37]*p[37]
	    +(int)buf1[38]*p[38]
	    +(int)buf1[39]*p[39]
	    +(int)buf1[40]*p[40]
	    +(int)buf1[41]*p[41]
	    +(int)buf1[42]*p[42]
	    +(int)buf1[43]*p[43]
	    +(int)buf1[44]*p[44]
	    +(int)buf1[45]*p[45]
	    +(int)buf1[46]*p[46]
	    +(int)buf1[47]*p[47]
	    +(int)buf1[48]*p[48]
	    +(int)buf1[49]*p[49]
	    +(int)buf1[50]*p[50]
	    +(int)buf1[51]*p[51]
	    +(int)buf1[52]*p[52]
	    +(int)buf1[53]*p[53]
	    +(int)buf1[54]*p[54]
	    +(int)buf1[55]*p[55]
	    +(int)buf1[56]*p[56]
	    +(int)buf1[57]*p[57]
	    +(int)buf1[58]*p[58]
	    +(int)buf1[59]*p[59]
	    +(int)buf1[60]*p[60]
	    +(int)buf1[61]*p[61]
	    +(int)buf1[62]*p[62]
	    +(int)buf1[63]*p[63];
	result[1] = r >> (16-1);
}

#endif  // __FMXDRVG_DOWNSAMPLE_GLOBAL_H__

// [EOF]
