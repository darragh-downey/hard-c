#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>

/**
 * Converts a double to a string in scientific notation.
 *
 * @param x The double value to convert.
 * @param n The number of digits to appear after the decimal point.
 * @param dp Pointer to an integer where the function will store the position of the decimal point.
 * @param sign Pointer to an integer where the function will store the sign of the number (0 for positive, 1 for negative).
 * @return A pointer to a static buffer containing the string representation of the double in scientific notation.
 *
 * Note: The returned buffer is statically allocated and will be overwritten by subsequent calls to this function.
 */
char *ecvt(double x, int n, int *dp, int *sign)
{
	static char buf[16];
	char tmp[32];
	int i, j;

	if (n - 1U > 15)
		n = 15;
	sprintf(tmp, "%.*e", n - 1, x);
	i = *sign = (tmp[0] == '-');
	for (j = 0; tmp[i] != 'e'; j += (tmp[i++] != '.'))
		buf[j] = tmp[i];
	buf[j] = 0;
	*dp = atoi(tmp + i + 1) + 1;

	return buf;
}
