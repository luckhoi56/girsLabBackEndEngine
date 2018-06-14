// TabularBinomialSupport.h: interface for the TabularBinomialSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARBINOMIALSUPPORT_H__7129A5B2_F523_4996_868E_48666B8D4C39__INCLUDED_)
#define AFX_TABULARBINOMIALSUPPORT_H__7129A5B2_F523_4996_868E_48666B8D4C39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularIntegrator.h"
#include "../global/Function.h"

class TabularBinomialSupport : public TabularIntegrator
{
public:
	TabularBinomialSupport(double k, double n, int m, double lo, double hi);
	TabularBinomialSupport(double k, double n, int m, double lo, double hi, double betaln);
	virtual ~TabularBinomialSupport();

	int computeIntegral(double x, double & i0, double & lnc) const;
	int inverseIntegral(double i0, double & x) const;
	int computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const;
	int computeLogIntegrant(double x, double &p, double &lnc) const;

private:
	int computeSeriesIntegral(int b, double q, double & p) const;
	int inverseSeriesIntegral(int b, double p, double & q) const;
	int computeSimpsonIntegral(double b, double q, double & p) const;
	int inverseSimpsonIntegral(double b, double p, double & q) const;


	double lnc;  // natural log of dividing constant
	double loi0; // integral offset (integral over 0 - lo)
	int m;       // 0 -> uniform prior, 1 -> loguniform prior
	double k;		 // number of events
	double n;       // number of demands
	double lo;   // lower integration bound
	double hi;   // upper integration bound

	bool integer_n; // flag: n is integer value. used when k == 0 and m == 1

};


#endif // !defined(AFX_TABULARBINOMIALSUPPORT_H__7129A5B2_F523_4996_868E_48666B8D4C39__INCLUDED_)
