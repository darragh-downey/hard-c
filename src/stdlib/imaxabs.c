#include <inttypes.h>
#include <limits.h>

/**
 * @file imaxabs.c
 * @brief Provides a function to compute the absolute value of an integer of type intmax_t.
 *
 * This function returns the absolute value of the given integer.
 */

/**
 * @brief Computes the absolute value of an integer of type intmax_t.
 *
 * This function takes an integer of type intmax_t as input and returns its absolute value.
 *
 * @param a The integer whose absolute value is to be computed.
 * @return The absolute value of the input integer.
 */
intmax_t imaxabs(intmax_t a)
{
	// flipping INTMAX_MIN results in INTMAX_MIN, so we need to handle it separately
	if (a == INTMAX_MIN)
		return INTMAX_MIN;

	// if a is negative, mask will be 0xFFFFFFFF, otherwise it will be 0x00000000
	intmax_t mask = a >> (sizeof(intmax_t) * CHAR_BIT - 1);
	// XOR with mask will flip all bits if a is negative, otherwise it will do nothing
	return (a + (-mask)) ^ mask;
}
