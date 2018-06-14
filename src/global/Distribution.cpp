// Distribution.cpp: implementation of the Distribution class.
//
//////////////////////////////////////////////////////////////////////


#include "Distribution.h"
#include <algorithm>
#include <math.h>

/* Implementation note: the distribution points are stored in the form
 * of an array. _size holds the size of the array. _cursor holds the
 * number of points currently stored points, and therefore the index
 * to the storage location of the next point to be stored.
 *
 * The points in the array are ordered when necessary. After they have
 * been ordered, traversal takes place using an iterator, which 
 * dynamically keeps track of corresponding cumulative probability.
 * The traversal routine is based on the expectation that cumulatives
 * or percentiles are called in either ascending or descending order.
 * 
 * A call to clear() will only reset _cursor, unless a size is specified
 * (which is optional). In that case, the distribution will resize itself
 * if necessary.
 */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Distribution::Distribution()
{
	_cursor = 0;			// current number of points in distribution
	_cumulative = 0;		// total probability of points in distribution
	_ordered = true;		// flag: points are ordered by value
	_iteratorCumulative = 0; // cumulative probability at iterator position
	_iterator = -1;			// iterator position
	_points = 0L;			// array of points
	_size = 0;				// maximum number of points in distribution
}

Distribution::Distribution(int size)
{
	_cursor = 0;
	_cumulative = 0;
	_ordered = true;
	_iteratorCumulative = 0;
	_iterator = -1;
	_points = new DistributionPoint[size];
	_size = size;
}

Distribution::Distribution(const Distribution & distribution)
{
	_points = 0L;
	_size = 0;

	int mySize = distribution.size();
	clear(mySize);

	for (int i = 0 ; i < mySize ; i++) {
		add(distribution.getValue(i),distribution.getProbability(i));
	}
}

Distribution::~Distribution()
{
	delete [] _points;
}

void Distribution::clear()
{
	_cursor = 0;
	_cumulative = 0;
	_ordered = true;
	_iteratorCumulative = 0;
	_iterator = -1;
}

void Distribution::clear(int size)
{
	_cursor = 0;
	_cumulative = 0;
	_ordered = true;
	_iteratorCumulative = 0;
	_iterator = -1;

	if (_size != size) {
		delete [] _points;
		_points = new DistributionPoint[size];
		_size = size;
	}
}

void Distribution::add(double value, double probability)
{
	if (_cursor < _size) {
		_points[_cursor] = DistributionPoint(value,probability);
		_cursor++;
		_cumulative += probability;
		_ordered = false;
	}
}

void Distribution::orderPoints()
{
	if (!_ordered) {
		sort(_cursor,_points);
		_ordered = true;
		_iteratorCumulative = 0;
		_iterator = -1;
	}
}

void Distribution::getExtremes(double &min, double &max)
{
	min = 0;
	max = 0;
	if (_cumulative > 0) {
		orderPoints();
		min = _points[0].getValue();
		max = _points[_cursor].getValue();
	}
}

double Distribution::getCumulative(double x)
{
	// this function moves the iterator to the point for which 
	// the value is lesser or equal to x, and then returns
	// corresponding cumulative probability value.
	// 
	// NOTE: same iterator is used by getPercentile()!

	double cumulative = 0;

	if (_cumulative > 0) {

		orderPoints();

		while (_points[_iterator].getValue() < x && _iterator < _cursor - 1) {
			_iterator++;
			_iteratorCumulative += _points[_iterator].getProbability();
		}

		if (_iterator == _cursor - 1) _iteratorCumulative = _cumulative;

		while (_points[_iterator].getValue() > x && _iterator > 0) {
			_iterator--;
			_iteratorCumulative -= _points[_iterator].getProbability();
		}

		if (_points[0].getValue() > x && _iterator == 0) {
			_iterator = -1;
			_iteratorCumulative = 0;
		}

		cumulative = _iteratorCumulative / _cumulative;
	}

	return cumulative;
}

double Distribution::getAvgDensity(double x0, double x1)
{
	if (x0 > x1) {
		double swap = x0;
		x0 = x1;
		x1 = swap;
	}

	if (x0 != x1) {
		double p0 = getCumulative(x0);
		double p1 = getCumulative(x1);
		return (p1 - p0) / (x1 - x0);
	} else {
		return 0;
	}
}

