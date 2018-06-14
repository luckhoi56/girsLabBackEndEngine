#pragma once
#include "genericdistribution.h"

class DPD :
	public GenericDistribution
{
public:
	DPD(void);
	DPD(double * valueInput, double * probabilityInput, int points);
	DPD(int points);
	~DPD(void);
private:
	double * probability;
	double * value;
	bool	sorted;
	bool	normalized;
	int		m_iPoints;

public:
	virtual void fillSampleArray(double * values, int size) const;
	virtual double getSample() const;
	virtual double getMedian() const;
	virtual double getMean() const;
	virtual double getQuantile(double x) const;
	virtual double getCDF(double x) const;
	virtual double getPDF(double x) const;
	virtual double getLogPDF(double x) const;
	virtual double getVariance() const;
	virtual void   releaseMemory();
	virtual double getX(double x) const;

	int		size();
	double	getValue(int index) const;
	bool	isValueDefined(int index);
	bool	isProbabilityDefined(int index);
	double	getSample();
	bool	isNormalized();
	void	normalize();
	bool	isSorted();
	void	sort();
	void	Sort(double data[], double prob[], int size);
	void	setValue(int index, double valueInput);
	void	setProbability(int index, double prob);
	void	clearProbability(int index);
	void	clearValue(int index);
	void	setSize(int size);
};
