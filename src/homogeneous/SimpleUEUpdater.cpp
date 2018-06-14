// SimpleUEUpdater.cpp: implementation of the SimpleUEUpdater class.
//
//////////////////////////////////////////////////////////////////////


#include "SimpleUEUpdater.h"
#include "LogNormalPoissonDistribution.h"
#include "LogNormalBinomialDistribution.h"
#include "UpdatedTabularDistribution.h"
#include "TabularPoissonDistribution.h"
#include "TabularBinomialDistribution.h"
#include "../parametric/BetaDistribution.h"
#include "../parametric/GammaDistribution.h"

#include <math.h>
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
#define max(a, b)  (((a) > (b)) ? (a) : (b))

///////////////////////////////////////////////////////////////////////

class TabularEntry {
public:
	TabularEntry(double val, double prob, TabularEntry * nxt);
	~TabularEntry();
	double value;
	double cdf;
	TabularEntry * next;
};

TabularEntry::TabularEntry(double val, double prob, TabularEntry *  nxt) 
{
	value = val;
	cdf = prob;
	next = nxt;
}

TabularEntry::~TabularEntry() 
{
	TabularEntry * entry = this->next;
	while (entry) {
		TabularEntry * nextentry= entry->next;
		entry->next = 0;
		delete entry;
		entry = nextentry;
	}
}

UEKTEntry::UEKTEntry(int flrs, double tm, double wght, UEKTEntry * nxt) :
   failures(flrs) , time(tm) , weight(wght) , next(nxt) {
   posterior = 0;   
}

UEKTEntry::~UEKTEntry() 
{
	UEKTEntry * entry = this->next;
	while (entry) {
		UEKTEntry * nextentry= entry->next;
		entry->next = 0;
		delete entry;
		entry = nextentry;
	}
	delete posterior;
}

//////////////////////////////////////////////////////////////////////

SimpleUEUpdater::SimpleUEUpdater()
{
	entries = 0;
	cdfArray = 0;
	valArray = 0;
}

SimpleUEUpdater::~SimpleUEUpdater()
{
	delete entries;
	delete [] valArray;
	delete [] cdfArray;
}

/**
 * Adds an interpretation for use in the updating procedure.
 * This method is used to add data both for Poisson type (failures/time) and
 * Binomial type (failures/demands) data.
 * 
 * INPUTS:
 *  events   : number of failures
 *  exposure : time or number of demands
 *  weight   : weight of the interpretation (weight > 0)
 */
void SimpleUEUpdater::addInterpretation(int events, double exposure, double weight)
{
	entries = new UEKTEntry(events,exposure,weight,entries);
}

/**
 * Sets the parameters of the prior distribution.
 * These values must correspond to the default parameter types of the 
 * distributions used:
 *  GAMMA     : alpha,lambda (mean = alpha / lambda)
 *  BETA      : alpha,beta
 *  LOGNORMAL : nu,tau
 */
void SimpleUEUpdater::setPrior(double par1, double par2)
{
	this->parameter1 = par1;
	this->parameter2 = par2;
}

void SimpleUEUpdater::setPrior(double * vals, double * cdfs, int size)
{
	this->tabularSize = size;

	valArray = new double[size];
	cdfArray = new double[size];

	for (int i = 0 ; i < size ; i++)
	{
		valArray[i] = vals[i];
		cdfArray[i] = cdfs[i];
	}
}

/**
 * Normalizes the weights of the interpretations.
 */
void SimpleUEUpdater::normalizeInterpretations()
{ 
	double sum = 0;
	UEKTEntry * entry = entries;
	while (entry) {
		sum += entry->weight;
		entry = entry->next;
	}

    entry = entries;
	while (entry) {
		entry->probability = entry->weight / sum;
		entry = entry->next;
	}
}

/**
 * Returns the number of interpretations.
 */
int SimpleUEUpdater::getInterpretationCount() {
	int cnt = 0;
	UEKTEntry * entry = entries;
	while (entry) {
		cnt ++;
		entry = entry->next;
	}
	return cnt;
}

/**
 * Constructs the weighted posterior, based on the prior distribution and the
 * interpretation data.
 *
 * The procedure first constructs posterior distributions for each of the
 * interpretations. Then, it uses a recursive procedure to construct a table 
 * of <x,CDF(x)> pairs.
 *
 * The method returns 0 if the CDF was constructed successfully.
 * 
 * INPUTS:
 *   pair     : specification of update pair
 *   xArray   : array in which to store x values
 *   cdfArray : array in which to store cdf values
 *   size     : size of the arrays
 */
