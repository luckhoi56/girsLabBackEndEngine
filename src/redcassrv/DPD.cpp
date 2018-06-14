
#include "DPD.h"
#include "Math.h"

DPD::~DPD(void)
{
	if (probability != NULL) {
		delete probability;
	}
	if (value != NULL) {
		delete value;
	}
}

DPD::DPD(double * valueInput, double * probabilityInput, int points) {
	m_iPoints = points;
	probability = new double[points];
	value = new double[points];
	sorted = false;
	normalized = false;

	for (int i = 0; i < points; i++) {
		probability[i] = probabilityInput[i];
		value[i] = valueInput[i];
	}
}

DPD::DPD(int points) {
	m_iPoints = points;
	probability = new double[points];
	value = new double[points];
	sorted = false;
	normalized = false;
}

/**
 * Returns the number of elements in the DPD.
 * 
 * @return number of elements
 */
int DPD::size() {
	return m_iPoints;
}

/**
 * Gets the value corresponding to the index.
 * <p>
 * Index counts starting from 0.
 */
double DPD::getValue(int index) const {
	if (value != NULL)
		return value[index];
	return 0;
}

/**
 * Returns true if value is defined. This implementation does not have the
 * capability to tell if a value has been defined. Always returns true.
 */
bool DPD::isValueDefined(int index) {
	return true;
}

/**
 * Returns true if probability is defined. This implementation does not have
 * the capability to tell if a value has been defined. It always returns
 * true.
 */
bool DPD::isProbabilityDefined(int index) {
	return true;
}

/**
 * Gets the probability associated with the value corresponding to the
 * index.
 * 
 * @see #getValue(int)
 */
double DPD::getPDF(double x) const {
	int index = (int)x;
	if (probability != NULL)
		return probability[index];
	return 0;
}

/**
 * Returns the cumulative probability at given index. It is not defined how
 * this function responds if the sample set is not sorted or not normalized.
 * 
 * @return cumulative probability
 * @see #isSorted()
 * @see #sort()
 */
double DPD::getCDF(double x) const {
	double sum = 0;
	int index = (int)x;
	for (int i = 0; i <= index; i++) {
		sum += getPDF(i);
	}
	return sum;
}

/**
 * Get the mean of the distribution with parameters. It is not defined what
 * this function returns if distribution is not normalized.
 */
double DPD::getMean() const {

	double sum = 0.;
	double prob = 0.;

	for (int i = 0; i < m_iPoints; i++) {
		sum += probability[i] * value[i];
		prob += probability[i];
	}

	if (prob > 0) {
		return sum / prob;
	}
	return 0;
}

/**
 * Get a random sample from the distribution. <b> not implemented </b>
 */
double DPD::getSample() {
	return 0;
}


/**
 * Returns true if DPD is in a normalized state, i.e. the cumulative
 * probability at the last index equals 1.00.
 */
bool DPD::isNormalized() {
	return normalized;
}

/**
 * Normalizes the probabilities in the DPD.
 */
void DPD::normalize() {

	double sum = getCDF(m_iPoints - 1);

	if (sum > 0 && !normalized) {

		for (int i = 0; i < m_iPoints ; i++) {
			probability[i] = probability[i] / sum;
		}

		if (m_iPoints > 1) {
			probability[m_iPoints - 1] = getCDF(m_iPoints - 2);
		}

		normalized = true;
	}
}

/**
 * Returns true if values in DPD are sorted in ascending order.
 */
bool DPD::isSorted() {
	return sorted;
}

/**
 * Sorts the values in the DPD in ascending order.
 */
void DPD::sort() {
	Sort(value, probability, m_iPoints);
	sorted = true;
}

