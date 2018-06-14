// TabularBinomialDistribution.h: interface for the TabularBinomialDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARBINOMIALDISTRIBUTION_H__EA59D0CC_A6B1_4D81_9E0A_0DDB5DC760DE__INCLUDED_)
#define AFX_TABULARBINOMIALDISTRIBUTION_H__EA59D0CC_A6B1_4D81_9E0A_0DDB5DC760DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UpdatedTabularDistribution.h"

class TabularBinomialDistribution : public UpdatedTabularDistribution  
{
public:
	TabularBinomialDistribution(double * vals, double * cdfs, int length, double k, double n);
	virtual ~TabularBinomialDistribution();

};

#endif // !defined(AFX_TABULARBINOMIALDISTRIBUTION_H__EA59D0CC_A6B1_4D81_9E0A_0DDB5DC760DE__INCLUDED_)
