#include <stdlib.h>

/**
 * @brief Converts a string to a double.
 *
 * This function takes a null-terminated string representing a floating-point
 * number and converts it to a double-precision floating-point value.
 *
 * @param s A pointer to the null-terminated string to be converted.
 * @return The converted double-precision floating-point value.
 */
double atof(const char *s)
{
	return strtod(s, 0);
}
