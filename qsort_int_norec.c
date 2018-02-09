#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int *list;
	size_t len;
} List;

void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

/*
  For each value between positions l and r, if the value at the current index is
  greater than that of the pivot; Swap it with the leftmost value and then
  incriment the left index by one place on the array.
*/
int partition(int arr[], int l, int r)
{
	int i, j, pivot;

	i = l, pivot = arr[r];

	for (j = l; j < r; j++)
		if (arr[j] <= pivot)
			swap(&arr[i++], &arr[j]);

	swap(&arr[i], &arr[r]);

	return i;
}

/*
  While there are still values to exchange; Unload the previous values from the
  stack buffer; If the value to the left of the partition is greater than the
  current left value; Add both the left value and the value at partition-1 to
  the buffer for later processing. If the value to the right of the partition is
  smaller than the right most value; Add both the pointer+1 and the rightmost
  value to the buffer for processing.
*/
void quicksort(int arr[], int l, int r)
{
	int p, pt = -1, *buf;

	buf = malloc(sizeof(buf)*300);

	buf[++pt] = l;
	buf[++pt] = r;

	while (pt >= 0)
	{
		r = buf[pt--];
		l = buf[pt--];

		p = partition(arr, l, r);

		if (p-1 > l) {
			buf[++pt] = l;
			buf[++pt] = p-1;
		}

		if (p+1 < r) {
			buf[++pt] = p+1;
			buf[++pt] = r;
		}
	}

	free(buf);
}

void fill_list(List *l)
{
	size_t i;
	int list[] = { 4, 7, 8, 3, 5, 53, 65, 23, 10 };
	l->list = malloc(sizeof list);
	l->len = sizeof list / sizeof (int);
	for (i = 0; i < l->len; i++)
		l->list[i] = list[i];
}

void print_list(List *l)
{
	size_t i;

	for (i = 0; i < l->len; i++)
		printf("%d ", l->list[i]);
	putchar('\n');
}

int main(void)
{
	List l;
	fill_list(&l);
	print_list(&l);
	quicksort(l.list, 0, l.len-1);
	print_list(&l);
	free(l.list);

	return 0;
}
