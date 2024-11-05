#include <stdlib.h>
#include "shgetc.h"
#include "floatscan.h"
#include "stdio_impl.h"

/**
 * @brief Converts a string to a long double.
 *
 * This function parses the input string `s` and converts it to a long double
 * value. It uses an internal file structure to perform the conversion.
 *
 * @param s The input string to be converted.
 * @param p If not NULL, it points to the character after the last character used in the conversion.
 * @param prec The precision of the conversion (e.g., float, double, or long double).
 * @return The converted long double value.
 */
static long double strtox(const char *s, char **p, int prec)
{
	FILE f;
	sh_fromstring(&f, s);
	shlim(&f, 0);
	long double y = __floatscan(&f, prec, 1);
	off_t cnt = shcnt(&f);
	if (p)
		*p = cnt ? (char *)s + cnt : (char *)s;
	return y;
}

/**
 * @brief Converts a string to a float.
 *
 * This function converts the initial portion of the string pointed to by `s`
 * to a floating-point number (type float). The conversion is done according
 * to the current locale.
 *
 * @param s A pointer to the null-terminated string to be interpreted.
 * @param p A pointer to a pointer to character. If `p` is not NULL, the
 *          function sets `*p` to point to the character after the last
 *          character used in the conversion.
 * @return The converted value as a float.
 */
float strtof(const char *restrict s, char **restrict p)
{
	return strtox(s, p, 0);
}

/**
 * @brief Converts a string to a double-precision floating-point number.
 *
 * This function converts the initial portion of the string pointed to by `s`
 * to a double-precision floating-point number representation.
 *
 * @param s A pointer to the null-terminated string to be interpreted.
 * @param p A pointer to a pointer to character. If `p` is not NULL, the function
 *          stores the address of the first invalid character in `*p`.
 * @return The converted value as a double-precision floating-point number.
 */
double strtod(const char *restrict s, char **restrict p)
{
	return strtox(s, p, 1);
}

long double strtold(const char *restrict s, char **restrict p)
{
	return strtox(s, p, 2);
}
