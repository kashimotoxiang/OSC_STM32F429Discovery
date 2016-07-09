#include "FFT.h"

static arm_cfft_radix4_instance_q15 scfft;

void FFT_Conv (q15_t* fft_outputbuf, q15_t* fft_inputbuf, int FFT_LENGTH) {
	arm_cfft_radix4_init_q15(&scfft, FFT_LENGTH, 0, 1);//初始化结构体，设定FFT参数
	arm_cfft_radix4_q15(&scfft, fft_inputbuf); //FFT计算
	arm_cmplx_mag_q15(fft_inputbuf, fft_outputbuf, FFT_LENGTH); //把运算结果求复数求得模值
}

