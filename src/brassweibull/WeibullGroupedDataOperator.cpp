// $Id: WeibullGroupedDataOperator.cpp 169 2005-01-28 14:17:28Z saulo $

#include "WeibullGroupedDataOperator.h"
#include "WeibullModelLayout.h"

#include <math.h>
#include <stdlib.h>

/**
 * Constructor.
 * @param data data set
 * @param node_cnt number of nodes in the model
 * @param mode_cnt number of modes in the model
 * @param node index of the node to which data applies
 * @param mode index of the mode to which data applies.
 */
WeibullGroupedDataOperator::WeibullGroupedDataOperator(GroupedData * data, 
	const int node_cnt, const int mode_cnt, const int node, const int mode)
{
	this->data = data->obtain();
	this->mode = mode;
	this->node = node;
	this->mode_cnt = mode_cnt;
	this->node_cnt = node_cnt;

	this->alpha = WeibullModelLayout::getScaleIndex(node,mode,node_cnt);
	this->beta = WeibullModelLayout::getShapeIndex(mode,node_cnt);

	int len = data->getRecordCount();
	cum_arr = new double[len];
}

/**
 * Destructor.
 */
WeibullGroupedDataOperator::~WeibullGroupedDataOperator(void)
{
	if (data != 0 && data->release()) delete data;
	delete [] cum_arr;
}

/**
 * Returns true if this operator applies to the specified parameter.
 * @param index index of the parameter.
 */
bool WeibullGroupedDataOperator::appliesTo(const int index) const 
		{return alpha == index || beta == index;}

/**
 * Computes the value for this operator.
 * For details, see Section 4 of the likelihood specification document.
 */
double WeibullGroupedDataOperator::compute(float * params, float * derivs) const
{
	int record_count = data->getRecordCount();
	int data_mode = data->getMode();
	double ln_dens = 0;
	double a,b,k,n;

	if (data_mode != -1) {
		// data set applies to single failure mode
		// compute cumulative failure rate values for each record
		a = exp(params[alpha]);
		b = params[beta];
		for (int i = 0 ; i < record_count ; i++) {
			const GroupedDataRecord * rec = data->getRecord(i);
			cum_arr[i] = pow(rec->end/a,b) - pow(rec->start/a,b);
		}
	} else {
		// data set applies to all failure modes
		// compute cumulative failure rate values for each record
		for (int i = 0 ; i < record_count ; i++) cum_arr[i] = 0;
		for (int mode = 1 ; mode <= mode_cnt ; mode++) {
			a = exp(params[WeibullModelLayout::getScaleIndex(node,mode,node_cnt)]); //
			b = params[WeibullModelLayout::getShapeIndex(mode,node_cnt)];
			for (int i = 0 ; i < record_count ; i++) {
				const GroupedDataRecord * rec = data->getRecord(i);
				cum_arr[i] += pow(rec->end/a,b) - pow(rec->start/a,b);
			}
		}
	}

	if (data->getRepair() == data->REPAIR_IMPERFECT) {
		// imperfect repair: Poisson likelihood
		for (int i = 0 ; i < record_count ; i++) {
			const GroupedDataRecord * rec = data->getRecord(i);
			if (cum_arr[i] != 0) {
				cum_arr[i] *= rec->population * rec->weight;
				k = rec->events * rec->weight;
				ln_dens += k * log(cum_arr[i]) - cum_arr[i];
			}
		}
	} else {
		// no repair: Binomial likelihood
		for (int i = 0 ; i < record_count ; i++) {
			const GroupedDataRecord * rec = data->getRecord(i);
			if (cum_arr[i] != 0) {
				k = rec->events;
				n = rec->population;
				if (cum_arr[i] > 10) {
					double lnq = -cum_arr[i];
					double q = exp(lnq);
					ln_dens += -k * q + (n - k) * lnq;
				} else if (cum_arr[i] < 0.001) {
					// taylor series approximation of 1 - Q
					double lnq = -cum_arr[i];
					double omq = cum_arr[i] - cum_arr[i] * cum_arr[i] / 2;
					ln_dens += k * log(omq) + (n - k) * lnq;
				} else {
					double lnq = -cum_arr[i];
					double lnomq = log(1 - exp(lnq));
					ln_dens += k * lnomq + (n - k) * lnq;
				}
			}
		}
	}

	return ln_dens;
}

/**
 * Creates an unlinked copy of the operator.
 */
NetOperatorPtr WeibullGroupedDataOperator::copy() const 
{
	WeibullGroupedDataOperator * op = new WeibullGroupedDataOperator(data,
		node_cnt,mode_cnt,node,mode);
	op->operator_id = this->operator_id;
	op->next = 0;
	return op;
}

/**
 * Implementation of superclass method.
 */
bool WeibullGroupedDataOperator::isDensity(const int index) const
{
	if (this->appliesTo(index)) return true;
	if (next != 0) return next->isDensity(index);
	return false;
}
