#include <stdlib.h>

/**
 * @brief Computes the quotient and remainder of the division of two long integers.
 *
 * This function takes two long integers, `num` and `den`, and returns a structure
 * of type `ldiv_t` containing the quotient and remainder of the division of `num` by `den`.
 *
 * @param num The numerator (dividend).
 * @param den The denominator (divisor).
 * @return A structure of type `ldiv_t` containing the quotient and remainder.
 */
ldiv_t ldiv(long num, long den)
{
	return (ldiv_t){num / den, num % den};
}
