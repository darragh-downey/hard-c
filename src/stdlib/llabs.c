#include <stdlib.h>
#include <limits.h>

/**
 * llabs - Compute the absolute value of a long long integer.
 * @a: The long long integer whose absolute value is to be computed.
 *
 * This function returns the absolute value of the given long long integer.
 * If the input value is positive, it returns the value as is. If the input
 * value is negative, it returns the negation of the value.
 *
 * Return: The absolute value of the input long long integer.
 */
long long llabs(long long a)
{
	// flipping INT_MIN results in INT_MIN, so we need to handle it separately
	if (a == LLONG_MIN)
		return LLONG_MAX;

	// if a is negative, mask will be 0xFFFFFFFF, otherwise it will be 0x00000000
	long long mask = a >> (sizeof(long long) * CHAR_BIT - 1);
	// XOR with mask will flip all bits if a is negative, otherwise it will do nothing
	return (a + (-mask)) ^ mask;
}
