#include <stdlib.h>

/**
 * @brief Computes the quotient and remainder of the division of two integers.
 *
 * This function takes two integers, `num` and `den`, and returns a `div_t`
 * structure containing the quotient and remainder of the division of `num` by `den`.
 *
 * @param num The numerator (dividend).
 * @param den The denominator (divisor).
 * @return A `div_t` structure where:
 *         - `quot` is the quotient of the division.
 *         - `rem` is the remainder of the division.
 */
div_t div(int num, int den)
{
	return (div_t){num / den, num % den};
}
