#ifndef RDATSRV_NONHOMOGENEOUS_H__
#define RDATSRV_NONHOMOGENEOUS_H__

#ifdef REDCASSRV_EXPORTS
#define REDCASSRV_API __declspec(dllexport)
#else
#define REDCASSRV_API __declspec(dllimport)
#endif

#undef redcas_numerical_NativeNonHomo_BETA_BINOMIAL
#define redcas_numerical_NativeNonHomo_BETA_BINOMIAL 601
#undef redcas_numerical_NativeNonHomo_GAMMA_POISSON
#define redcas_numerical_NativeNonHomo_GAMMA_POISSON 602
#undef redcas_numerical_NativeNonHomo_LOGNORM_POISSON
#define redcas_numerical_NativeNonHomo_LOGNORM_POISSON 603
#undef redcas_numerical_NativeNonHomo_LOGNORM_BINOMIAL
#define redcas_numerical_NativeNonHomo_LOGNORM_BINOMIAL 604

#undef redcas_numerical_NativeNonHomo_DEFAULT_SAMPLE_SIZE
#define redcas_numerical_NativeNonHomo_DEFAULT_SAMPLE_SIZE 5000L
#undef redcas_numerical_NativeNonHomo_DPDMODE_LINEAR_INTERVAL
#define redcas_numerical_NativeNonHomo_DPDMODE_LINEAR_INTERVAL 1L
#undef redcas_numerical_NativeNonHomo_DPDMODE_LOGARITHMIC_INTERVAL
#define redcas_numerical_NativeNonHomo_DPDMODE_LOGARITHMIC_INTERVAL 2L
#undef redcas_numerical_NativeNonHomo_MARGINALMODE_LINEAR_INTERVAL
#define redcas_numerical_NativeNonHomo_MARGINALMODE_LINEAR_INTERVAL 1L
#undef redcas_numerical_NativeNonHomo_MARGINALMODE_LOGARITHMIC_INTERVAL
#define redcas_numerical_NativeNonHomo_MARGINALMODE_LOGARITHMIC_INTERVAL 2L
#undef redcas_numerical_NativeNonHomo_MARGINAL_ALPHA
#define redcas_numerical_NativeNonHomo_MARGINAL_ALPHA 0L
#undef redcas_numerical_NativeNonHomo_MARGINAL_BETA
#define redcas_numerical_NativeNonHomo_MARGINAL_BETA 1L
#undef redcas_numerical_NativeNonHomo_BETA_BINOMIAL
#define redcas_numerical_NativeNonHomo_BETA_BINOMIAL 1L
#undef redcas_numerical_NativeNonHomo_GAMMA_POISSON
#define redcas_numerical_NativeNonHomo_GAMMA_POISSON 2L
#undef redcas_numerical_NativeNonHomo_LOGNORM_LOGNORM
#define redcas_numerical_NativeNonHomo_LOGNORM_LOGNORM 3L
#undef redcas_numerical_NativeNonHomo_NORMAL_NORMAL
#define redcas_numerical_NativeNonHomo_NORMAL_NORMAL 4L
#undef redcas_numerical_NativeNonHomo_LOGNORM_POISSON
#define redcas_numerical_NativeNonHomo_LOGNORM_POISSON 5L
#undef redcas_numerical_NativeNonHomo_LOGNORM_BINOMIAL
#define redcas_numerical_NativeNonHomo_LOGNORM_BINOMIAL 6L
#undef redcas_numerical_NativeNonHomo_MEASURE_CDF
#define redcas_numerical_NativeNonHomo_MEASURE_CDF 1L
#undef redcas_numerical_NativeNonHomo_MEASURE_PDF
#define redcas_numerical_NativeNonHomo_MEASURE_PDF 2L
#undef redcas_numerical_NativeNonHomo_MEASURE_MEAN
#define redcas_numerical_NativeNonHomo_MEASURE_MEAN 3L
#undef redcas_numerical_NativeNonHomo_MEASURE_VARIANCE
#define redcas_numerical_NativeNonHomo_MEASURE_VARIANCE 4L
#undef redcas_numerical_NativeNonHomo_MEASURE_QUANTILE
#define redcas_numerical_NativeNonHomo_MEASURE_QUANTILE 5L
#undef redcas_numerical_NativeNonHomo_PARAMETER_DEFAULT
#define redcas_numerical_NativeNonHomo_PARAMETER_DEFAULT 3L
#undef redcas_numerical_NativeNonHomo_PRIOR_MEAN_STD_ERR
#define redcas_numerical_NativeNonHomo_PRIOR_MEAN_STD_ERR 1L
#undef redcas_numerical_NativeNonHomo_PRIOR_MEDIAN_EF
#define redcas_numerical_NativeNonHomo_PRIOR_MEDIAN_EF 2L
#undef redcas_numerical_NativeNonHomo_PRIOR_MEDIAN_SIGMA
#define redcas_numerical_NativeNonHomo_PRIOR_MEDIAN_SIGMA 3L

#undef redcas_PRIOR_MEAN_STD_ERR
#define redcas_PRIOR_MEAN_STD_ERR 1
#undef redcas_PRIOR_MEDIAN_EF
#define redcas_PRIOR_MEDIAN_EF 2

#undef redcas_ErrorCode_InvalidHandle 
#define redcas_ErrorCode_InvalidHandle -1

class CRedcassrvNonHomo {
public:
	CRedcassrvNonHomo(void);
	static CRedcassrvNonHomo * getInstance() {return &instance;}

private:
	static CRedcassrvNonHomo instance;

public:
	int		createSession();
	void	destroySession(int iHandle);
	void	setModelN(int sessionID, int model, double pp11,
				double pp12, double pp21, double pp22);
	int		runAnalysis(int iHandle, int iSamples);
	void	addDataPoint(int sessionID, double x_star, double tau, double events,
			double exposure);
	int		getProgress(int iHandle);
	int		getStatusCode(int iHandle);
	void	requestCancel(int iHandle);

	double	getPopVarMeanMean(int iHandle);
	double	getPopVarVarianceMean(int iHandle);
	double	getPopVarMeanPercentile(int iHandle, double p);
	double	getPopVarPercentilePercentile(int iHandle, double z, double p);

	double	getPopVarCDF(int iHandle, double x, double p);
	double	getPopVarPDF(int iHandle, double x, double p);
	double	getPopVarCDFMean(int iHandle, double x);
	double	getPopVarPDFMean(int iHandle, double x);

	/* returns the number of samples contained in the sampleset */
	int getSampleCount(int iHandle);

	bool constructBins(int iHandle, int size, bool logScale);

	/* returns the density of given cell in binned sample space */
	double getBinnedAlpha(int iHandle, int index);
	double getBinnedBeta(int iHandle, int index);
	double getBinnedDensity(int iHandle, int alpha, int beta);

	int getMarginalSize(int iHandle);
	int getMarginalAlpha(int iHandle, int size, bool bLogScale);
	int getMarginalBeta(int iHandle, int size, bool bLogScale);

	double getUpperBound(int iHandle);
	double getLowerBound(int iHandle);

	int clearData(int iHandle);
	int clearSamplesN(int iHandle);
};

#endif