// ParametricDistributionFitter.cpp: implementation of the ParametricDistributionFitter class.
//

//////////////////////////////////////////////////////////////////////


#include "../dcdf/DCDFLIBInterface.h"
#include "ParametricDistributionFitter.h"
#include "NumericalRecipes.h"
#include "BetaDistMLEFunction.h"
#include <math.h>
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ParametricDistributionFitter::ParametricDistributionFitter()
{

}

ParametricDistributionFitter::~ParametricDistributionFitter()
{

}

// Estimates the Gamma distribution parameters via MLE. It uses the interpretation
// of Beta parameter as alpha * beta is the mean. Therefore, before returning
// he result, it takes the inverse of the estimated beta parameter (interpretation
// used by Java code)
// It returns alpha in result[0] and beta in result[1]. If result[0] = -1, then
// the procedure failed. It could not solve the MLE equation for the alpha
// parameter. It should try a differnet starting pair that does bracket the root
double* ParametricDistributionFitter::FitGamma(int points, double x[])
{
	double *result = new double[2];
	double alphaLow = 0.01;
	double alphaHigh = 5;
	double tol = 0.0001;

	GammaDistAlphaMLEFunction function(points, x);
	result[0] = NumericalRecipes::zbrent(function, alphaLow, alphaHigh, tol);
	if( result[0] != -1 ) {
		double sum = 0;
		for( int i = 0 ; i < points; i++ )
			sum += x[i];
		result[1] = sum/(points * result[0]);
		result[1] = 1/result[1];
	}

	return result;
}


double* ParametricDistributionFitter::FitBeta(int points, double x[])
{
	double *result = new double[2];
	//int check;

	FitBetaApproxMLE(points, x, result);
	

	//BetaDistMLEFunction function(points, x);
	//result[0] = 1;
	//result[1] = 1;
	//NumericalRecipes::broydn(result, 2, &check, function);
	
	return result;
}

void ParametricDistributionFitter::FitBetaApproxMLE(int points, double x[], double result[])
{
	double prodX = 1;
	double prod1MinusX = 1;

	for(int i = 0; i < points; i++) {
		prodX *= pow(x[i], 1.0/points);
		prod1MinusX *= pow(1.0-x[i], 1.0/points);
	}

	result[0] = 0.5*(1.0-prod1MinusX)/(1.0-prodX-prod1MinusX); // Alpha estimate
	result[1] = 0.5*(1.0-prodX)/(1.0-prodX-prod1MinusX); // Beta estimate
}



double* ParametricDistributionFitter::FitBetaMLE(int points, double x[])
{
	double *result = new double[2];
	int check;
	

	BetaDistMLEFunction function(points, x);
	result[0] = 1;
	result[1] = 1;
	NumericalRecipes::broydn(result, 2, &check, function);
	
	return result;
}


double* ParametricDistributionFitter::FitNormal(int points, double x[], double y[])
{
	int i;
    double Sumx = 0;
    double Sumy = 0;
    double Sumxx = 0;
    double Sumxy = 0;
    double Sumxy2 = 0;
    double Sumy2 = 0;
    double xMed = 0;
    double yMed = 0;
    double aEst = 0;
    double bEst = 0;
    double *yTransformed = new double[points];
    
    for( i = 0; i < points; i++ ) {
		double stdNormalDeviate = DCDFLIBInterface::computeNormalQuantile(0, 1, y[i]);
		if( y[i] >= 1 )
			stdNormalDeviate = 4.0;
		if( y[i] <= 0 )
			stdNormalDeviate = -4.0;
		yTransformed[i] = stdNormalDeviate;
	}

    for( i = 0; i < points; i++ ) {
		Sumxy += x[i] * yTransformed[i];
		Sumy += yTransformed[i];
		Sumx += x[i];
		Sumxx += x[i] * x[i];
    }
    xMed = Sumx / points;
    yMed = Sumy / points;
    bEst = ( Sumxy - xMed * Sumy ) / ( Sumxx - points * xMed * xMed );
    aEst = yMed - bEst * xMed;

    for( i = 0; i < points; i++ ) {
		Sumxy2 += pow( yTransformed[i] - aEst - bEst * x[i], 2.0 );
		Sumy2  +=  pow( yTransformed[i] - yMed, 2.0 );
    }

	double *result = new double[3];
    result[0] = -aEst / bEst; // Mean
    result[1] = 1 / bEst; // Std
	result[2] = 1 - Sumxy2 / Sumy2; // Coef. of Determination

	delete[] yTransformed;

	return result;
}

