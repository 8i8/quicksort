/*
 ============================================================================

 Name        : quicksort.c
 Author      : Iain Hill
 Version     : 1
 Copyright   : GPL

 Description : Program to observe and debug the quiksort algorithm

    Copyright (C) 2017 Iain Hill. 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define REFRESH_RATE	100000

typedef short int bool;
enum colours { WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA };
enum swap { NONE, ONE, TWO, THREE };
enum boolean { false, true };
bool STEP, AUTO, WORD, RESIZE;

void getSizes(void);
int myRand(int range);
void fillArray(int v[], int x, int y);
void pArray(int v[], int x);
void _qsort(int *v, int left, int right);
void swap(int *v, int left, int right, int type);
void draw(int *v, int left, int right, int colour, int swap);
void histogram(int array[], int x, int y, int left, int right, int colour);
void clearScreen();

int ROW, X = 20;
int COL, Y = 40;

int main(int argc, char* argv[])
{
	int c;
	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
				case 's':
					STEP = true;
					break;
				case 'a':
					AUTO = true;
					break;
				case 'w':
					WORD = true;
					break;
				case 'r':
					RESIZE = true;
					break;
				default:
					break;
			}

	getSizes();
	int x = X;
	int y = Y;
	int v[x];

	while(1)
	{
		fillArray(v, x, y);
		_qsort(v, 0, x-1);

		if (AUTO)
			usleep(REFRESH_RATE*10);
		else
			getchar();
	}

	return 0;
}

void getSizes(void)
{
	struct winsize max;
	ioctl(0, TIOCGWINSZ , &max);

	ROW = max.ws_row;
	COL = max.ws_col;
	if (RESIZE) {
		Y = ROW-2;
		X = (COL/3)-2;
	}
}

/*
 * Return a random value between the given limits, I have used timespec to
 * generate the seed for my random value, for this reason the program will only
 * run on a POSIX machine, I shall look further into this subject in the future
 * as it is clearly a facinating area of the computing landscape.
 *
 * <time.h>
 *
 * typedef __clockid_t clockid_t;
 * extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp) __THROW;
 *
 * https://linux.die.net/man/3/clock_getres
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/clock_gettime.html
 * https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/gettime.html
 */
int myRand(int range)
{
	struct timespec seed;
	clock_gettime(CLOCK_MONOTONIC, &seed);
	int x;

	srand(seed.tv_nsec);
	x = rand()%range+1;

	return x;
}

/*
 * Fill array with randomly generated values.
 */
void fillArray(int v[], int x, int y)
{
	int i;
	for(i = 0; i <= x; i++)
		v[i] = myRand(y-1);	
}

/*
 * Print out an array pf int's.
 */
void pArray(int v[], int x)
{
	int i;
	for (i = 0; i < x; i++)
		printf("\033[37m%2d ", v[i]);
	putchar('\n');
	for (i = 0; i < x; i++)
		printf("\033[37m%2d ", i);
	putchar('\n');
}

/*
 * Perform qsort on an array of integers.
 */
void _qsort(int *v, int left, int right)
{
	int i, last;

	if (left >= right) {
		draw(v, left, right, BLUE, NONE);
		return;
	}

	swap(v, left, (left + right)/2, ONE);

	last = left;
	for (i = left+1; i <= right; i++) {
		if (v[i] < v[left]) {
			swap(v, ++last, i, TWO);
		} else
			draw(v, left, i, BLUE, NONE);
	}

	swap(v, left, last, THREE);

	_qsort(v, left, last-1);
	_qsort(v, last+1, right);
}

char* setSwap(int swap)
{
	static int hold = 0;
	swap ? (hold = swap) : (swap = hold);

	if (swap == ONE)
		return "One";
	else if (swap == TWO)
		return "Two";
	else if (swap == THREE)
		return "Three";

	return NULL;
}

void draw(int *v, int left, int right, int colour, int swap)
{
	char* sw;

	clearScreen();
	sw = setSwap(swap);
	histogram(v, X, Y, left, right, colour);
	pArray(v, X);
	if (WORD)
		printf("\033[37mleft: v[%d]=%d,\tright v[%d]=%d\t\t\tswap:~ %s\n", 
				left, v[left], right, v[right], sw);

	if (STEP && ((colour == RED && v[left] > v[right]) || colour == YELLOW)) {
		getchar();
	} else 
		usleep(REFRESH_RATE);
}

void swap(int *v, int left, int right, int swap)
{
	int temp;

	if (left == right) {
		draw(v, left, right, MAGENTA, swap);
		return;
	}

	draw(v, left, right, RED, swap);

	temp = v[left];
	v[left] = v[right];
	v[right] = temp;

	draw(v, left, right, RED, swap);
	draw(v, left, right, YELLOW, swap);
	draw(v, left, right, GREEN, swap);
}

/*
 * Generate and print a histogram of an array of integers.
 */
void histogram(int *v, int x, int y, int left, int right, int colour)
{
	int hist[x][y];
	int i, j, k;

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++)
			if (j < v[i])
				hist[i][j] = '|';
			else 
				hist[i][j] = ' ';
	}

	for (j = y-1; j >= 0; j--) {
		for (k = 0; k < x; k++) {
			if (k == left || k == right) {
				if (colour == RED)
					printf(" \033[1;31m%c ", hist[k][j]);
				else if (colour == GREEN)
					printf(" \033[1;32m%c ", hist[k][j]);
				else if (colour == YELLOW)
					printf(" \033[1;33m%c ", hist[k][j]);
				else if (colour == BLUE)
					printf(" \033[1;34m%c ", hist[k][j]);
				else if (colour == MAGENTA)
					printf(" \033[1;35m%c ", hist[k][j]);
			} else
				printf(" \033[37m%c ", hist[k][j]);
		}
		printf("\n");
	}
}

/*
 * Clear the screen.
 */
void clearScreen()
{
	puts("\033[H\033[J");
}

