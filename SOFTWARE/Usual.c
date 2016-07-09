#include "Usual.h"
#include <cstdarg>
#include <stdarg.h>
/*-------------------------------------------------------*/
char TemptString[10] = {0};//数字字符串转换数组
//---------------------------------------------------------------------//
void Bubble_Sort (uint8_t volatile* num, int n)
{
	static int32_t i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; i + j < n - 1; j++) {
			if (num[j] > num[j + 1]) {
				uint8_t temp = num[j];
				num[j] = num[j + 1];
				num[j + 1] = temp;
			}
		}
	}
	return;
}
