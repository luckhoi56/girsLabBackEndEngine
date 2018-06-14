// BetaDistMLEFunction.cpp: implementation of the BetaDistMLEFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BetaDistMLEFunction.h"
#include "DCDFLIBInterface.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BetaDistMLEFunction::BetaDistMLEFunction( int n, double x[])
{
	points = n;
	sumX = sum1X = 0;
	for( int i = 0; i < points; i++ ) {
		sumX += log(x[i]);
		sum1X += log(1 - x[i]);
	}
}

BetaDistMLEFunction::~BetaDistMLEFunction()
{
	
}

void BetaDistMLEFunction::evaluate(double alpha, double beta, double result[])
{
	result[0] = DCDFLIBInterface::computeDiGamma(alpha+beta)-
				DCDFLIBInterface::computeDiGamma(beta) - sum1X/points;
	result[1] = DCDFLIBInterface::computeDiGamma(alpha+beta)-
				DCDFLIBInterface::computeDiGamma(alpha) - sumX/points;

	//return result;
}

double BetaDistMLEFunction::evaluate(const Parameters & params) const
{
	return -1;
}
