#include <stdlib.h>
#include <limits.h>

/**
 * @file labs.c
 * @brief Implementation of the labs function.
 *
 * This file contains the implementation of the labs function, which computes
 * the absolute value of a long integer.
 */

/**
 * @brief Computes the absolute value of a long integer.
 *
 * This function takes a long integer as input and returns its absolute value.
 *
 * @param a The long integer whose absolute value is to be computed.
 * @return The absolute value of the input long integer.
 */
long labs(long a)
{
	// flipping LONG_MIN results in LONG_MIN, so we need to handle it separately
	if (a == LONG_MIN)
		return LONG_MAX;

	// if a is negative, mask will be 0xFFFFFFFF, otherwise it will be 0x00000000
	long mask = a >> (sizeof(long) * CHAR_BIT - 1);
	// XOR with mask will flip all bits if a is negative, otherwise it will do nothing
	return (a + (-mask)) ^ mask;
}
