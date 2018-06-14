// TabularPoissonDistribution.h: interface for the TabularPoissonDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARPOISSONDISTRIBUTION_H__49654E35_2F55_4F35_8993_CA320FDCBC9C__INCLUDED_)
#define AFX_TABULARPOISSONDISTRIBUTION_H__49654E35_2F55_4F35_8993_CA320FDCBC9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UpdatedTabularDistribution.h"

class TabularPoissonDistribution : public UpdatedTabularDistribution  
{
public:
	TabularPoissonDistribution(double * vals, double * cdfs, int length, double k, double t);
	virtual ~TabularPoissonDistribution();
};

#endif // !defined(AFX_TABULARPOISSONDISTRIBUTION_H__49654E35_2F55_4F35_8993_CA320FDCBC9C__INCLUDED_)
