// BetaDistMLEFunction.h: interface for the BetaDistMLEFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BETADISTMLEFUNCTION_H__A8017198_4370_4AD9_9D18_9E3CF2943B23__INCLUDED_)
#define AFX_BETADISTMLEFUNCTION_H__A8017198_4370_4AD9_9D18_9E3CF2943B23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"

class BetaDistMLEFunction : public Function  
{
public:
	BetaDistMLEFunction( int n, double x[]);
	virtual ~BetaDistMLEFunction();
	double evaluate(const Parameters & params) const;
	void evaluate(double alpha, double beta, double result[]);
	virtual int getDimension() const {return 2;}
private:
	int points;
	double sumX;
	double sum1X;
};

#endif // !defined(AFX_BETADISTMLEFUNCTION_H__A8017198_4370_4AD9_9D18_9E3CF2943B23__INCLUDED_)
