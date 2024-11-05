#define _BSD_SOURCE
#include <stdlib.h>

/**
 * @typedef cmpfun
 * @brief A function pointer type for comparison functions.
 *
 * This type defines a function pointer for comparison functions used in sorting algorithms.
 * The comparison function takes two `const void *` arguments and returns an `int`.
 * The return value should be negative if the first argument is less than the second,
 * zero if they are equal, and positive if the first argument is greater than the second.
 *
 * @param a Pointer to the first element to compare.
 * @param b Pointer to the second element to compare.
 * @return An integer less than, equal to, or greater than zero if the first argument is
 *         considered to be respectively less than, equal to, or greater than the second.
 */
typedef int (*cmpfun)(const void *, const void *);

/**
 * @brief A wrapper function for comparing two elements using a custom comparison function.
 *
 * This function takes two void pointers to the elements to be compared and a pointer to the
 * comparison function. It casts the comparison function pointer to the appropriate type and
 * calls it with the two elements.
 *
 * @param v1 Pointer to the first element to be compared.
 * @param v2 Pointer to the second element to be compared.
 * @param cmp Pointer to the comparison function.
 * @return An integer less than, equal to, or greater than zero if the first element is considered
 *         to be respectively less than, equal to, or greater than the second element.
 */
static int wrapper_cmp(const void *v1, const void *v2, void *cmp)
{
	return ((cmpfun)cmp)(v1, v2);
}

/**
 * @brief Sorts an array using the quicksort algorithm.
 *
 * This function sorts an array of `nel` elements, each of `width` bytes,
 * using the quicksort algorithm. The comparison function `cmp` is used
 * to determine the order of the elements.
 *
 * @param base Pointer to the array to be sorted.
 * @param nel Number of elements in the array.
 * @param width Size of each element in bytes.
 * @param cmp Comparison function that determines the order of the elements.
 *            It should return a negative value if the first argument is less
 *            than the second, zero if they are equal, and a positive value
 *            if the first argument is greater than the second.
 */
void qsort(void *base, size_t nel, size_t width, cmpfun cmp)
{
	__qsort_r(base, nel, width, wrapper_cmp, (void *)cmp);
}