// implement the sort function for public method sort()
void DPD::Sort(double data[], double prob[], int size) {
	double tmp;

	if (size == 1) {
	} else if (size == 2) {
		if (data[0] > data[1]) {
			tmp = data[0];
			data[0] = data[1];
			data[1] = tmp;
			tmp = prob[0];
			prob[0] = prob[1];
			prob[1] = tmp;
		}
	} else {
		int size1 = size / 2;
		int size2 = size - size1;
		double * data1 = new double[size1];
		double * prob1 = new double[size1];
		double * data2 = new double[size2];
		double * prob2 = new double[size2];

		for (int i = 0; i < size1; i++) {
			data1[i] = data[i];
			prob1[i] = prob[i];
		}
		for (int i = 0; i < size2; i++) {
			data2[i] = data[i + size1];
			prob2[i] = prob[i + size1];
		}

		Sort(data1, prob1, size1);
		Sort(data2, prob2, size2);

		int pointA = 0;
		int pointB = 0;
		int pointC = 0;

		double * data3 = new double[size];
		double * prob3 = new double[size];

		while (pointA < size1 && pointB < size2 && pointC < size) {
			while (pointA < size1 && pointB < size2
					&& data1[pointA] <= data2[pointB]) {
				data3[pointC] = data1[pointA];
				prob3[pointC] = prob1[pointA];
				pointC++;
				pointA++;
			}
			while (pointA < size1 && pointB < size2
					&& data1[pointA] > data2[pointB]) {
				data3[pointC] = data2[pointB];
				prob3[pointC] = prob2[pointB];
				pointC++;
				pointB++;
			}
		}

		if (pointA < size1) {
			while (pointC < size) {
				data3[pointC] = data1[pointA];
				prob3[pointC] = prob1[pointA];
				pointC++;
				pointA++;
			}
		} else if (pointB < size2) {
			while (pointC < size) {
				data3[pointC] = data2[pointB];
				prob3[pointC] = prob2[pointB];
				pointC++;
				pointB++;
			}
		}

		for (int i = 0; i < size; i++) {
			data[i] = data3[i];
			prob[i] = prob3[i];
		}

		delete data3;
		delete data2;
		delete data1;
		delete prob1;
		delete prob2;
		delete prob3;
	}
}

/**
 * Sets the value for the specified entry.
 */
void DPD::setValue(int index, double valueInput) {
	value[index] = valueInput;
	sorted = false;
}

/**
 * Sets the probability for the specified entry.
 */
void DPD::setProbability(int index, double prob) {
	probability[index] = prob;
	normalized = false;
}

/**
 * Clears the probability of specified element. Probability is set to 0.
 */
void DPD::clearProbability(int index) {
	probability[index] = 0;
	normalized = false;
}

/**
 * Clears the value of specified element. Value is set to 0.
 */
void DPD::clearValue(int index) {
	value[index] = 0;
	sorted = false;
}

void DPD::setSize(int size) {
	if (value != NULL) {
		delete value;
	}
	if (probability != NULL) {
		delete probability;
	}
	value = new double[size];
	probability = new double[size];
	sorted = false;
	normalized = false;
	m_iPoints = size;
}

double DPD::getQuantile(double cumulative) const {

	if (!sorted || !normalized)
		return 0;

	double sum = 0;
	int i;

	for (i = 0; i < m_iPoints; i++) {
		sum += probability[i];
		if (sum > cumulative) {
			return getValue(i);
		}
	}

	return 0;
}

double DPD::getVariance() const {

	double sum = 0.;
	double prob = 0.;

	if (!sorted || !normalized)
		return 0;

	double mean = getMean();

	for (int i = 0; i < m_iPoints; i++) {
		sum += probability[i] * pow(value[i] - mean, 2);
		prob += probability[i];
	}

	if (prob > 0) {
		return sum / prob;
	}

	return 0;
}

void DPD::fillSampleArray(double * values, int size) const
{
}

double DPD::getSample() const
{
	return 0;
}

double DPD::getMedian() const
{
	int index = (int)(m_iPoints/2);
	return value[index];
}

double DPD::getLogPDF(double x) const
{
	return 0;
}

void   DPD::releaseMemory()
{
	if (probability != NULL) {
		delete probability;
	}
	probability = NULL;
	if (value != NULL) {
		delete value;
	}
	value = NULL;
}

double DPD::getX(double x) const {
	return getValue((int)x);
}