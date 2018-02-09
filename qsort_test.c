#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LEN	5

#define MIN(a, b) ((a)<(b) ? (a) : (b))

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
		{ 3, "And this number 2", 2 },
		{ 5, "World of wonders", 3	},
		{ 2, "Another line always another line", 4 },
		{ 1, "Aardvarks are always first", 5 }
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

#define MIN(a, b) ((a)<(b) ? (a) : (b))

static void swap(char *a, char *b, size_t len)
{
	char tmp[256];
	size_t l;
	while (len) {
		l = MIN(sizeof tmp, len);
		memcpy(tmp, a, l);
		memcpy(a, b, l);
		memcpy(b, tmp, l);
		a += l;
		b += l;
		len -= l;
	}
}

static void sift(char *base, size_t root, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	size_t max;

	while (2*root <= nel) {
		max = 2*root;
		if (max < nel && cmp(base+max*width, base+(max+1)*width) < 0)
			max++;
		if (cmp(base+root*width, base+max*width) < 0) {
			swap(base+root*width, base+max*width, width);
			root = max;
		} else break;
	}
}

void qsort(void *_base, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	char *base = _base;
	size_t i;

	if (!nel) return;
	for (i=(nel+1)/2; i; i--)
		sift(base, i-1, nel-1, width, cmp);
	for (i=nel-1; i; i--) {
		swap(base, base+i*width, width);
		sift(base, 0, i-1, width, cmp);
	}
}

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
	return s1->i - s2->i;
}

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
