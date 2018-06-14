// $Id: QuickSorter.cpp 169 2005-01-28 14:17:28Z saulo $
#include "QuickSorter.h"

#define M  7
#define NSTACK  50

QuickSorter::QuickSorter(void)
{
}

QuickSorter::~QuickSorter(void)
{
}

void QuickSorter::sort(int n,double array [])
{
	double * myArray;

	int i;
	int ir = n - 1;
	int j;
	int k;
	int l = 0;
	int jstack = 0;

	int istack[NSTACK + 1];

	double a;

	myArray = array;

	for (;;) {
		if (ir - l < M) {
			for (j = l+1; j <= ir; j++) {
				a = myArray[j];
				for (i = j-1 ; i >= l; i--) {
					if (myArray[i] <= a) break;
					myArray[i+1] = myArray[i];
				}
				myArray[i+1] = a;
			}
			if (jstack == 0) break;
			ir = istack[jstack--]; // Pop stack and begin a new round of partitioning.
			l = istack[jstack--];
		} else {
			k = (l + ir) / 2; // Choose median of left, center, and right elements as partitioning element a. Also rearrange so that a[l] a[l+1] a[ir].
			swap(k, l + 1, myArray);
			if (myArray[l] > myArray[ir]) {
				swap(l,ir,myArray);
			}
			if (myArray[l+1] > myArray[ir]) {
				swap(l + 1, ir,myArray);
			}
			if (myArray[l] > myArray[l+1]) {
				swap(l, l + 1,myArray);
			}
			i = l + 1; // Initialize pointers for partitioning.
			j = ir;
			a = myArray[l+1]; // Partitioning element.

			for (;;) { // Beginning of innermost loop.
				do {i++;} while (myArray[i] < a); 
				do {j--;} while (myArray[j] > a);
				if (j < i) break; // Pointers crossed. Partitioning complete.
				swap(i, j,myArray); // Exchange elements.
			} // End of innermost loop.

			myArray[l+1] = myArray[j]; // Insert partitioning element.
			myArray[j] = a;
			jstack += 2; // Push pointers to larger subarray on stack, process smaller subarray immediately.
			// if (jstack > NSTACK) nrerror("NSTACK too small in sort.");
			if (ir - i + 1 >= j - l) {
				istack[jstack] = ir;
				istack[jstack - 1] = i;
				ir = j - 1;
			} else {
				istack[jstack] = j - 1;
				istack[jstack - 1] = l;
				l = i;
			}
		}
	}
}

void QuickSorter::swap(int i, int j, double *array)
{
	double x = array[i];
	array[i] = array[j];
	array[j] = x;
}
