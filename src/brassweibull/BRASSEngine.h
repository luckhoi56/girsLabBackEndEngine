// $Id: BRASSEngine.h 2150 2013-08-04 04:03:53Z Dongfeng Zhu $

#pragma once

#include "../core/ParamArray.h"
#include "../core/TestConditions.h"
#include "../core/TestData.h"
#include "../core/GroupedData.h"
#include "../core/TimedMeasure.h"
#include "ProportionalModelOperator.h"
#include "WeibullModelLayout.h"
#include "../core/JointDensityEstimate.h"
#include "../core/WeibullStatus.h"
#include "../core/BRASSStreams.h"
#include "../core/BRASSError.h"

#include <string>
#include <vector>

using namespace std;

/**
 * This class serves as the central access point to all functionality. 
 *
 * The three major method categories are:
 *  - model initialization and definition
 *  - sampling	
 *  - retrieving results
 * The methods in this class can and should be used to access all 
 * functionality by the engine.
 *
 * <b>Recent modifications:</b>
 *  - Added stub methods for adding custom priors
 *  - Added functions for joint densities over any variable in the model
 *  - Added method that allows sample chains for single variables to be retrieved
 *
 * <b>IMPLEMENTATION ISSUES:</b>
 *
 *  - Engine must be able to specify which nodes to store results/samples for.
 *    THe GUI design calls for an ability by the user to specify which nodes to 
 *    store results for. This can lead to a reduction of the storage requirements.
 *    For now, this is ignored by the engine: all results are stored.
 *
 *  - Engine must be able to specify custom priors: specify the prior in terms of 
 *    of e.g., reliability bounds as a function of time (for single failure modes,
 *    or possibly complete competing FM model). Currently, parametric priors are 
 *    supported.
 *
 *  - Engine must have feature for import/export of sample chains, such that results
 *    can be permanently stored. This should also have the simulation settings.
 * 
 *  - Some of the functions in the engine are stubbed.
 * 
 */
class BRASSEngine
{
public:
	BRASSEngine(void);
	~BRASSEngine(void);
	int setup(int node_cnt, int mode_cnt, int condition_cnt);
	int setAcceleration(int mode, int condition, int type, double acceleration);

	/// use addProportionalRateModel instead.
	int addProportionalHazardModel(int node0, int node1, int mode, double median, double errfac)
	{return addProportionalRateModel(node0,node1,mode,median,errfac);}
	/// use addProportionalRateModel instead.
	int addProportionalHazardModel(int node0, int node1, double median, double errfac)
	{return addProportionalRateModel(node0,node1,median,errfac);}

	int addProportionalRateModel(int node0, int node1, int mode, double median, double errfac);
	int addProportionalRateModel(int node0, int node1, double median, double errfac);
	int addProportionalTimeModel(int node0, int node1, int mode, double median, double errfac);
	int addProportionalTimeModel(int node0, int node1, double median, double errfac);

	int addScalePrior(int node, int mode, double median, double errfac);
	int addShapePrior(int mode, double median, double errfac);
	int fixScaleParameter(int node, int mode, double value);
	int fixShapeParameter(int mode, double value);

	int createGroupedDataSet(const int record_cnt, const int mode = -1, const int repair = 5);
	int setGroupedDataRecord(const int index, const double start,
		const double end, const int failures, const int population);
	int attachGroupedDataSet(int node, double weight); 
	int attachGroupedDataSet(int node, int mode, double weight); 

	int createTestDataSet(int record_cnt);
	int setTestDataRecord(const int index, const double start, 
		const double end, const int mode, const int condition = 0, 
		double strength = 1, double credit = 0); 
	int attachTestDataSet(int node, double weight);
	int attachTestDataSet(int node, int mode, double weight);

	int loadTestDataSet(const char * file_name,int node, int mode, double weight); 
	int loadTestConditions(const char * file_name);

	int writeSamples(OutputStream & os);
	int readSamples(InputStream & is);

	int createCustomPrior(const int record_cnt, int measure);
	int setCustomPriorRecord(const int index, const double time, const double lo, 
									  const double hi, const double confidence);
	int attachCustomPrior(const int node, const int mode);

	int generateSamples(int samples, int chains, int interleave, int burnin, long seed, bool clear);

	int computeMeasure(int measure, int node, int mode, double time, double &mean, double &median, 
								double &p01, double &p05, double &p10, 
								double &p90, double &p95, double &p99);

	int computeAdjustment(int adjustment, int node0, int node1, int mode,
								double &mean, double &median, 
								double &p01, double &p05, double &p10, 
								double &p90, double &p95, double &p99);
	
