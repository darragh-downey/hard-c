#include <stdlib.h>

/**
 * @file lldiv.c
 * @brief Provides the lldiv function for performing division and modulus operations on long long integers.
 *
 * This function takes two long long integers, performs integer division and modulus operations,
 * and returns the results in a structure of type lldiv_t.
 *
 * @param num The numerator of the division operation.
 * @param den The denominator of the division operation.
 * @return A structure of type lldiv_t containing the quotient and remainder of the division operation.
 */
lldiv_t lldiv(long long num, long long den)
{
	return (lldiv_t){ num/den, num%den };
}
