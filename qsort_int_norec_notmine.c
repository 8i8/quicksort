#include <stdio.h>

void swap (int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int partition (int arr[], int l, int r)
{
	int i, t;

	for (i = l, t = 0; i < r; i++, t = 0)
		if (arr[i] <= arr[r])
			swap (&arr[l++], &arr[i]);

	swap (&arr[l], &arr[r]);

	return l;
}

void quicksort(int arr[], int l, int r)
{
	int p, pt = -1, buf[r - l+1];

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
}

void printArr(int arr[], int n)
{
	int i;
	for (i = 0; i < n; ++i)
		printf("%d ", arr[i]);
}

int main()
{
	//int arr[] = {4, 3, 5, 2, 1, 3, 2, 3};
	int arr[] = {4, 2, 5, 3, 1};
	int n = sizeof arr / sizeof *arr;
	quicksort(arr, 0, n - 1);
	printArr(arr, n);
	return 0;
}
