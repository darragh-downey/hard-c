/* Copyright (C) 2011 by Lynn Ochs
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/* Minor changes by Rich Felker for integration in musl, 2011-04-27. */

/* Smoothsort, an adaptive variant of Heapsort.  Memory usage: O(1).
   Run time: Worst case O(n log n), close to O(n) in the mostly-sorted case. */

#define _BSD_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "atomic.h"
#define ntz(x) a_ctz_l((x))

/**
 * @typedef cmpfun
 * @brief A function pointer type for comparison functions used in sorting algorithms.
 *
 * This type defines a pointer to a comparison function that takes three arguments:
 * - Two `const void *` pointers representing the elements to be compared.
 * - A `void *` pointer representing additional user data.
 *
 * The comparison function should return an integer less than, equal to, or greater than zero
 * if the first element is considered to be respectively less than, equal to, or greater than the second element.
 */
typedef int (*cmpfun)(const void *, const void *, void *);

/**
 * @brief Calculate the position number of trailing zeros in a size_t array.
 *
 * This function calculates the position number of trailing zeros in a
 * two-element size_t array. It first checks the number of trailing zeros
 * in the first element. If the result is non-zero, it returns that value.
 * Otherwise, it calculates the number of trailing zeros in the second
 * element, adjusted by the size of size_t in bits, and returns that value
 * if it is not equal to the size of size_t in bits.
 *
 * @param p A two-element array of size_t.
 * @return The position number of trailing zeros.
 */
static inline int pntz(size_t p[2])
{
	int r = ntz(p[0] - 1);
	if (r != 0 || (r = 8 * sizeof(size_t) + ntz(p[1])) != 8 * sizeof(size_t))
	{
		return r;
	}
	return 0;
}

/**
 * @brief Performs a cyclic permutation of an array of pointers to unsigned char arrays.
 *
 * This function takes an array of pointers to unsigned char arrays and performs a cyclic
 * permutation of the elements. The permutation is done in chunks of size `width` bytes.
 *
 * @param width The number of bytes to be cycled in each iteration.
 * @param ar An array of pointers to unsigned char arrays. The array must have space for
 *           `n + 1` elements, where the last element is used as temporary storage.
 * @param n The number of elements in the array to be cycled.
 */
static void cycle(size_t width, unsigned char *ar[], int n)
{
	unsigned char tmp[256];
	size_t l;
	int i;

	if (n < 2)
	{
		return;
	}

	ar[n] = tmp;
	while (width)
	{
		l = sizeof(tmp) < width ? sizeof(tmp) : width;
		memcpy(ar[n], ar[0], l);
		for (i = 0; i < n; i++)
		{
			memcpy(ar[i], ar[i + 1], l);
			ar[i] += l;
		}
		width -= l;
	}
}

/**
 * @brief Perform a left shift on a 128-bit integer represented by two size_t values.
 *
 * This function shifts the 128-bit integer represented by the array `p` to the left by `n` bits.
 * The integer is stored in `p` as two size_t values, with `p[0]` being the lower part and `p[1]`
 * being the upper part.
 *
 * @param p An array of two size_t values representing the 128-bit integer to be shifted.
 * @param n The number of bits to shift the integer to the left. Must be greater than 0.
 */
/* shl() and shr() need n > 0 */
static inline void shl(size_t p[2], int n)
{
	if (n >= 8 * sizeof(size_t))
	{
		n -= 8 * sizeof(size_t);
		p[1] = p[0];
		p[0] = 0;
	}
	p[1] <<= n;
	p[1] |= p[0] >> (sizeof(size_t) * 8 - n);
	p[0] <<= n;
}

/**
 * @brief Performs a bitwise right shift on a pair of size_t values.
 *
 * This function shifts the bits of the two-element array `p` to the right by `n` positions.
 * If `n` is greater than or equal to the number of bits in a size_t, the function adjusts
 * the shift amount and moves the higher-order bits from `p[1]` to `p[0]`.
 *
 * @param p A two-element array of size_t values to be shifted.
 * @param n The number of positions to shift the bits to the right.
 */
static inline void shr(size_t p[2], int n)
{
	if (n >= 8 * sizeof(size_t))
	{
		n -= 8 * sizeof(size_t);
		p[0] = p[1];
		p[1] = 0;
	}
	p[0] >>= n;
	p[0] |= p[1] << (sizeof(size_t) * 8 - n);
	p[1] >>= n;
}

/**
 * @brief Sifts down the heap to restore the heap property.
 *
 * This function is used in the heapsort algorithm to maintain the heap property
 * by sifting down the element at the given position. It compares the element
 * with its children and swaps it with the larger child if necessary.
 *
 * @param head Pointer to the current element in the heap.
 * @param width Size of each element in the heap.
 * @param cmp Comparison function used to compare elements.
 * @param arg Additional argument passed to the comparison function.
 * @param pshift Current position shift in the heap.
 * @param lp Array of sizes of left subtrees.
 */
