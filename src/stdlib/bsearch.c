#include <stdlib.h>

/**
 * bsearch - Perform a binary search on an array.
 * @key: Pointer to the item being searched for.
 * @base: Pointer to the base of the array to be searched.
 * @nel: Number of elements in the array.
 * @width: Size of each element in the array.
 * @cmp: Comparison function which returns negative if the first argument is less than the second,
 *       zero if they are equal, and positive if the first argument is greater than the second.
 *
 * This function performs a binary search on the array specified by @base. The array must be sorted
 * in ascending order according to the comparison function @cmp. If the item is found, a pointer to
 * the item is returned. If the item is not found, NULL is returned.
 */
void *bsearch(const void *key, const void *base, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	void *try;
	int sign;
	while (nel > 0)
	{
		try = (char *)base + width * (nel / 2);
		sign = cmp(key, try);
		if (sign < 0)
		{
			nel /= 2;
		}
		else if (sign > 0)
		{
			base = (char *)try + width;
			nel -= nel / 2 + 1;
		}
		else
		{
			return try;
		}
	}
	return NULL;
}
