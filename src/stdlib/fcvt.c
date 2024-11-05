#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Converts a floating-point number to a string representation.
 *
 * @param x The floating-point number to convert.
 * @param n The number of digits to appear after the decimal point.
 * @param dp Pointer to an integer where the position of the decimal point will be stored.
 * @param sign Pointer to an integer where the sign of the number will be stored (0 for positive, 1 for negative).
 * @return A pointer to a string representation of the floating-point number.
 *
 * The function converts the floating-point number `x` to a string with `n` digits after the decimal point.
 * The position of the decimal point is stored in the integer pointed to by `dp`.
 * The sign of the number is stored in the integer pointed to by `sign`.
 * If the number of digits `n` is greater than 1400, it is limited to 1400.
 * If the number of digits `n` is less than or equal to the number of leading zeros, a string of zeros is returned.
 * Otherwise, the function calls `ecvt` to perform the conversion.
 */
char *fcvt(double x, int n, int *dp, int *sign)
{
	char tmp[1500];
	int i, lz;

	if (n > 1400U)
		n = 1400;
	sprintf(tmp, "%.*f", n, x);
	i = (tmp[0] == '-');
	if (tmp[i] == '0')
		lz = strspn(tmp + i + 2, "0");
	else
		lz = -(int)strcspn(tmp + i, ".");

	if (n <= lz)
	{
		*sign = i;
		*dp = 1;
		if (n > 14U)
			n = 14;
		return "000000000000000" + 14 - n;
	}

	return ecvt(x, n - lz, dp, sign);
}
