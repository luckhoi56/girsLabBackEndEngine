// UpdatedTabularDistribution.h: interface for the UpdatedTabularDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATEDTABULARDISTRIBUTION_H__23318C43_B260_4E35_BB53_B86940FD3565__INCLUDED_)
#define AFX_UPDATEDTABULARDISTRIBUTION_H__23318C43_B260_4E35_BB53_B86940FD3565__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricDistribution.h"
#include "SimpleHomogeneous.h"

class UpdatedTabularDistribution : public ParametricDistribution
{

public:
	UpdatedTabularDistribution(double * vals, double * cdfs, int length, ParametricDistribution * likelihood);
	virtual ~UpdatedTabularDistribution();

	virtual double getSample() const {return -1;}
	virtual double getMedian() const {return -1;}
	virtual double getMean() const {return -1;}
	virtual double getQuantile(double x) const ;
	virtual double getCDF(double x) const;
	virtual double getLogPDF(double x) const {return -1;}
	virtual double getVariance() const {return -1;}
	virtual void   releaseMemory() {};

	double getUpperBound() {return upperBound;}
	double getLowerBound() {return lowerBound;}

private:
	double * valArray;
	double * cdfArray;
	int size;
	ParametricDistribution * like;

	double upperBound;
	double lowerBound;
	double integral;

};

#endif // !defined(AFX_UPDATEDTABULARDISTRIBUTION_H__23318C43_B260_4E35_BB53_B86940FD3565__INCLUDED_)