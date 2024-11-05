#include <stdlib.h>
#include <ctype.h>

/**
 * @brief Converts a string to an integer.
 *
 * This function takes a string representing a decimal number and converts it
 * to an integer. It handles optional leading whitespace and an optional sign
 * character ('+' or '-'). The conversion stops at the first non-digit character.
 *
 * @param s Pointer to the null-terminated string to be converted.
 * @return The integer value represented by the string. If the string does not
 *         contain a valid integer, the result is undefined.
 */
int atoi(const char *s)
{
	int n = 0, neg = 0;
	while (isspace(*s))
		s++;
	switch (*s)
	{
	case '-':
		neg = 1;
	case '+':
		s++;
	}
	/* Compute n as a negative number to avoid overflow on INT_MIN */
	while (isdigit(*s))
		n = 10 * n - (*s++ - '0');
	return neg ? n : -n;
}
