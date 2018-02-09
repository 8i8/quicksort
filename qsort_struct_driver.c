#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN	5

struct test {
	char *string;
	size_t num;
};

struct test *iterate_list(void (*fn)(struct test*, size_t), struct test *l, size_t len);
void print_struct(struct test *l, size_t i);
void _qsort(void *base, size_t nel, size_t width, int (*fn)(const void*, const void*));
int strscmp(const void* v1, const void* v2);

/**
 * main:	A test, to drive a list of structs through qsort.
 */
int main(void)
{
	struct test list[LEN] = {
		{ "This is one", 1 },
		{ "Hello num 2", 2 },
		{ "World of wonders", 3 },
		{ "Another is one", 4 },
		{ "Aardvarks always come first", 5 }
	};
	struct test *l_ptr;
	l_ptr = list;

	//iterate_list(print_struct, l_ptr, LEN);
	_qsort(&list, LEN, sizeof(struct test), strscmp);
	iterate_list(print_struct, l_ptr, LEN);

	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    iterated functions
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * print_struct:	Print struct test[i] content.
 */
void print_struct(struct test *l, size_t i)
{
	printf("%lu: %s\n", l[i].num, l[i].string);
}

/**
 * iterate:	For 'len' elements preform fn(struct test*[],size_t len).
 */
struct test *iterate_list(void (*fn)(struct test*, size_t), struct test *l, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		fn(l, i);

	return l;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    qsort
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * qsort:	generic qsort function.
 */
void _qsort(void *base, size_t nel, size_t width, int (*fn)(const void*,const void*))
{
	void swap(void *string, size_t i, size_t j, size_t width);
	unsigned char *b = (unsigned char*)base;
	size_t i, left, last;

	left = 0;
	if (nel)
		nel--;

	if (left >= nel)
		return;

	swap(b, left, nel/2, width);

	last = left;
	for (i = left+1; i <= nel; i++)
		if ((fn)(&b[i*width], &b[left*width]) < 0)
			swap(b, ++last, i, width);

	swap(b, left, last, width);

	_qsort(b+(left*width), last-left, width, fn);
	_qsort(b+((last+1)*width), nel-last, width, fn);
}

/**
 * swap:	For 'width' places, swap 'i' to 'j' on the given data.
 */
void swap(void *string, size_t i, size_t j, size_t width)
{
	if (i == j)
		return; /* return directly if swap is not required */

	unsigned char t;
	unsigned char *s;
	s = string;

	i = i*width;
	j = j*width;

	size_t k;
	for (k = 0; k < width; k++) {
		t        = *(s+k+i);
		*(s+k+i) = *(s+k+j);
		*(s+k+j) = t;
	}
}

/**
 * strscmp:	Wrapper for struct data, sort function.
 */
int strscmp(const void* v1, const void* v2)
{
	const struct test *s1, *s2;
	s1 = v1, s2 = v2;
	return strcmp((const void*)s1->string, (const void*)s2->string);
}

