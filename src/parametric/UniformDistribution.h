#pragma once
#include "parametricdistribution.h"

class UniformDistribution :
	public ParametricDistribution
{
public:
	UniformDistribution(void);
	~UniformDistribution(void);

	virtual double getSample() const;
	virtual double getMean() const;
	virtual double getQuantile(double x) const;
	virtual double getCDF(double x) const;
	virtual double getLogPDF(double x) const;
	virtual double getVariance() const;
};