int SimpleUEUpdater::constructCDF(UEPair pair,double * xArray, double * cdfArray, int size)
{
	LogNormalPoissonDistribution * lnp;
	LogNormalBinomialDistribution * lnb;
	UpdatedTabularDistribution * upt;
	GammaDistribution * gamma;
	BetaDistribution * beta;
	double * cdfs;
	double * vals;

	double lower = 1e300;
	double upper = 0;
	double loweri;
	double upperi;
	
	normalizeInterpretations();

	// construct posterior distributions
	UEKTEntry * entry = entries;
	while (entry) {

		double k = entry->failures;
		double t = entry->time;
		switch (pair) {
		case UE_LOGNORMAL_POISSON :
			lnp = new LogNormalPoissonDistribution(parameter1,parameter2,k,t);
			loweri = lnp->getLowerBound();
			upperi = lnp->getUpperBound();
			entry->posterior = lnp;
			break;
		case UE_GAMMA_POISSON : 
			cout << "Constructing gamma " << 1. / (1. / parameter2 + t) << " " << parameter1 + k << endl;
			gamma = new GammaDistribution(1. / (1. / parameter2 + t) , parameter1 + k);
			loweri = gamma->getQuantile(0.001);
			upperi = gamma->getQuantile(0.999);
			cout << "Bounds : " << loweri << " " << upperi << endl;
			entry->posterior = gamma;
			break;
		case UE_BETA_BINOMIAL :
			beta = new BetaDistribution(parameter1 + k, parameter2 + t - k);
			loweri = beta->getQuantile(0.001);
			upperi = beta->getQuantile(0.999);
			entry->posterior = beta;
			break;
		case UE_LOGNORMAL_BINOMIAL :
			lnb = new LogNormalBinomialDistribution(parameter1,parameter2,k,t);
			loweri = lnb->getLowerBound();
			upperi = lnb->getUpperBound();
			entry->posterior = lnb;
			break;
		case UE_TABULAR_POISSON : 
			vals = copyDoubleArray(this->valArray,tabularSize);
			cdfs = copyDoubleArray(this->cdfArray,tabularSize);
			upt = new TabularPoissonDistribution(vals,cdfs,tabularSize,k,t);
			loweri = upt->getLowerBound();
			upperi = upt->getUpperBound();
			entry->posterior = upt;
			break;
		case UE_TABULAR_BINOMIAL :
			vals = copyDoubleArray(this->valArray,tabularSize);
			cdfs = copyDoubleArray(this->cdfArray,tabularSize);
			upt = new TabularBinomialDistribution(vals,cdfs,tabularSize,k,t);
			loweri = upt->getLowerBound();
			upperi = upt->getUpperBound();
			entry->posterior = upt;
			break;
		}

		lower = min(lower,loweri);
		upper = max(upper,upperi);

		entry = entry->next;
	}

	cout << "Construction of distributions complete" << endl;

	// recursively construct a list of <x,cdf(x)> pairs.
	TabularEntry * tabular = new TabularEntry(upper,computeCDF(entries,upper),0);
	tabular = new TabularEntry(lower,computeCDF(entries,lower),tabular);

	cout << "starting recursion" << endl;

	constructCDFr(tabular,entries, 0.25 / size);

	cout << "recursive procedure complete" << endl;
	cout << "constructing table" << endl;

	// search for the first x value to be stored in the table
	TabularEntry * current = tabular;
	while (current->next->cdf < 1. / (10 * size)) current = current->next;
	xArray[0] = current->value;
	cdfArray[0] = 0;

	// select appropriate values from the list to be stored in the table
	for (int i = 1 ; i < size - 1 ; i++) {
		while (current->cdf < 1. * i / (size - 1) && current->next->next) current = current->next;
		xArray[i] = current->value;
		cdfArray[i] = current->cdf;
	}

	cout << "almost done " << endl;

	// search for the last x value to be stored in the table
	while (current->cdf < 1 - 1. / (10 * size) && current->next) current = current->next;
	xArray[size - 1] = current->value;
	cdfArray[size - 1] = 1; 

	cout << "completely done" << endl;

	// clean up the list and posteriors
	delete tabular;
	entry = entries;
	while (entry) {
		delete entry->posterior;
		entry->posterior = 0;
		entry = entry->next;
	}

	cout << "done" << endl;

	return 0;
}

/**
 * Recursively constructs the CDF table. Each recursive call introduces a new 
 * data point in between the point specified by 'tabular' and its immediate 
 * successor in the list. The recursion is terminated when the difference 
 * in CDF values between successive points in the table is smaller than
 * maxDelta.
 */
void SimpleUEUpdater::constructCDFr(TabularEntry * tabular, UEKTEntry * entries, double maxDelta)
{
//	double x = sqrt(tabular->value * tabular->next->value);
	double x = (tabular->value + tabular->next->value) / 2;
	tabular->next = new TabularEntry(x,computeCDF(entries,x),tabular->next);
	// recur if too big a jump exists in the CDF values.
	if (tabular->next->next->cdf - tabular->next->cdf > maxDelta)
		constructCDFr(tabular->next,entries,maxDelta);
	if (tabular->next->cdf - tabular->cdf > maxDelta)
		constructCDFr(tabular,entries,maxDelta);
}

/**
 * Computes the CDF value of the weighted posterior at x.
 */
double SimpleUEUpdater::computeCDF(UEKTEntry * entries,double x) {

	double cdf = 0;
	UEKTEntry * entry = entries;
	while (entry) {
		cdf += entry->posterior->getCDF(x) * entry->probability;
		entry = entry->next;
	}
	return cdf;
}

double * SimpleUEUpdater::copyDoubleArray(double * inarr,int size)
{
	double * outarr = new double[size];
	for (int i = 0 ; i < size ; i++) 
		outarr[i] = inarr[i];
	return outarr;
}


