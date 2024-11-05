#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Converts a floating-point number to a string.
 *
 * This function converts the given floating-point number `x` to a string
 * representation with `n` significant digits and stores the result in the
 * buffer `b`.
 *
 * @param x The floating-point number to convert.
 * @param n The number of significant digits to include in the output.
 * @param b The buffer to store the resulting string. It should be large enough
 *          to hold the resulting string.
 * @return A pointer to the buffer `b` containing the string representation of `x`.
 */
char *gcvt(double x, int n, char *b)
{
	// Estimate the maximum length of the resulting string
	size_t max_len = n + 7; // n significant digits + "e+XXXX" + null terminator

	// Ensure the buffer is large enough
	if (snprintf(NULL, 0, "%.*g", n, x) >= max_len)
	{
		// Buffer is too small
		return NULL;
	}

	sprintf(b, "%.*g", n, x);
	return b;
}
