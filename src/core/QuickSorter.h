// $Id: QuickSorter.h 169 2005-01-28 14:17:28Z saulo $

#pragma once

class QuickSorter
{
public:
	QuickSorter(void);
	~QuickSorter(void);
	void swap(int i, int j, double *array);
	void sort(int n,double array []);
};