static void sift(unsigned char *head, size_t width, cmpfun cmp, void *arg, int pshift, size_t lp[])
{
	unsigned char *rt, *lf;
	unsigned char *ar[14 * sizeof(size_t) + 1];
	int i = 1;

	ar[0] = head;
	while (pshift > 1)
	{
		rt = head - width;
		lf = head - width - lp[pshift - 2];

		if (cmp(ar[0], lf, arg) >= 0 && cmp(ar[0], rt, arg) >= 0)
		{
			break;
		}
		if (cmp(lf, rt, arg) >= 0)
		{
			ar[i++] = lf;
			head = lf;
			pshift -= 1;
		}
		else
		{
			ar[i++] = rt;
			head = rt;
			pshift -= 2;
		}
	}
	cycle(width, ar, i);
}

/**
 * @brief Performs the trinkle operation as part of the smoothsort algorithm.
 *
 * This function is used internally by the smoothsort algorithm to maintain the
 * heap property of the array being sorted. It adjusts the position of the
 * element at the given head pointer by comparing it with its "stepson" and
 * potentially other elements, and then performing necessary swaps.
 *
 * @param head Pointer to the current element in the array.
 * @param width Size of each element in the array.
 * @param cmp Comparison function used to compare two elements.
 * @param arg Additional argument passed to the comparison function.
 * @param pp Array representing the current position in the Leonardo heap.
 * @param pshift Current shift value in the Leonardo heap.
 * @param trusty Flag indicating whether the current element is in a trusted position.
 * @param lp Array of Leonardo numbers used in the smoothsort algorithm.
 */
static void trinkle(unsigned char *head, size_t width, cmpfun cmp, void *arg, size_t pp[2], int pshift, int trusty, size_t lp[])
{
	unsigned char *stepson,
		*rt, *lf;
	size_t p[2];
	unsigned char *ar[14 * sizeof(size_t) + 1];
	int i = 1;
	int trail;

	p[0] = pp[0];
	p[1] = pp[1];

	ar[0] = head;
	while (p[0] != 1 || p[1] != 0)
	{
		stepson = head - lp[pshift];
		if (cmp(stepson, ar[0], arg) <= 0)
		{
			break;
		}
		if (!trusty && pshift > 1)
		{
			rt = head - width;
			lf = head - width - lp[pshift - 2];
			if (cmp(rt, stepson, arg) >= 0 || cmp(lf, stepson, arg) >= 0)
			{
				break;
			}
		}

		ar[i++] = stepson;
		head = stepson;
		trail = pntz(p);
		shr(p, trail);
		pshift += trail;
		trusty = 0;
	}
	if (!trusty)
	{
		cycle(width, ar, i);
		sift(head, width, cmp, arg, pshift, lp);
	}
}

/**
 * @brief Sorts an array using a variant of the smoothsort algorithm.
 *
 * This function sorts an array of elements using a comparison function provided by the user.
 * It is a variant of the smoothsort algorithm, which is an adaptive sorting algorithm.
 *
 * @param base Pointer to the base of the array to be sorted.
 * @param nel Number of elements in the array.
 * @param width Size of each element in the array.
 * @param cmp Comparison function that determines the order of the elements.
 *            It should return a negative value if the first argument is less than the second,
 *            zero if they are equal, and a positive value if the first argument is greater than the second.
 * @param arg Additional argument to be passed to the comparison function.
 */
void __qsort_r(void *base, size_t nel, size_t width, cmpfun cmp, void *arg)
{
	size_t lp[12 * sizeof(size_t)];
	size_t i, size = width * nel;
	unsigned char *head, *high;
	size_t p[2] = {1, 0};
	int pshift = 1;
	int trail;

	if (!size)
		return;

	head = base;
	high = head + size - width;

	/* Precompute Leonardo numbers, scaled by element width */
	for (lp[0] = lp[1] = width, i = 2; (lp[i] = lp[i - 2] + lp[i - 1] + width) < size; i++)
		;

	while (head < high)
	{
		if ((p[0] & 3) == 3)
		{
			sift(head, width, cmp, arg, pshift, lp);
			shr(p, 2);
			pshift += 2;
		}
		else
		{
			if (lp[pshift - 1] >= high - head)
			{
				trinkle(head, width, cmp, arg, p, pshift, 0, lp);
			}
			else
			{
				sift(head, width, cmp, arg, pshift, lp);
			}

			if (pshift == 1)
			{
				shl(p, 1);
				pshift = 0;
			}
			else
			{
				shl(p, pshift - 1);
				pshift = 1;
			}
		}

		p[0] |= 1;
		head += width;
	}

	trinkle(head, width, cmp, arg, p, pshift, 0, lp);

	while (pshift != 1 || p[0] != 1 || p[1] != 0)
	{
		if (pshift <= 1)
		{
			trail = pntz(p);
			shr(p, trail);
			pshift += trail;
		}
		else
		{
			shl(p, 2);
			pshift -= 2;
			p[0] ^= 7;
			shr(p, 1);
			trinkle(head - lp[pshift] - width, width, cmp, arg, p, pshift + 1, 1, lp);
			shl(p, 1);
			p[0] |= 1;
			trinkle(head - width, width, cmp, arg, p, pshift, 1, lp);
		}
		head -= width;
	}
}

weak_alias(__qsort_r, qsort_r);
