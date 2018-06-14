// Distribution.h: interface for the Distribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTION_H__08663498_4F9C_11D4_9894_002018557056__INCLUDED_)
#define AFX_DISTRIBUTION_H__08663498_4F9C_11D4_9894_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DistributionPoint.h"
#include <vector>

using namespace std;

typedef vector<DistributionPoint> DistributionPointList;

class Distribution  
{
public:
	void getPoint(int index, double & value, double & probability) const;
	double getProbability(int index) const;
	double getValue(int index) const;
	int size() const;
	double getMean();

	void reduce(int intervals, bool logScale);
	void getExtremes(double & min, double & max);
	/* returns the average density over specified interval */
	double getAvgDensity(double x0,double x1);
	/* returns cumulative probability at x */
	double getCumulative(double x);
	/* clears the distribution, reset maximum number of points */
	void clear(int size);
	/* clears the distribution */
	void clear();
	/* returns the percentile value */
	double getPercentile(double cumulative);
	/* add a new point to the distribution */
	void add(double value, double probability);

	Distribution();
	Distribution(int size);
	Distribution(const Distribution & distribution);
	virtual ~Distribution();
private:

	void reduceInterval(int start, int end,double & mean, double & probability);
	void swap(int i, int j, DistributionPoint *array);
	void sort(int n, DistributionPoint array[]);
	DistributionPoint * _points;
	bool _ordered;
	int _cursor;
	int _iterator;
	double _iteratorCumulative;
	int _size;
	double _cumulative;
	void orderPoints();
};

#endif // !defined(AFX_DISTRIBUTION_H__08663498_4F9C_11D4_9894_002018557056__INCLUDED_)
