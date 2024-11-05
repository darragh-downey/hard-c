#include <stdlib.h>
#include <limits.h>


/**
 * @file abs.c
 * @brief Implementation of the absolute value function for integers.
 *
 * This file contains the implementation of the abs function, which computes
 * the absolute value of an integer. The function handles the special case
 * where the input is INT_MIN, as flipping INT_MIN results in INT_MIN.
 *
 * @param a The integer value for which the absolute value is to be computed.
 * @return The absolute value of the input integer.
 */
int abs(int a)
{
	// flipping INT_MIN results in INT_MIN, so we need to handle it separately
	if (a == INT_MIN)
		return INT_MAX;

	// if a is negative, mask will be 0xFFFFFFFF, otherwise it will be 0x00000000
	int mask = a >> (sizeof(int) * CHAR_BIT - 1);
	// XOR with mask will flip all bits if a is negative, otherwise it will do nothing
	return (a + (-mask)) ^ mask;
}
