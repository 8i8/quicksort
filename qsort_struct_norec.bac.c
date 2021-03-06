#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LEN	5

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Stack
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef struct _stack {
	void *elems;
	int elemSize;
	int logLength;
	int allocLength;
} stack;

void stackNew(stack *s, int elemSize);
void stackDispose(stack *s);
void stackPush(stack *s, void *elemAddr);
void stackPop(stack *s, void *elemAddr);

void stackNew(stack *s, int elemSize)
{
	s->elemSize = elemSize;
	s->logLength = 0;
	s->allocLength = 4;
	s->elems = malloc (4 * elemSize);
}

void stackDispose(stack *s)
{
	free(s->elems);
}

void stackGrow(stack *s)
{
	s->allocLength *= 2;
	s->elems = realloc(s->elems, s->allocLength * s->elemSize);
}

void stackPush(stack *s, void* elemAddr)
{
	if (s->logLength == s->allocLength)
		stackGrow(s);
	
	void *target = (char*)s->elems + s->logLength * s->elemSize;

	memcpy(target, elemAddr, s->elemSize);
	s->logLength++;
}

void stackPop(stack *s, void *elemAddr)
{
	s->logLength--;
	void *source = (char*)s->elems + s->logLength * s->elemSize;
	memcpy(elemAddr, source, s->elemSize);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  driver
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef struct {
	int i;
	char *string;
	size_t num;
} Test;

typedef int (*comp)(const void *, const void *);
static void _swap(char *i, char *j, size_t width);

Test *iterate_list(void (*fn)(Test*, size_t), Test *l, size_t len);
void print_struct(Test *l, size_t i);
int strscmp(const void* v1, const void* v2);
int intscmp(const void* v1, const void* v2);

/*
 * main:	A test, to drive a list of structs through qsort.
 */
int main(void)
{
	Test list[LEN] = {
		{ 4, "This is string one", 1 },
		{ 2, "And this number 2", 2 },
		{ 5, "World of wonders", 3	},
		{ 3, "Another line always another line", 4 },
		{ 1, "Aardvarks are always first", 5 },
	};
	Test *l_ptr;
	l_ptr = list;

	iterate_list(print_struct, l_ptr, LEN);
	printf("~~~\n");
	qsort(&list, LEN, sizeof(Test), strscmp);
	iterate_list(print_struct, l_ptr, LEN);
	printf("\n");

	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  qsort
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

char *_partition(char *l, char *r, size_t width, comp fn)
{
	char *i;
	int t;

	for (i = l, t = 0; i < r; i += width, t = 0)
		if ((t = (fn)(i, r)) < 0) {
			_swap(l, i, width);
			l += width;
		} else if (t == 0)
			l += width;

	_swap(l, r, width);

	return l;
}

/*
 * qsort:	generic qsort function.
 */
void qsort(void *base, size_t nel, size_t width, comp fn)
{
	char **buf;
	char *b, *e, *l, *r, *p;
	int pt = -1;

	if (nel > 1)
		nel--;
	else
		return;

	buf = malloc(300*width);
	b = base;
	e = b+nel*width;

	*(buf+(++pt)*width) = b;
	*(buf+(++pt)*width) = e;

	while (pt >= 0)
	{
		r = *(buf+(pt--)*width);
		l = *(buf+(pt--)*width);

		p = _partition(l, r, width, fn);

		if(p > b)
			if ((fn)(p-width, l) > 0) {
				*(buf+(++pt)*width) = l;
				*(buf+(++pt)*width) = p-width;
			}

		if (p < e)
			if ((fn)(p+width, r) < 0) {
				*(buf+(++pt)*width) = p+width;
				*(buf+(++pt)*width) = r;
			}
	}
	free(buf);
}

/*
 * Swap for 'width' places.
 */
static void _swap(char *l, char *r, size_t width)
{
	char tmp[256];
	size_t len;

	while (width) {
		len = (sizeof tmp < width) ? sizeof tmp : width;
		memcpy(tmp, l, len);
		memcpy(l, r, len);
		memcpy(r, tmp, len);
		l += len;
		r += len;
		width -= len;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  sort functions
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * strscmp:	Wrapper for struct data, sort function.
 */
int strscmp(const void* v1, const void* v2)
{
	const Test *s1, *s2;
	s1 = v1, s2 = v2;
	return strcmp((const void*)s1->string, (const void*)s2->string);
}

int intscmp(const void* v1, const void* v2)
{
	const Test *s1, *s2;
	s1 = v1, s2 = v2;
	if(s1->i < s2->i)
		return 1;
	if(s1->i > s2->i)
		return -1;
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  print
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * print_struct:	Print Test[i] content.
 */
void print_struct(Test *l, size_t i)
{
	printf("%d %lu: %s\n", l[i].i, l[i].num, l[i].string);
}

/*
 * iterate:	For 'len' elements preform fn(Test*[],size_t len).
 */
Test *iterate_list(void (*fn)(Test*, size_t), Test *l, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		fn(l, i);

	return l;
}
