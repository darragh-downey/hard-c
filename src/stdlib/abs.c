#include <stdlib.h>
#include <limits.h>

// Safely returns the absolute value of a given integer.
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
