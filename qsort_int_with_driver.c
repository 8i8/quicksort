#include <stdio.h>
#include <string.h>

/*
 * Sort
 */
void qsort_int(int v[], int left, int right)
{
	int i, last;
	void _swap(int v[], int i, int j);

	if (left >= right)	/* do nothing if array cantains */
		return;		/* fewer than two eliments */
	_swap(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
	if (v[i] < v[left])
		_swap(v, ++last, i);
	_swap(v, left, last);
	qsort_int(v, left, last-1);
	qsort_int(v, last+1, right);
}

/*
 * Swap
 */
void _swap(int v[], int i, int j)
{
	int temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

void print_array(int *arr, int len)
{
	int i;
	for (i = 0; i < len; i++)
	printf("%d ", arr[i]);
}

int main(void)
{
	int arr[] = { 6, 12, 4, 18, 3, 27, 16, 15, 19 };
	int len = sizeof(arr)/sizeof(int);

	qsort_int(arr, 0, len-1);
	print_array(arr, len);

	return 0;
}

