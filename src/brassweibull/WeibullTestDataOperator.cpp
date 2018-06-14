// $Id: WeibullTestDataOperator.cpp 169 2005-01-28 14:17:28Z saulo $

#include "WeibullTestDataOperator.h"

#include <math.h>
#include "WeibullModelLayout.h"

/**
 * Constructs a new operator.
 * The operator computes the Weibull likelihood for test data.
 * Test data sets are shared between test data operators.
 * Failure mode numbering starts at 1.
 * @param data test data set
 * @param alpha scale parameter index
 * @param beta shape parameter index 
 * @param mode failure mode 
 * @todo <b>Must still allow for undefined failure modes.</b>
 */
WeibullTestDataOperator::WeibullTestDataOperator(TestData * data, TestConditions * conditions, const int node_cnt, const int mode_cnt, const int node, const int mode)
{
	this->is_debug = false;

	this->data = data->obtain();
	this->conditions = conditions->obtain();
	this->mode = mode;
	this->node = node;
	this->mode_cnt = mode_cnt;
	this->node_cnt = node_cnt;

	this->alpha = WeibullModelLayout::getScaleIndex(node,mode,node_cnt);
	this->beta = WeibullModelLayout::getShapeIndex(mode,node_cnt);

	this->weighted_sumlnt = 0;
	this->weighted_failures = 0;

	int record_count = data->getRecordCount();
	for (int i = 0 ; i < record_count ; i++) 
	{
		const TestDataRecord * rec = data->getRecord(i);
		if (rec->mode == mode)
		{	
			// compute failed weight for the record to account for (1) design credit (2) strength
			double failed_weight = rec->weight * (1 - rec->confidence);
			weighted_failures += failed_weight;
			weighted_sumlnt += failed_weight * log(rec->end);
		} else if (rec->mode == TestData::MODE_ANY) {
			// DO NOTHING
		}
	}
}

/**
 * Destructor.
 */
WeibullTestDataOperator::~WeibullTestDataOperator(void)
{
	if (data != 0 && data->release()) delete data;
	if (conditions != 0 && conditions->release()) delete conditions;
}

/**
 * Computes the value for this operator.
 */
double WeibullTestDataOperator::compute(float * params, float * derivs) const
{
	int record_count = data->getRecordCount();

	double a = exp(params[alpha]);
	double b = params[beta];
	double ln_dens = 0;

	if (is_debug) {
		cout << "*****************************" << endl;
		cout << alpha << "\t" << beta  << "\t" << a << "\t" << b << endl;
	}

	for (int i = 0 ; i < record_count ; i++) 
	{
		if (is_debug) cout << i << "\t";

		const TestDataRecord * rec = data->getRecord(i);
		const AccelerationFactor * acc = conditions->getAcceleration(rec->condition,mode);
		double fac = acc->factor;
		double ln_recdens = 0; // log density for this record

		switch (acc->type) 
		{
			case AccelerationFactor::ACCELERATE_NONE:
				ln_recdens = pow(rec->start / a , b) - pow(rec->end / a , b);
				break;

			case AccelerationFactor::ACCELERATE_RATE:
				ln_recdens = fac * (pow(rec->start / a , b) - pow(rec->end / a , b)); 
				if (rec->mode == mode) ln_recdens += log(fac) * (1 - rec->confidence);
				break;

			case AccelerationFactor::ACCELERATE_TIME:
				ln_recdens = pow(fac * rec->start / a , b) - pow(fac * rec->end / a , b);
				if (rec->mode == mode) ln_recdens += b * log(fac) * (1 - rec->confidence);
				break;

			case AccelerationFactor::ACCELERATE_NULL:
				break;
		}

		if (is_debug) cout << ln_recdens << "\t";

		if (rec->mode == TestData::MODE_ANY) {

			double lambda = 0;
			double lambda_tot = 0;

			for (int mode = 1 ; mode <= mode_cnt ; mode++) {

				double ai = exp(params[WeibullModelLayout::getScaleIndex(node,mode,node_cnt)]);
				double bi = params[WeibullModelLayout::getShapeIndex(mode,node_cnt)];
				acc = conditions->getAcceleration(rec->condition,mode);
				double faci = acc->factor;

				switch (acc->type)
				{
					case AccelerationFactor::ACCELERATE_NONE:
						lambda = bi * pow(rec->end / ai,bi - 1) / ai;
						break;
					case AccelerationFactor::ACCELERATE_RATE:
						lambda = bi * pow(rec->end / ai,bi - 1) * faci / ai;
						break;
					case AccelerationFactor::ACCELERATE_TIME:
						lambda = bi * pow(faci * rec->end / ai,bi - 1) * faci / ai;
						break;
					case AccelerationFactor::ACCELERATE_NULL:
						lambda = 0;
						break;
				}
				lambda_tot += lambda;
			}

			if (is_debug) cout << "LTOT: " << lambda_tot << "\t";

			ln_recdens += log(lambda_tot) * (1 - rec->confidence);
		}
		ln_dens += ln_recdens * rec->weight;

		if (is_debug) cout << " : " << ln_dens << endl;
	}

	// Add terms for failures of corresponding failure mode.
	// Parameters 'failures' and 'sumlnt' include corrections for design 
	// credit and data strength.
	ln_dens += weighted_failures * (log(b) - b * log(a));
	ln_dens += weighted_sumlnt * (b - 1);
	
	return ln_dens;
}

/**
 * Creates an unlinked copy of the operator.
 */
NetOperatorPtr WeibullTestDataOperator::copy() const 
{
	WeibullTestDataOperator * op = new WeibullTestDataOperator(data,conditions,node_cnt,mode_cnt,node,mode);
	op->operator_id = this->operator_id;
	op->next = 0;
	return op;
}

bool WeibullTestDataOperator::isDensity(const int index) const
{
	if (this->appliesTo(index)) return true;
	if (next != 0) return next->isDensity(index);
	return false;
}