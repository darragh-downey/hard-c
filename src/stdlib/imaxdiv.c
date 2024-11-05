#include <inttypes.h>

/**
 * @brief Computes the quotient and remainder of the division of two integers.
 *
 * This function takes two integers, `num` and `den`, and returns a structure
 * of type `imaxdiv_t` containing the quotient and remainder of the division
 * of `num` by `den`.
 *
 * @param num The numerator (dividend).
 * @param den The denominator (divisor).
 * @return A structure of type `imaxdiv_t` containing the quotient and remainder.
 */
imaxdiv_t imaxdiv(intmax_t num, intmax_t den)
{
	return (imaxdiv_t){ num/den, num%den };
}