double* ParametricDistributionFitter::FitLogNormal(int points, double x[], double y[])
{
	double *xTransformed = new double[points];
	double *result = new double[3];
        
    for( int i = 0; i < points; i++ )
		xTransformed[i] = log( x[i] ); // Take the natural log.
	
	result = FitNormal(points, xTransformed, y);
	result[0] = exp(result[0]); // Median of the LogNormal distribution.
	delete[] xTransformed;
	
	return result;
}


//******************************************************************************
GammaDistAlphaMLEFunction::GammaDistAlphaMLEFunction(int datapoints, double x[])
{
	dataPoints = datapoints;
	xValues = x;
}

GammaDistAlphaMLEFunction::~GammaDistAlphaMLEFunction()
{

}

double GammaDistAlphaMLEFunction::evaluate(const Parameters & params) const
{
	double alpha = params.getValue(0);
	if (alpha == 0) return 0;

	int i;
	double a = DCDFLIBInterface::computeDiGamma(alpha);
	double b = log(alpha);
	double sum1 = 0;
	double sum2 = 0;

	for( i = 0; i < dataPoints; i++ ) {
		sum1 += log(xValues[i]);
		sum2 += xValues[i];
	}
	double c = sum1/dataPoints;
	double d = log(sum2/dataPoints);

	return a - b - c  + d;
}

void ParametricDistributionFitter::TestFitGamma()
{
	double x[] = {0.102586455, 0.210720827, 0.32503749, 0.446286776, 0.575364538,
				  0.713349664, 0.861566605, 1.021651315, 1.195674031, 1.386295025,
				  1.597015853, 1.832581802, 2.099645826, 2.40794634, 2.772590051,
				  3.218874554, 3.794239092, 4.605171853, 5.991460057, 9.210343705};
	int points = 20;
	double *params = new double[2];

	params = FitGamma(points, x);
	cout << "Alpha = " << params[0] << endl << "Beta = " << params[1];
}

void ParametricDistributionFitter::TestFitBeta()
{
	//double x[] = {0.00684399, 0.010378, 0.00843834, 0.0113549, 0.00901281, 0.0124768,
	//			  0.0100752, 0.0074704, 0.00425978, 0.00543046};  //a=8, b=1000
	//double x[] = {0.818399, 0.104819, 0.247511, 0.103147, 0.296683, 0.612121,
				  //0.439308, 0.400186, 0.661319, 0.27858};  //a=2, b=3
	//a=5, b=100,000
	//double x[] = {0.000033062, 0.0000262515, 0.0000653424, 0.0000226177, 9.5677e-6,
		          //0.000100915, 0.0000748653, 0.0000519455, 0.0000528867, 0.0000379983};
	//double x[] = {0.0000329651, 0.000100823, 0.0000218721, 0.0000304915, 0.0000543914,
		          //0.0000594646, 0.0000556585, 0.0000298259, 0.000105124, 0.000052207,
	              //0.0000493865, 0.0000301947, 0.0000161342, 0.0000822513, 0.000057034,
				  //0.0000492651, 0.0000365196, 0.0000574114, 0.0000509868, 0.0000299291};

	//a=0.5, b=0.2
	double x[] = {0.918786, 0.866101, 0.354743, 0.997641, 0.742154, 0.999964, 0.180845,
		          0.999952, 0.998693, 1.};
					

	int points = 10;
	double *params = new double[2];

	params = FitBeta(points, x);
	cout << "Alpha = " << params[0] << endl << "Beta = " << params[1];
	delete [] params;
}