	int computeParameter(int param, int node, int mode,
								double &mean, double &median, 
								double &p01, double &p05, double &p10, 
								double &p90, double &p95, double &p99);

	int computeConfidence(OperatorFunction * func, double val, double & confidence);

	int computeUncertainty(OperatorFunction * func,
								double &mean, double &median, 
								double &p01, double &p05, double &p10, 
								double &p90, double &p95, double &p99);

	double computeWeibullMTTF(double dShape, double dScale);

	int getChain(OperatorFunction * func, int start, int length, double * arr);

	int estimateJointParameterDensity(int node, int mode, bool log_scale, int draw_points);
	int estimateJointDensity(OperatorFunction * f1, OperatorFunction * f2, int draw_points);

	void clear();
	void clearAll();

	OperatorFunction * createParameterFunction(int param, int node, int mode) const;
	OperatorFunction * createMeasureFunction(int measure, int node, int mode, double time) const;
	OperatorFunction * createAdjustmentFunction(int adjustment, int node0, int node1, int mode) const;

	int getJointDensity(int x_index, int y_index, double & x, double & y, double & density) ;

	void setModeNames(const char ** names, int name_cnt);
	void setNodeNames(const char ** names, int name_cnt);
	bool nextMessage(char * msg, int len);

	/// Returns the status identifier for the ongoing computation.
	int getStatus() const ;
	/// Returns the progress of the ongoing computation.
	int getProgress() const ;
	/// Requests the analysis to be aborted.
	void requestCancel();

	/// measure identifier: reliability
	const static int MEASURE_RELIABILITY = 11;
	/// measure identifier: cumulative failures
	const static int MEASURE_CUMULATIVE = 14;
	/// measure identifier: i/1000
	const static int MEASURE_IPER1000 = 12;
	/// use MEASURE_RATE instead.
	const static int MEASURE_INTENSITY = 13;
	/// measure identifier: failure rate
	const static int MEASURE_RATE = 13;

	/// measure identifier: time-to-failure adjustment.
	const static int ADJUSTMENT_TIME = 14;
	/// measure identifier: same as ADJUSTMENT_RATE.
	const static int ADJUSTMENT_INTENSITY = 15;
	/// measure identifier: failure rate adjustment.
	const static int ADJUSTMENT_RATE = 15;
	/// parameter identifier: Weibull scale.
	const static int PARAMETER_SCALE = 16;
	/// parameter identifier: log10(Weibull scale).
	const static int PARAMETER_LOGSCALE = 17;
	/// parameter identifier: Weibull shape.
	const static int PARAMETER_SHAPE = 18;
	
	/// failure mode identifier: no failure (success).
	const static int MODE_SUCCESS = 0;
	/// failure mode identifier: any failure mode. 
	const static int MODE_ANY = -1;

	/// Acceleration mode: no acceleration
	static const int ACCELERATE_NONE = 0;
	/// Acceleration mode: time to failure adjustment
	static const int ACCELERATE_TIME = 1;
	/// Acceleratin mode: failure rate adjustment
	static const int ACCELERATE_RATE = 2;
	/// Acceleration mode: failure mode is not applicable
	static const int ACCELERATE_NULL = 3;

	/// Grouped data: 'no repair' assumption
	static const int REPAIR_NONE = 6;
	/// Grouped data: as-good-as-old assumption
	static const int REPAIR_IMPERFECT = 5;

private:

	void logInfo(string str) const {}
	
	/// Returns the index of the shape parameter in the model
	int getShapeIndex(const int mode) const 
	{return WeibullModelLayout::getShapeIndex(mode,node_cnt);}
	/// Returns the index of the scale parameter in the model
	int getScaleIndex(const int node, const int mode) const 
	{return WeibullModelLayout::getScaleIndex(node,mode,node_cnt);}

	const string getNodeName(int node);
	const string getModeName(int node);

	WeibullStatus status;						///< status object
	ParamArray * model;							///< probabilistic network model
	TestConditions * conditions;				///< test condition definitions
	WeibullSampleSet * samples;					///< sampling results
	TestData * test_data;						///< test data set
	GroupedData * grouped_data;					///< grouped data set
	TimedMeasure * custom_prior;				///< custom prior
	JointDensityEstimate * joint_density;		///< joint density estimate
	int mode_cnt;								///< number of failure modes
	int node_cnt;								///< number of product nodes
	int condition_cnt;							///< number of test condition

	MsgList msg_lst;							///< list of messages
	vector<string> mode_names;					///< list of failure mode names.
	vector<string> node_names;					///< list of product node names.
};