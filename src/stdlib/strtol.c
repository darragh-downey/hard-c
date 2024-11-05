#include "stdio_impl.h"
#include "intscan.h"
#include "shgetc.h"
#include <inttypes.h>
#include <limits.h>
#include <ctype.h>

/**
 * @brief Converts a string to an unsigned long long integer.
 *
 * This function parses the input string `s` according to the specified `base`
 * and converts it to an unsigned long long integer, with a limit specified by `lim`.
 *
 * @param s The input string to be converted.
 * @param p If non-NULL, a pointer to a pointer that will be set to the character
 *          in the string where the conversion stopped.
 * @param base The numerical base to use for the conversion (e.g., 10 for decimal, 16 for hexadecimal).
 * @param lim The maximum value that can be returned.
 * @return The converted unsigned long long integer.
 */
static unsigned long long strtox(const char *s, char **p, int base, unsigned long long lim)
{
	FILE f;
	sh_fromstring(&f, s);
	shlim(&f, 0);
	unsigned long long y = __intscan(&f, base, 1, lim);
	if (p)
	{
		size_t cnt = shcnt(&f);
		*p = (char *)s + cnt;
	}
	return y;
}

unsigned long long strtoull(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, ULLONG_MAX);
}

long long strtoll(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, LLONG_MIN);
}

unsigned long strtoul(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, ULONG_MAX);
}

long strtol(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, 0UL + LONG_MIN);
}

intmax_t strtoimax(const char *restrict s, char **restrict p, int base)
{
	return strtoll(s, p, base);
}

uintmax_t strtoumax(const char *restrict s, char **restrict p, int base)
{
	return strtoull(s, p, base);
}

weak_alias(strtol, __strtol_internal);
weak_alias(strtoul, __strtoul_internal);
weak_alias(strtoll, __strtoll_internal);
weak_alias(strtoull, __strtoull_internal);
weak_alias(strtoimax, __strtoimax_internal);
weak_alias(strtoumax, __strtoumax_internal);
