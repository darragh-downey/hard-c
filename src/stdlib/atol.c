#include <stdlib.h>
#include <ctype.h>

/**
 * @file atol.c
 * @brief Converts a string to a long integer.
 *
 * This function takes a string representing a number and converts it to a
 * long integer. It handles optional leading whitespace and an optional
 * sign ('+' or '-'). The conversion stops at the first non-digit character.
 *
 * @param s The string to be converted.
 * @return The converted long integer. If the string represents a negative
 *         number, the result will be negative.
 */
long atol(const char *s)
{
	long n = 0;
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
	/* Compute n as a negative number to avoid overflow on LONG_MIN */
	while (isdigit(*s))
		n = 10 * n - (*s++ - '0');
	return neg ? n : -n;
}
