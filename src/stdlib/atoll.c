#include <stdlib.h>
#include <ctype.h>

/**
 * @file atoll.c
 * @brief Implementation of the atoll function to convert a string to a long long integer.
 *
 * This function converts the initial portion of the string pointed to by s to long long integer representation.
 * It skips any leading whitespace characters, then takes an optional '+' or '-' sign, and converts the subsequent
 * characters to a long long integer until a non-digit character is encountered.
 *
 * @param s Pointer to the null-terminated string to be converted.
 * @return The converted long long integer value. If the string represents a negative number, the result is negative.
 *         If the string does not contain any digits, the result is 0.
 */
long long atoll(const char *s)
{
	long long n = 0;
	int neg = 0;
	while (isspace(*s))
		s++;
	switch (*s)
	{
	case '-':
		neg = 1;
	case '+':
		s++;
	}
	/* Compute n as a negative number to avoid overflow on LLONG_MIN */
	while (isdigit(*s))
		n = 10 * n - (*s++ - '0');
	return neg ? n : -n;
}