double Distribution::getPercentile(double cumulative)
{
	// moves iterator to point at which cumulative probability
	// is reached, and returns corresponding value.
	// 
	// NOTE: same iterator is used by getCumulative().

	double percentile = 0;

	if (_cumulative > 0) {

		orderPoints();	

		double target = cumulative * _cumulative;

		while (_iteratorCumulative > target && _iterator > 0) {
			_iterator--;
			_iteratorCumulative -= _points[_iterator].getProbability();
		}

		if (_iterator == 0) _iteratorCumulative = _points[0].getProbability();

		while (_iteratorCumulative < target && _iterator < _cursor - 1) {
			_iterator++;
			_iteratorCumulative += _points[_iterator].getProbability();
		}

		if (_iterator == _cursor - 1) _iteratorCumulative = _cumulative;

		percentile = _points[_iterator].getValue();
	}

	return percentile;

}

#define M  7
#define NSTACK  50

void Distribution::sort(int n,DistributionPoint array [])
{

	// Based on QuickSort routine taken from Numerical Recipes in C. 

	DistributionPoint * myArray;

	int i;
	int ir = n - 1;
	int j;
	int k;
	int l = 0;
	int jstack = 0;

	int istack[NSTACK + 1];

	DistributionPoint a;
	DistributionPoint temp;

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
				do {i++;} while (myArray[i] < a); // Scan up to nd element > a.
				do {j--;} while (myArray[j] > a); // Scan down to nd element < a.
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

void Distribution::swap(int i, int j, DistributionPoint *array)
{
	DistributionPoint point(array[i]);
	array[i] = array[j];
	array[j] = point;
}

void Distribution::reduce(int intervals, bool logScale)
{
	double value = 0;
	double mean = 0;
	double prob = 0;

	int index = 0;
	int oldIndex = 0;
	int prevIndex = 0;

	if (_cursor > 0 && intervals > 0) {

		orderPoints();

		double minValue = _points[0].getValue();
		double maxValue = _points[_cursor - 1].getValue();
		int k = 0;

		DistributionPoint * points = new DistributionPoint[intervals];

		for (int i = 0; i < intervals ; i++) {

			switch (logScale) {

			case true:
				value = minValue * pow(maxValue / minValue, (1. + i) / intervals);
				break;

			case false:
				value = minValue + (maxValue - minValue) * (i + 1.) / intervals;
				break;
			}

			if (i == intervals - 1)
				index = _cursor ;
			else {
				// move to first point beyond value
				while (_points[index].getValue() <= value && index < _cursor) {
					index++;
				}
			}

			reduceInterval(oldIndex,index - 1,mean,prob);

			if (prob > 0) { 
				// skip intervals that do not have any content
				points[k] = DistributionPoint(mean,prob);
				k++;
			}

			oldIndex = index;

		}

		delete [] _points;

		_size = intervals;
		_points = points;
		_cursor = k;
		_ordered = true;
		_iteratorCumulative = 0;
		_iterator = -1;
	}
}

void Distribution::reduceInterval(int start, int end, double & mean, double & probability)
{
	probability = 0;
	mean = 0;
	double sum = 0;

	if (start < 0) start = 0;
	if (end >= _cursor) end = _cursor - 1;

	for (int i = start; i <= end ; i++) {
		probability += _points[i].getProbability();
		sum += _points[i].getProbability() * _points[i].getValue();
	}

	if (probability > 0) 
		mean = sum / probability;
	else
		mean = 0.0;
}


double Distribution::getMean()
{
	double mean, probability;
	reduceInterval(0,_cursor - 1,mean,probability);
	return mean;
}

int Distribution::size() const
{
	return _cursor;
}

double Distribution::getValue(int index) const
{
	return _points[index].getValue();
}

double Distribution::getProbability(int index) const
{
	return _points[index].getProbability();
}

void Distribution::getPoint(int index, double &value, double &probability) const
{
	probability = _points[index].getProbability();
	value = _points[index].getValue();
}
