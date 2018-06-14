// $Id: BRASSEngine.cpp 2240 2014-02-23 02:14:31Z Dongfeng Zhu $

#include "BRASSEngine.h"
#include "AdjustAggregateOperator.h"
#include "AdjustWeibullOperator.h"
#include "../core/NormalDensityOperator.h"
#include "WeibullTestDataOperator.h"
#include "WeibullGroupedDataOperator.h"
#include "../core/WeightedLikeOperator.h"
#include "../core/SliceSampler.h"
#include "WeibullMeasureFunction.h"
#include "LinkedAdjustmentModel.h"
#include "WeibullPrior.h"
#include <math.h>
#include <fstream>
#include <cstdlib>
#include "../core/SliceSamplerAids.h"
#include "../core/ConvergenceOperator.h"
#include "../dcdf/DCDFLIBInterface.h"

#include "../core/BRASSError.h"

using namespace std;

BRASSEngine::BRASSEngine(void)
{
	cout << "%%% BRASSEngine constructor: " << this << endl;
	model = 0;
	samples = 0;
	joint_density = 0;
	test_data = 0;
	grouped_data = 0;
	custom_prior = 0;
	conditions = 0;
}

BRASSEngine::~BRASSEngine(void)
{
	cout << "%%% BRASSEngine destructor: " << this << endl; 
	clearAll();
	cout << "%%% BRASSEngine destructor done " << this << endl; 
}

/**
 * Writes the sample set to the output stream.
 */
int BRASSEngine::writeSamples(OutputStream & os)
{
	int retval;

	// check if samples are available.
	if (samples == 0) {
		os.writeInt(0);
		os.writeInt(0);
		return os.writeInt(0);
	}

	os.writeInt(this->node_cnt);
	os.writeInt(this->mode_cnt);
	retval = os.writeInt(samples->getSampleCount());

	if (retval == 0) {
		int len = (1 + node_cnt) * mode_cnt;
		OutputStreamOperator sop(len,os);
		samples->execute(&sop,0);
		retval = sop.getReturnCode();
	}

	return retval;
}

/**
 * Reads the sample set from the input stream.
 * This function is used to restore the results of 
 * the analysis in the engine, by reading the sampleset
 * from the stream. Model information is NOT restored.
 * However, all model information is cleared.
 * This function return 0 if operation was successful.
 */
int BRASSEngine::readSamples(InputStream & is)
{
	int retval,node_cnt,mode_cnt,sample_cnt;

	// clean the engine
	this->clearAll();

	// read number of nodes, modes, samples.
	is.readInt(node_cnt);
	is.readInt(mode_cnt);
	retval = is.readInt(sample_cnt);

	if (retval == -1) {
		node_cnt = mode_cnt = 0;
		return -1;
	}

	if (node_cnt == 0 || mode_cnt == 0 || sample_cnt == 0) {
		return 0;
	}

	if (setup(node_cnt,mode_cnt,1) == 0) {
		int len = (1 + node_cnt) * mode_cnt;
		samples = new WeibullSampleSet(len, min(5000, sample_cnt));
		retval = samples->read(is,sample_cnt);
	} else {
		// failed to perform setup	
		retval = -1;
	}

	if (retval != 0) clearAll();

	return retval;

}

/**
 * Returns the status identifier for the ongoing computation.
 * Identifiers are defined in the Status class. They indicate whether
 * the engine is sampling, doing burn-in, etc.
 *
 * This method can be called by a thread other than the computation
 * thread, in order to support progress/status reports while the
 * computation is going on.
 *
 * @see Status
 * @see #getProgress()
 */
int BRASSEngine::getStatus() const
{
	return status.getStatus();
}


/**
 * Returns the progress of the ongoing computation.
 * The progress is a value between 0 and 100, indicating
 * progress of the current phase of the operation. Use
 * getStatus() to obtain information regarding the current
 * phase
 *
 * This method can be called by a thread other than the computation
 * thread, in order to support progress/status reports while the
 * computation is going on.
 *
 * @see #getStatus()
 */
int BRASSEngine::getProgress() const {
	return status.getProgress();
}

/**
 * Initializes the engine.
 * This method initializes the engine by specifying the number of product
 * nodes and number of failure modes. It sets up the required objects for
 * computation and storage of simulation results.
 *
 * All earlier settings and results are lost when this function is called.
 *
 * Returns 0 when initialization was successful.
 *
 * @param node_cnt the number of product nodes
 * @param mode_cnt the number of failure modes
 * @param condition_cnt number of test conditions
 */
int BRASSEngine::setup(int node_cnt, int mode_cnt, int condition_cnt)
{
	cout << "%%% SETUP: " << node_cnt << " NODES, " << mode_cnt << " MODES, " << condition_cnt << " CONDITIONS" << endl;
	clearAll();

	if (node_cnt <= 0 || mode_cnt <= 0 || condition_cnt <= 0) return BRASSError::ERR_INVALID_SETUP;

	this->node_cnt = node_cnt;
	this->mode_cnt = mode_cnt;
	this->condition_cnt = condition_cnt;

	// Construct the node array based on Weibull layout. The structure of the layout 
	//    mode1_shape_node_1, mode1_scale_node_1, mode1_scale_node_2, ..., mode1_scale_node_nodecnt,
	//     ...
	//    mode(mode_cnt)_shape_node_1, mode(mode_cnt)_scale_node_1, mode(mode_cnt)_scale_node_2, ..., mode(mode_cnt)_scale_node_n
	model = new ParamArray((node_cnt + 1) * mode_cnt);

	// Construct the test condition array; the size of the test condition array 
	// equals condition_cnt * mode_cnt
	conditions = new TestConditions(condition_cnt,mode_cnt);

	// Use default slice sampler aid for all nodes
	for (int i = 0 ; i < (node_cnt + 1) * mode_cnt ; i++) model->setAid(i,new DefaultSliceSamplerAid(0.5,4,0));

	// For shape parameter, use Positive Slice Sampler Aid
	for (int j = 1 ; j <= mode_cnt ; j++) model->setAid(getShapeIndex(j),new PositiveSliceSamplerAid(0.5,4,1));

	for (int j = 1 ; j <= mode_cnt ; j++)
	{
		for (int i = 1 ; i < node_cnt ; i++)
		{
			// Add correlation for all node scale parameters for the same failure mode
			model->addCorrelation( (j - 1) * (node_cnt + 1) + 1, (j - 1) * (node_cnt + 1) + 1 + i);
		}
	}

	return 0;
}

/**
 * Sets the specified acceleration factor.
 *
 * This method sets the acceleration factor for the specified failure mode
 * under the specified testing condition. The acceleration must be greater than
 * 0. Depending on the selected type of acceleration, it is interpreted as a
 * multiplication factor for the failure rate or time.
 *
 * Note that this assumes that the acceleration factor is known. If not, a separate
 * product node must be used, corresponding to the node under a particular test
 * condition. The adjustment models can then be used to model the uncertain accelerations.
 *
 * The default value for all acceleration factors is 1. Failure modes are numbered
 * starting at 1. Conditions are numbered starting at 0. The failure mode can not be
 * MODE_ANY.
 *
 * This function returns 0.
 *
 * @param mode failure mode
 * @param condition testing condition index
 * @param type acceleration type defined in AccelerationFactor.
 *             the possible values are defined inside class AccelerationFactor
 * @param acceleration acceleration factor
 */
int BRASSEngine::setAcceleration(int mode, int condition, int type, double acceleration)
{
	cout << "%%% SET ACCELERATION: MODE " << mode << ", COND " << condition << ": TYPE " << type << "," << acceleration << endl;

	if (conditions == 0 || mode <= 0 || acceleration < 0 || condition < 0 || condition >= conditions->getConditionCount()) {
		cout << "XXXXXX INVALID PARAMETER" << endl;
		return BRASSError::ERR_INVALID_ACCELERATION;
	}

	conditions->setAcceleration(condition,mode,type,acceleration);
	clear();
	return 0;
}

/**
 * Adds a proportional hazard rate model to the problem.
 * This method applies the probabilistic model lamba1(t) = z * lambda0(t) to
 * the specified failure modes of the node0 and node1. z is assumed to be
 * lognormally distributed with given median and errorfactor.
 *
 * Failure modes are numbered starting at 1. Product nodes are numbered starting at 0.
 *
 * The errfac parameter is normally greater than 1. In the special case where errfac
 * is equal to 1, the median value is taken as the fixed and known value of the
 * proportional factor.
 *
 * Note that this method is specific to a given failure mode. Use the overloaded version
 * of this method to add non-specific acceleration models. The mode parameter can not be
 * MODE_ANY.
 * @see addProportionalHazardModel(int,int,double,double)
 *
 * @param node0 product node
 * @param node1 product node
 * @param mode failure mode
 * @param median z-factor median
 * @param errfac z-factor error factor
 */
int BRASSEngine::addProportionalRateModel(int node0, int node1, int mode, double median, double errfac)
{
	if (mode == this->MODE_ANY) return addProportionalRateModel(node0,node1,median,errfac);

	cout << "%%% ADD PROP RATE MODEL" << node0 << " - " << node1 << ", MODE " << mode << ": " << median << "," << errfac << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (median == 0) return 0;
	if (median < 0) return BRASSError::ERR_INVALID_ADJUSTMENT;
	if (errfac < 1) return BRASSError::ERR_INVALID_ADJUSTMENT;

	int beta = this->getShapeIndex(mode);
	int alpha = this->getScaleIndex(node0,mode);
	int gamma = this->getScaleIndex(node1,mode);

	if (errfac == 1) {
		ProportionalRateOperator pro = ProportionalRateOperator(alpha,gamma,beta,median);
		model->addModel(pro);
	} else {
		AdjustWeibullRateOperator awro = AdjustWeibullRateOperator(alpha,gamma,beta,median,errfac);
		model->addOperator(awro);
	}
	clear();
	return 0;
}

/**
 * Adds a proportional time model to the network.
 * The model R1(t)=R0(z*t) is added to the network. z is either known (errfac == 1) or
 * lognormally distributed (errfac > 1). In this method, the reliability functions
 * apply to the reliability function for a single failure mode. The mode parameter can
 * not be MODE_ANY. For non-specific adjustments, see the overloaded version of this method.
 * @param node0 product node
 * @param node1 product node
 * @param mode failure mode
 * @param median z-factor median
 * @param errfac z-factor error factor
 */
int BRASSEngine::addProportionalTimeModel(int node0, int node1, int mode, double median, double errfac)
{
	if (mode == this->MODE_ANY) return addProportionalTimeModel(node0,node1,median,errfac);

	cout << "%%% ADD PROP TIME MODEL" << node0 << " - " << node1 << ", MODE " << mode << ": " << median << "," << errfac << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (median == 0) return 0;
	if (median < 0) return BRASSError::ERR_INVALID_ADJUSTMENT;
	if (errfac < 1) return BRASSError::ERR_INVALID_ADJUSTMENT;

	int beta = this->getShapeIndex(mode);
	int alpha = this->getScaleIndex(node0,mode);
	int gamma = this->getScaleIndex(node1,mode);

	if (errfac == 1) {
		ProportionalTimeOperator pto = ProportionalTimeOperator(alpha,gamma,beta,median);
		model->addModel(pto);
	} else {
		AdjustWeibullTimeOperator awto = AdjustWeibullTimeOperator(alpha,gamma,beta,median,errfac);
		model->addOperator(awto);
	}
	clear();
	return 0;
}

/**
 * Adds a proportional hazard rate model to the problem.
 * This method applies the probabilistic model lamba1(t) = z * lambda0(t) to
 * the specified failure modes of the node0 and node1. z is assumed to be
 * lognormally distributed with given median and errorfactor.
 *
 * Note that this method is specific to the sum of failure modes, i.e. the
 * total failure rate for the system. This is currently implemented by
 * introducing separate models for each failure mode.
 *
 * Later implementations may change this by applying the model to the actual
 * sum of failure rates.
 *
 * The errfac parameter is normally greater than 1. In the special case where errfac
 * is equal to 1, the median value is taken as the fixed and known value of the
 * proportional factor.
 *
 * Failure modes are numbered starting at 1.
 * Product nodes are numbered starting at 0.
 * @see addProportionalHazardModel(int,int,int,double,double)
 *
 * @param node0 product node
 * @param node1 product node
 * @param mode failure mode
 * @param median z-factor median
 * @param errfac z-factor error factor
 */
int BRASSEngine::addProportionalRateModel(int node0, int node1, double median, double errfac)
{
	cout << "%%% ADD PROP RATE MODEL" << node0 << " - " << node1 << ", ALL MODES: " << median << "," << errfac << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (median == 0) return 0;
	if (median < 0) return BRASSError::ERR_INVALID_ADJUSTMENT;
	if (errfac < 1) return BRASSError::ERR_INVALID_ADJUSTMENT;

	if (errfac == 1) {
		for (int i = 1 ; i <= mode_cnt ; i++)
			addProportionalRateModel(node0,node1,i,median,errfac);
	} else {
		//model->addOperator(AggregateRateOperator(node0,node1,median,errfac,mode_cnt,node_cnt));
		addProportionalRateModel(node0,node1,1,median,errfac);
		for (int i = 2 ; i <= mode_cnt ; i++) {
			LinkedRateAdjustmentModel lram = LinkedRateAdjustmentModel(node0,node1,node_cnt,i,1);
			model->addModel(lram);
		}
	}
	clear();
	return 0;
}

/**
 * Adds a proportional time-to-failure model to the problem.
 * This function works similar to addProportionalHazardModel(), with the exception of
 * the type of model that is introduced. The proportional time to failure mode is applied to
 * all failure modes.
 * @see #addProportionalHazardModel(int,int,double,double)
 * @see #addProportionalHazardModel(int node0, int node1, int mode, double median, double errfac)
 * @param node0 product node
 * @param node1 product node
 * @param median z-factor median
 * @param errfac z-factor error factor
 */
int BRASSEngine::addProportionalTimeModel(int node0, int node1, double median, double errfac)
{
	cout << "%%% ADD PROP TIME MODEL" << node0 << " - " << node1 << ", ALL MODES: " << median << "," << errfac << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (median == 0) return 0;
	if (median < 0) return BRASSError::ERR_INVALID_ADJUSTMENT;
	if (errfac < 1) return BRASSError::ERR_INVALID_ADJUSTMENT;

	if (errfac == 1) {
		for (int i = 1 ; i <= mode_cnt ; i++)
			addProportionalTimeModel(node0,node1,i,median,errfac);
	} else {
		// model->addOperator(AggregateTimeOperator(node0,node1,median,errfac,mode_cnt,node_cnt));
		addProportionalTimeModel(node0,node1,1,median,errfac);
		for (int i = 2 ; i <= mode_cnt ; i++) {
			LinkedTimeAdjustmentModel ltam = LinkedTimeAdjustmentModel(node0,node1,node_cnt,i,1);
			model->addModel(ltam);
		}
	}
	clear();
	return 0;
}

/**
 * Specifies a lognormal prior for the scale parameter in the model.
 *
 * The scale parameter is identified by the product node and the failure mode.
 * By default, the scale parameter prior is log-uniform, i.e., uniform over the
 * logarithm of the scale parameter.
 *
 * Note that internally, the engine operates on the natural log of the scale
 * parameters, such that the lognormal translates into a normal prior density.
 *
 * If the median is 0 and errfac is 1, the prior information is ignored, but
 * the code returns 0. For any other invalid parameter combination, this
 * method returns an error code (< 0)..
 *
 * Failure modes are numbered starting at 1.
 * Product nodes are numbered starting at 0.
 * @param node product node
 * @param mode failure mode
 * @param median prior median for scale parameter
 * @param errfac prior errfac for scale parameter
 */
int BRASSEngine::addScalePrior(int node, int mode, double median, double errfac)
{
	cout << "%%% ADD SCALE PRIOR: NODE " << node << ", MODE " << mode << ": " << median << "," << errfac << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (median == 0 && errfac == 1) return 0;

	if (median <= 0) return BRASSError::ERR_INVALID_ADJUSTMENT;
	if (errfac <= 1) return BRASSError::ERR_INVALID_ADJUSTMENT;

	int alpha = getScaleIndex(node,mode);
	model->addOperator(NormalDensityOperator(alpha,log(median),log(errfac)/1.645));
	clear();
	return 0;
}

/**
 * Specifies a lognormal prior distribution for the shape parameter.
 *
 * The shape parameter is identified by the failure mode. Note that the
 * shape parameter is shared by all product nodes. By default, the prior
 * is lognormal with median 1 and error factor 10.
 *
 * In the special case where the error factor is 1, the value of the
 * shape parameter is fixed.
 *
 * If the median is 0 and errfac is 1, the prior information is ignored, but
 * the code returns 0. For any other invalid parameter combination, this
 * method returns an error code (< 0)..
 *
 * @param mode failure mode
 * @param median prior median
 * @param errfac prior error factor
 */
int BRASSEngine::addShapePrior(int mode, double median, double errfac)
{
	cout << "%%% ADD SHAPE PRIOR " << mode << ": " << median << "," << errfac << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (median == 0 && errfac == 1) return 0;

	if (median <= 0) return BRASSError::ERR_INVALID_ADJUSTMENT;
	if (errfac < 1) return BRASSError::ERR_INVALID_ADJUSTMENT;

	int beta = getShapeIndex(mode);
	if (errfac == 1) {
		model->setRange(beta,model->RANGE_FIXED);
		model->setParam(beta,(float)median);
		clear();
		return 0;
	} else {
		model->addOperator(LogNormalDensityOperator(beta,median,errfac));
		clear();
		return 0;
	}
}

/**
 * Fixes the value of the scale parameter to the specified value.
 *
 * This causes the parameter to be excluded from the sampling procedure,
 * and its value to be held constant.
 * The scale parameter is identified by product node and failure mode.
 *
 * This method is primarily used for debugging purposes. Note that the
 * constraint could be violated when adjustment models with fixed
 * factors involving the parameter are used.
 *
 * @param node product node index
 * @param mode failure mode index
 * @param value value to fix parameter at
 */
int BRASSEngine::fixScaleParameter(int node, int mode, double value)
{
	if (model == 0) return BRASSError::ERR_INVALID_SETUP;

	cout << "### FIX SCALED PARAMETER " << node << " " << mode << ": USE FOR DEBUG ONLY!" << endl;
	int alpha = getScaleIndex(node,mode);
	model->setRange(alpha,model->RANGE_FIXED);
	model->setParam(alpha, value);
	clear();
	return 0;
}

/**
 * Fixes the value of the shape parameter to the specified value.
 *
 * The shape parameter is identified by failure mode to which it
 * applies.
 *
 * This causes the parameter to be excluded from the sampling procedure,
 * and its value to be held constant.
 *
 * This method is primarily used for debugging purposes. Note that the
 * constraint could be violated when adjustment models with fixed
 * factors involving the parameter are used.
 *
 * @param node product node index
 * @param mode failure mode index
 * @param value value to fix parameter at
 */
int BRASSEngine::fixShapeParameter(int mode, double value)
{
	if (model == 0) return BRASSError::ERR_INVALID_SETUP;

	cout << "### FIX SHAPE PARAMETER " << mode << ": USE FOR DEBUG ONLY!" << endl;
	int beta = getShapeIndex(mode);
	model->setRange(beta,model->RANGE_FIXED);
	clear();
	return 0;
}

/**
 * Creates a new data set.
 * Call this function to create a new data set to be attached to a node in the model.
 * Any test data set previously created but not attached to the model using
 * attachTestDataSet() is deleted.
 * @see #attachTestDataSet(int,double)
 * @see #setTestDataRecord(int,double,double,int,int,double,double)
 * @param record_cnt number of records in the data set
 */
int BRASSEngine::createTestDataSet(int record_cnt)
{
	cout << "%%% CREATE TEST DATA, " << record_cnt << " RECORDS" << endl;
	if (test_data != 0) {
		if (test_data->release()) delete test_data;
		test_data = 0;
	}
	test_data = new TestData(record_cnt);
	test_data->obtain();
	return 0;
}

/**
 * Creates a new grouped data set.
 * @param record_cnt number of records
 * @param mode failure mode (-1 for 'any FM').
 * @param repair repair assumption: REPAIR_NONE or REPAIR_IMPERFECT.
 */
int BRASSEngine::createGroupedDataSet(const int record_cnt, const int mode, const int repair)
{
	cout << "%%% CREATE GROUPED DATA, " << record_cnt << " RECORDS. MODE " << mode << " REPAIR " << repair << endl;
	if (grouped_data != 0) {
		if (grouped_data->release()) delete grouped_data;
		grouped_data = 0;
	}
	grouped_data = new GroupedData(record_cnt,mode,repair);
	grouped_data->obtain();
	return 0;
}

/**
 * Loads the data set from the specified file, and attaches to the node.
 * This method allows test data sets to be loaded from test data sets.
 */
int BRASSEngine::loadTestConditions(const char * file_name)
{
	if (model == 0) return BRASSError::ERR_INVALID_SETUP;

	cout << "%%% LOADING TEST CONDITIONS FROM " << file_name << endl;
	int condition,mode,type;
	double factor;
	ifstream ifs;
	ifs.open(file_name);
	if (ifs) {
		ifs >> mode;
		while (ifs) {
			ifs >> condition >> type >> factor;
			this->setAcceleration(mode,condition,type,factor);
			ifs >> mode;
		}
		ifs.close();
	}
	return 0;
}

/**
 * Loads the data set from the specified file, and attaches to the node.
 * This method allows test data sets to be loaded from test data sets.
 * Set mode to MODE_ANY to apply the test data to all failure modes.
 */
int BRASSEngine::loadTestDataSet(const char * file_name, int node, int mode, double weight)
{
	cout << "### LOADING TEST DATA SET FROM " << file_name << endl;

	int rec_cnt,fm,condition;
	double start,end,strength,credit;
	ifstream ifs;
	ifs.open(file_name);
	if (ifs) {
		ifs >> rec_cnt;
		createTestDataSet(rec_cnt);
		for (int i = 0 ; i < rec_cnt ; i++) {
			ifs >> start >> end >> fm >> condition >> strength >> credit;
			setTestDataRecord(i,start,end,fm,condition,strength,credit);
		}
	}
	ifs.close();
	attachTestDataSet(node,mode,weight);
	return 0;
}

/**
 * Sets the content of a record in the test data set.
 * This method is used to specify the content of data set records.
 * It must be called for all records in the data set. It must be called
 * before calling attachTestDataSet().
 * @see #createTestDataSet(int)
 * @see #attachTestDataSet(int,double)
 * @param index index of the test data record, starts counting at 0
 * @param start test start time
 * @param end test end time
 * @param mode failure mode, starts counting at 1.
 * @param condition test condition, starts counting at 0
 * @param strength weighted likelihood strength for record (same as 'weight')
 * @param credit design credit (same as 'confidence in fix')
 */
int BRASSEngine::setTestDataRecord(const int index, const double start,
		const double end, const int mode, const int condition,
		double strength, double credit)
{
	cout << "%%% SET TD RECORD " << index << " , " << start << " , " << end << " , " << mode << " , " << condition << " , " << strength << " , " << credit << endl;
	if (test_data == 0) return BRASSError::ERR_INVALID_TESTDATA;

	test_data->setRecord(index,start,end,mode,condition,strength,credit);
	return 0;
}

/**
 * Sets the information for the specified grouped data record.
 */
int BRASSEngine::setGroupedDataRecord(const int index, const double start,
		const double end, const int failures, const int population)
{
	const double strength = 1;
	cout << "%%% SET GD RECORD " << index << " , " << start << " , " << end << " , " << failures << " , " << population << endl;
	if (grouped_data == 0) return BRASSError::ERR_INVALID_TESTDATA;
	grouped_data->setRecord(index,start,end,population,failures,strength);
	return 0;
}

/**
 * Attaches the current data set to the specified node and failure mode.
 *
 * This version associates the data with a specific failure mode.
 * For a general description, see attachTestDataSet(int,double).
 *
 * @see #attachTestDataSet(int,double)
 * @param node product node
 * @param mode failure mode
 * @param weight weight/strength of the data set.
 */
int BRASSEngine::attachTestDataSet(int node, int mode, double weight)
{
	cout << "### ATTACHING DATA SET TO NODE " << node ;
	cout << ", MODE " << mode << ", WEIGHT " << weight << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (mode == MODE_ANY) {
		return attachTestDataSet(node,weight);
	}

	if (test_data != 0)
	{
		int alpha = getScaleIndex(node,mode);
		int beta = getShapeIndex(mode);
		WeibullTestDataOperator wtd(test_data,conditions,node_cnt,mode_cnt,node,mode);

		if (weight < 1) {
			model->addOperator(WeightedLikeOperator(&wtd,weight));
		} else {
			model->addOperator(wtd);
		}

		if (test_data->release()) delete test_data;
		test_data = 0;
	}
	clear();
	return 0;
}

/**
 * Associates the grouped data set with all failure modes.
 * @param node node to attach the data set to.
 * @param weight statistical strength of the data set.
 */
int BRASSEngine::attachGroupedDataSet(int node, double weight)
{
	if (grouped_data == 0) return 0;
	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	int mode = grouped_data->getMode();

	cout << "### ATTACHING GROUPED DATA SET TO NODE " << node ;
	cout << ", MODE " << mode << ", WEIGHT " << weight << endl;

	if (mode != MODE_ANY) {
		return attachGroupedDataSet(node,mode,weight);
	} else {
		for (mode = 1 ; mode <= this->mode_cnt ; mode++) {
			WeibullGroupedDataOperator wgd(grouped_data,node_cnt,mode_cnt,node,mode);
			if (weight < 1) {
				model->addOperator(WeightedLikeOperator(&wgd,weight));
			} else {
				model->addOperator(wgd);
			}
		}
		if (grouped_data->release()) delete grouped_data;
		grouped_data = 0;
	}
	clear();
	return 0;
}

/**
 * Associates the grouped data set with the specified node.
 */
int BRASSEngine::attachGroupedDataSet(int node, int mode, double weight)
{
	cout << "### ATTACHING GROUPED DATA SET TO NODE " << node ;
	cout << ", MODE " << mode << ", WEIGHT " << weight << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (mode == MODE_ANY) {
		return attachGroupedDataSet(node,weight);
	}

	if (mode != grouped_data->getMode()) return 0;

	if (grouped_data != 0)
	{
		WeibullGroupedDataOperator wgd(grouped_data,node_cnt,mode_cnt,node,mode);
		if (weight < 1) {
			model->addOperator(WeightedLikeOperator(&wgd,weight));
		} else {
			model->addOperator(wgd);
		}
		if (grouped_data->release()) delete grouped_data;
		grouped_data = 0;
	}
	clear();
	return 0;
}

/**
 * Attaches a data set to a product revision node in the model.
 *
 * The data set is attached to the specified node, and assigned a weight/strength
 * value, which is interpreted as a statistical strength factor (1 = full strength,
 * 0 = no strength).
 *
 * Before calling this method, a new test data set must first be created, and
 * all its records set: attaching the data set causes some processing of the
 * content of the data set. This method creates WeibullTestDataOperator objects
 * for each of the failure modes.
 *
 * An overloaded version of this method allows test data to be associated with a
 * specific failure mode.
 *
 * This function can be called once for each test data set.
 *
 * @see #createTestDataSet(int)
 * @see #attachDataSet(int,int,double)
 * @see #setTestDataRecord(int,double,double,int,int,double,double)
 * @param node node number to attach data set to
 * @param weight weighted likelihood weight for data set
 */
int BRASSEngine::attachTestDataSet(int node, double weight)
{
	cout << "%%% ATTACH DATA SET " << test_data << " TO NODE " << node ;
	cout << ", WEIGHT " << weight << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (test_data != 0)
	{
		// add the previously created data set to the model
		for (int i = 1 ; i <= mode_cnt ; i++) {
			int alpha = getScaleIndex(node,i);
			int beta = getShapeIndex(i);
			WeibullTestDataOperator wtd(test_data,conditions,node_cnt,mode_cnt,node,i);

			if (weight < 1) {
				model->addOperator(WeightedLikeOperator(&wtd,weight));
			} else {
				model->addOperator(wtd);
			}
		}
		if (test_data->release()) delete test_data;
		test_data = 0;
	}
	clear();
	return 0;
}

/**
 * Creates a new custom prior.
 * @param record_cnt number of records
 * @param measure measure type identifier
 */
int BRASSEngine::createCustomPrior(const int record_cnt, const int measure)
{
	cout << "%%% CREATE CUSTOM PRIOR: " << record_cnt << " RECORDS" << endl;

	if (custom_prior != 0) {
		if (custom_prior->release()) delete custom_prior;
		custom_prior = 0;
	}

	custom_prior = new TimedMeasure(record_cnt,measure);
	custom_prior->obtain();
	return 0;
}

/**
 * Sets a record in the custom prior.
 * @param index record index
 * @param time measure time
 * @param lo  lower bound
 * @param hi  upper bound
 * @param confidence degree of confidence
 */
int BRASSEngine::setCustomPriorRecord(const int index, const double time, const double lo,
									  const double hi, const double confidence)
{
	cout << "%%%%% SET PRIOR RECORD " << index << " AT " << time << ": LO " << lo;
	cout << ", HI " << hi << " CONF " << confidence << endl;

	if (custom_prior == 0) return BRASSError::ERR_INVALID_PRIOR;
	custom_prior->setRecord(index,time,lo,hi,confidence);
	return 0;
}

/**
 * Attaches the custom prior to the model.
 * Failure mode may be any specific failure mode or MODE_ANY.
 * @param node to which prior applies
 * @param failure mode to which prior applies
 */
int BRASSEngine::attachCustomPrior(const int node, const int mode)
{
	cout << "%%% ATTACH PRIOR TO NODE " << node << " MODE " << mode << endl;

	if (this->model == 0) return BRASSError::ERR_INVALID_SETUP;

	if (custom_prior != 0)
	{
		const MeasureRecord * rec = custom_prior->getRecord(0);
		if (!rec || rec->lower == 0 || rec->upper == 0) {
			cout << "%%% IGNORING PRIOR RECORD" << endl;
			if (custom_prior->release()) delete custom_prior;
			custom_prior = 0;
		}
	}

	if (custom_prior != 0)
	{
		WeibullPrior prior(custom_prior,node,mode,node_cnt,mode_cnt);
		model->addOperator(prior);
		if (custom_prior->release()) delete custom_prior;
		custom_prior = 0;
	}
	clear();
	return 0;
}

/**
 * Clears previously generated samples.
 * This method removes previously generated results from the engine.
 */
void BRASSEngine::clear()
{
	delete samples;
	samples = 0;

	delete joint_density;
	joint_density = 0;
}

/**
 * Clears all model and sample information from memory.
 * Dynamically allocated objects are deallocated.
 */
void BRASSEngine::clearAll() 
{
	node_cnt = mode_cnt = 0;

	if (conditions != 0 && conditions->release()) delete this->conditions;
	this->conditions = 0;

	if (custom_prior != 0 && custom_prior->release()) delete this->custom_prior;
	this->custom_prior = 0;

	if (grouped_data != 0 && grouped_data->release()) delete this->grouped_data;
	this->grouped_data = 0;

	if (test_data != 0 && test_data->release()) delete this->test_data;
	this->test_data = 0;

	delete this->joint_density;
	this->joint_density = 0;

	delete this->model;
	this->model = 0;

	delete this->samples;
	this->samples = 0;
}

/**
 * Generates samples using the slice sampler.
 * This method applies the slice sampler to generate samples from the model.
 * The samples and chain parameters jointly determine how many samples are generated.
 * Samples for each chain are stored separately. The interleave parameter can be used
 * to make the sampler skip iterations between storing samples. The burnin parameter can
 * be used to specify how many iterations should be performed before samples are stored.
 * If burnin is set to 0, the sampler will continue from where it last stopped. Otherwise,
 * it will start from a generic starting point. The clear parameter can be used to specify
 * whether existing samples should be removed from memory before starting the sampling
 * procedure.
 *
 * Note that the number of chains parameter is for now used to increase the size of
 * a single chain.
 *
 * @param samples total number of samples
 * @param chains number of independent sample chains.
 * @param interleave number of iterations skipped between samples for thinning.
 * @param burnin number of burnin iterations
 * @param clear clear sample set before sampling.
 * @param seed random number generator seed
 */
int BRASSEngine::generateSamples(int samples, int chains, int interleave, int burnin, long seed, bool clear)
{
	this->msg_lst.clear();
	int result = 0;

	// msg_lst.add("MODIFIED FOR DEBUG!");

	// max potential. See Gilks, MCMC in Practice (1996), Sec. 8.4.
	const float max_allowed_potential = 1.2f;

	cout << "%%% GENERATE SAMPLES" << endl;
	cout << "%%%% SAMPLES   : " << samples << endl;
	cout << "%%%% CHAINS    : " << chains << endl;
	cout << "%%%% INTERLEAVE: " << interleave << endl;
	cout << "%%%% BURNIN    : " << burnin << endl;
	cout << "%%%% SEED      : " << seed << endl;
	cout << "%%%% CLEAR     : " << clear << endl;

	if (model == 0) 
	{
		msg_lst.add("ERROR: no model defined.");
		return BRASSError::ERR_INVALID_SETUP;
	}

	status.setProgress(0);
	status.setStatus(WeibullStatus::STATUS_BURNIN);

	if (clear) this->clear();

	// clear joint density regardless of whether samples are cleared
	delete joint_density;
	joint_density = 0;

	// compute the required size of the parameter array
	int len = (1 + node_cnt) * mode_cnt;

	SliceSampler sampler;
	// sets the sampler seed value
	sampler.setSeed(seed);

	if (burnin > 0) {

		// set up variable initial values
		for (int v = 0 ; v < len ; v++) {
			if (model->getRange(v) == model->RANGE_VARIABLE) {
				model->setParam(v,model->getAid(v)->getStartValue());
			}
		}

		cout << "%%%%% INITIALIZING" << endl;
		int retval = model->initialize();
		if (retval != 0) {

			for (int mode = 1 ; mode <= mode_cnt ; mode++) 
			{
				for (int node = 0 ; node < node_cnt ; node++) 
				{
					int ai = (1 + node) + (mode - 1) * (1 + node_cnt);
					if (!model->hasModel(ai))
					{
						string msg = "ERROR: No data or prior specified for '";
						msg += getNodeName(node)+ "." + getModeName(mode) + "'. ";
						msg_lst.add(msg);
						result = BRASSError::ERR_IMPROPER_POSTERIOR;
					}
				}
			}

			if (retval == BRASSError::ERR_LOOP_IN_MODEL)
			{
				msg_lst.add("ERROR: the adjustment models form a loop, which is not allowed.");
			}
			else 
			{
				msg_lst.add("ERROR: the model failed to initialize.");
			}

			cout << "%%%% INITIALIZATION ERROR" << endl;
			return retval;
		}

		cout << "%%%%% BURNIN" << endl;
		// perform the burnin sampling
		for (int b = 0 ; b < burnin ; b++) {

			sampler.advance(*model);

			status.setProgress((int)(100. * b / burnin));
			if (b % (burnin / 10) == 0) {
				cerr << "." ;
				cerr.flush();
			}
		}
	}

	// set up the sampler set
	if (this->samples == 0) 
		this->samples = new WeibullSampleSet(len, 2000);

	// determine required number of samples
	int iterations = samples * chains * interleave;

	status.setStatus(WeibullStatus::STATUS_SAMPLE);
	status.setProgress(0);

	cout << endl << "%%%%% SAMPLING" << endl;

	// perform the actual sampling
	for (int i = 0 ; i < iterations && result == 0 && !status.isCancelRequested() ; i++) {

		sampler.advance(*model);

		if ((i + 1) % interleave == 0)
		{	
			// check for sampler runaway conditions by imposing
			// 'reasonable' limits on the cumulative failure rate
			for (int mode = 1 ; mode <= mode_cnt ; mode++) 
			{
				float b = model->getParam((mode - 1) * (1 + node_cnt));
				for (int node = 0 ; node < node_cnt ; node++) 
				{
					float lna = model->getParam((1 + node) + (mode - 1) * (1 + node_cnt));
					float lncum = b * lna;					
					if (lncum < -200 || lncum > 200)
					{
						string msg = "ERROR: Insufficient evidence or conflicting evidence detected for '";
						msg += getNodeName(node)+ "." + getModeName(mode) + "'. ";
						msg += "Please see help on 'insufficient evidence' for more information.";
						msg_lst.add(msg);
						result = BRASSError::ERR_IMPROPER_POSTERIOR;
					}
				}
			}
		}
		
		if (result == 0) {

			if ((i + 1) % interleave == 0) {
				this->samples->fetch(model);
			}

			status.setProgress((int)(100. * i / iterations));

			if (i % (iterations / 100) == 0) {
				// cerr << "." ;
				cerr.flush();
			}
		}

	}

	if (status.isCancelRequested()) 
	{
		msg_lst.add("Analysis canceled...");
		status.setStatus(status.STATUS_ABORTED);
		result = BRASSError::CANCELED;
	}

	if (result == 0)
	{
		// check convergence
		status.setStatus(WeibullStatus::STATUS_CONVERGENCE);
		status.setProgress(0);

		ConvergenceOperator convergence(len, samples);
		this->samples->execute(convergence);

		float max_potential = 0;

		cout << endl;

		for (int mode = 1 ; mode <= mode_cnt ; mode++) 
		{
			bool conv_ok = true;
			int bi = (mode - 1) * (1 + node_cnt);
			float potential = convergence.computePotential(bi);
			cout << bi << " : POTENTIAL = " << potential << endl;

			if (potential > max_allowed_potential) {
				max_potential = potential;
				conv_ok = false;
				cout << "!!!";
			}
			cout << endl;

			for (int node = 0 ; node < node_cnt ; node++) 
			{
				int ai = (1 + node) + (mode - 1) * (1 + node_cnt);
				potential = convergence.computePotential(ai);
				cout << ai << " : POTENTIAL = " << potential;
				if (potential > max_allowed_potential) {
					max_potential = potential;
					conv_ok = false;
					cout << "!!!";
				}
				cout << endl;
			}
			if (!conv_ok) msg_lst.add("Poor convergence for '" + getModeName(mode) + "'.");
		}

		if (max_potential > max_allowed_potential) 
		{
			char buffer[256];
			sprintf(buffer,
				"Maximum correlation improvement potential = %f (%f max).",
				max_potential,max_allowed_potential);
			string msg = "WARNING: Poor sampler convergence was detected for one or more parameters.";
			msg += buffer;
			msg += "Please check data or retry with a higher number of samples. See help on 'convergence' for more information";
			msg_lst.add(msg);
			cout << "POOR CONVERGENCE WARNING!";
		}
	}

	if (result == 0) 
	{
		for (int mode = 1 ; mode <= mode_cnt ; mode++) 
		{
			int bi = (mode - 1) * (1 + node_cnt);
			for (int node = 0 ; node < node_cnt ; node++) 
			{
				int ai = (1 + node) + (mode - 1) * (1 + node_cnt);
				SortingQuantileOperator quant_op(new WeibullLnCumulativeFunction(ai,bi,1));
				this->samples->execute(quant_op);
				if (quant_op.getQuantile(quant_op.Q01) < -50 || 
					quant_op.getQuantile(quant_op.Q99) > 50) 
				{
					string msg = "ERROR: Insufficient evidence or conflicting evidence detected for '";
					msg += getNodeName(node)+ "." + getModeName(mode) + "'. ";
					msg += "Please see help on 'insufficient evidence' for more information.";
					msg_lst.add(msg);
					result = BRASSError::ERR_IMPROPER_POSTERIOR;
					cout << "Improper posterior: node " << node << " mode " << mode << " : " << quant_op.getQuantile(quant_op.Q01) << " " << quant_op.getQuantile(quant_op.Q99) << endl;
				}
			}
		}
	}

	if (result != 0) 
	{
		delete this->samples;
		this->samples = 0;
		msg_lst.add("No results generated.");
	}
	else
	{
		msg_lst.add("Analysis completed.");
	}

	status.setProgress(100);
	status.setStatus(WeibullStatus::STATUS_READY);
	cout << endl << "%%%%% DONE" << endl;

	return result;
}

/**
 * Returns characteristics of the measure distribution.
 *
 * This method estimates the characteristics of the distribution over
 * the specified measure. The measure is MEASURE_RELIABILITY
 * MEASURE_CUMULATIVE, or MEASURE_INTENSITY. Note that MEASURE_CUMULATIVE
 * causes the cumulative number of failures to be computed. The measure
 * can be computed for a single failure mode or the aggregated failure modes
 * (mode = MODE_ANY).
 *
 * Other measures are not supported by this method.
 *
 * @param node product node
 * @param mode failure mode or MODE_ANY
 * @param time time for which to compute the measure
 * @param mean estimated mean
 * @param median estimated median
 * @param p01 estimated 1st percentile
 * @param p05 estimated 5th percentile
 * @param p10 estimated 10th percentile.
 */
int BRASSEngine::computeMeasure(int measure, int node, int mode, double time,
								double &mean, double &median,
								double &p01, double &p05, double &p10,
								double &p90, double &p95, double &p99)
{
	OperatorFunction * func;

	if (samples == 0 || samples->isEmpty()) return BRASSError::ERR_NO_SAMPLES;

	func = createMeasureFunction(measure,node,mode,time);

	return computeUncertainty(func,mean,median,p01,p05,p10,p90,p95,p99);
}

/**
 * Computes uncertainty characteristics of the specified adjustment.
 * Adjustment is either ADJUSTMENT_INTENSITY or ADJUSTMENT_TIME.
 * Computations must be specific to a failure mode, i.e., MODE_ANY is not
 * allowed as a value for mode. Returns 0 on success.
 * @param adjustment adjustment type identifier
 * @param node0 product node index
 * @param node1 product node index
 * @param mode failure mode
 */
int BRASSEngine::computeAdjustment(int adjustment, int node0, int node1, int mode,
								double &mean, double &median,
								double &p01, double &p05, double &p10,
								double &p90, double &p95, double &p99)
{
//	cout << "%%% COMPUTE ADJUSTMENT" << endl;

	OperatorFunction * func;
	if (samples == 0 || samples->isEmpty()) return BRASSError::ERR_NO_SAMPLES;
	if (mode == MODE_ANY) return BRASSError::ERR_INVALID_MEASURE;
	func = createAdjustmentFunction(adjustment,node0,node1,mode);
	return computeUncertainty(func,mean,median,p01,p05,p10,p90,p95,p99);
}

/**
 * Returns uncertainty properties for the specified parameter.
 * @param param parameter type identifier
 * @param node product node
 * @param mode failure mode
 */
int BRASSEngine::computeParameter(int param, int node, int mode,
								double &mean, double &median,
								double &p01, double &p05, double &p10,
								double &p90, double &p95, double &p99)
{
//	cout << "%%% COMPUTE PARAMETER" << endl;
	if (samples == 0 || samples->isEmpty()) return BRASSError::ERR_NO_SAMPLES;
	if (mode == MODE_ANY) return BRASSError::ERR_INVALID_MEASURE;
	OperatorFunction * func = createParameterFunction(param,node,mode);
	if (func == 0) return BRASSError::ERR_INVALID_MEASURE;
	return computeUncertainty(func,mean,median,p01,p05,p10,p90,p95,p99);
}

/**
 * Computes the confidence that a particular value is exceeded by the function.
 * The confidence is estimated by counting the fraction of samples for which
 * the value is exceeded. The method is therefore unreliable for high and low
 * confidence levels.
 */
int BRASSEngine::computeConfidence(OperatorFunction * func, double val, double & confidence)
{
	if (samples == 0 || samples->isEmpty()) return BRASSError::ERR_NO_SAMPLES;

	if (func != 0) {
		ConfidenceOperator co(func,val);
		samples->execute(&co);
		confidence = co.getConfidence();
		return 0;
	} else return BRASSError::ERR_INVALID_MEASURE;
}

/**
 * Computes uncertainty characteristics for the specified function.
 * @see #computeMeasure()
 * @see #computeAdjustment()
 */
int BRASSEngine::computeUncertainty(OperatorFunction * func,
								double &mean, double &median,
								double &p01, double &p05, double &p10,
								double &p90, double &p95, double &p99)
{
	MeanSDevOperator * mnsdop;
	QuantileOperator * quantop;
	DeviateGenerator generator;

	if (samples == 0 || samples->isEmpty()) return BRASSError::ERR_NO_SAMPLES;

	if (func != 0) {

		mnsdop = new MeanSDevOperator(func->clone());
		samples->execute(mnsdop,&generator);
		mean = mnsdop->getMean();
		delete mnsdop;

		quantop = new SortingQuantileOperator(func->clone());
		samples->execute(quantop,&generator);
		median = quantop->getQuantile(quantop->Q50);
		p01 = quantop->getQuantile(quantop->Q01);
		p05 = quantop->getQuantile(quantop->Q05);
		p10 = quantop->getQuantile(quantop->Q10);
		p90 = quantop->getQuantile(quantop->Q90);
		p95 = quantop->getQuantile(quantop->Q95);
		p99 = quantop->getQuantile(quantop->Q99);
		delete quantop;
		delete func;

		return 0;
	} else {
		return BRASSError::ERR_INVALID_MEASURE;
	}
}


/**
 * Prepares an estimate of the joint parameter density.
 *
 * The function estimates a joint density over the shape/scale of the specified
 * product revision node and failure mode. The draw_points parameter specifies
 * the size of the grid (on each axis), and must be at least 2. In this implementation
 * the bounds are automatically selected.
 *
 * Coordinates and density values are retrieved using the getJointDensity() function.
 *
 * Note that more flexibility is provided by the
 * The user will be allowed to specify exactly which parameters will be used
 * to construct the joint density, including (a) scale parameters (b) natural log
 * of the scale (c) shape parameters, (d) time/rate adjustment parameters.
 *
 * The current density estimate is generated using a simple binning/histogram procedure.
 *
 * @param node	product node
 * @param mode  failure mode, or MODE_ANY
 * @param log_scale flag: use log10 scaling for scale parameter
 * @param draw_points number of points on each axis
 * @see #estimateJointDensity()
 * @see #getJointDensity()
 */
int BRASSEngine::estimateJointParameterDensity(int node, int mode, bool log_scale, int draw_points)
{
	cout << "%%% ESTIMATE JOINT PARAMETER DENSITY: NODE " << node << ", MODE " << mode << ", LOGSCALE " << log_scale << ", POINTS " << draw_points << endl;
	int scale = log_scale ? PARAMETER_LOGSCALE : PARAMETER_SCALE;
	OperatorFunction * f1 = this->createParameterFunction(scale,node,mode);
	OperatorFunction * f2 = createParameterFunction(PARAMETER_SHAPE,node,mode);
	return estimateJointDensity(f1,f2,draw_points);
}

/**
 * Prepares the joint density for the specified functions.
 * This method allows to compute the joint density for two
 * variables in the model, specified by the operator function
 * objects.
 *
 * The density is estimated through a binning procedure, a
 * mesh is created based on the number of draw points. WHen a
 * sample falls into a cell in the mesh, the points making up
 * the corners of the cell are credited based on their distance
 * to the sample.
 *
 * Note that the operator functions will be deleted after
 * the operation is complete. No references should be maintained
 * after calls to this function.
 *
 * After the density is estimated, values can be obtained using the
 * getJointDensity() function.
 * @see #getJointDensity()
 * @see #createJointParameterDensity()
 */
int BRASSEngine::estimateJointDensity(OperatorFunction * f1, OperatorFunction * f2, int draw_points)
{
	if (samples == 0 || samples->isEmpty()) return BRASSError::ERR_NO_SAMPLES;

	if (joint_density != 0) {
		delete joint_density;
		joint_density = 0;
	}
	joint_density = new JointDensityEstimate(f1,f2,samples,draw_points);
	return 0;
}

/**
 * Retrieves the information for the specified point in the joint density grid.
 *
 * This returns the results of the last call to estimateJoint[...]Density().
 * @param x_index x coordinate
 * @param y_index y coordinate
 * @param x x value
 * @param y y value
 * @param density density value
 * @see #estimateJointDensity()
 * @see #estimateJointParameterDensity()
 */
int BRASSEngine::getJointDensity(int x_index, int y_index, double & x, double & y, double & density)
{
	if (joint_density != 0) {
		return joint_density->getEstimate(x_index,y_index,x,y,density);
	} else {
		return BRASSError::ERR_NO_JOINTDENSITY;
	}
}

/**
 * Creates an OperatorFunction that can be used to compute the specified measure.
 * This can be used in conjuction with such functions as computeJointDensity().
 * @param measure measure type identifier
 * @param node node index
 * @param mode failure mode index
 * @param time time at which to compute measure
 */
OperatorFunction * BRASSEngine::createMeasureFunction(int measure, int node, int mode, double time) const
{
	if (model == 0) return 0;

	OperatorFunction * func = 0;

	if (mode == MODE_ANY) {
		switch (measure) {
			case MEASURE_RELIABILITY:
				func = new TotalWeibullReliabilityFunction(node,node_cnt,mode_cnt,time);
				break;
			case MEASURE_CUMULATIVE:
				func = new TotalWeibullCumulativeFunction(node,node_cnt,mode_cnt,time);
				break;
			case MEASURE_IPER1000:
				func = new TotalWeibullI1000Function(node,node_cnt,mode_cnt,time);
				break;
			case MEASURE_INTENSITY:
				func = new TotalWeibullIntensityFunction(node,node_cnt,mode_cnt,time);
				break;
		}
	} else {
		int alpha = getScaleIndex(node,mode);
		int beta = getShapeIndex(mode);
		switch (measure) {
			case MEASURE_RELIABILITY:
				func = new WeibullReliabilityFunction(alpha,beta,time);
				break;
			case MEASURE_CUMULATIVE:
				func = new WeibullCumulativeFunction(alpha,beta,time);
				break;
			case MEASURE_IPER1000:
				func = new WeibullI1000Function(alpha,beta,time);
				break;
			case MEASURE_INTENSITY:
				func = new WeibullIntensityFunction(alpha,beta,time);
				break;
		}
	}

	return func;
}

/**
 * Creates an OperatorFunction that can be used to compute the specified parameter.
 * This can be used in conjuction with such functions as computeJointDensity()
 * @param param type of parameter identifier
 * @param node node index
 * @param mode failure mode index
 */
OperatorFunction * BRASSEngine::createParameterFunction(int param, int node, int mode) const
{
	if (model == 0) return 0;

	OperatorFunction * func = 0;
	int alpha = getScaleIndex(node,mode);
	int beta = getShapeIndex(mode);
	switch (param) {
		case PARAMETER_SHAPE:
			func = new IndexFunction(beta);
			break;
		case PARAMETER_SCALE:
			func = new ExpIndexFunction(alpha);
			break;
		case PARAMETER_LOGSCALE:
			func = new Log10WrapFunction(new ExpIndexFunction(alpha));
			break;
	}
	return func;
}

/**
 * Creates an OperatorFunction that can be used to compute the specified adjustment.
 * This can be used in conjuction with such functions as computeJointDensity()
 * @param adjustment type of adjustment identifier
 * @param node0 node 0 index
 * @param node1 node 1 index
 * @param mode failure mode index
 */
OperatorFunction * BRASSEngine::createAdjustmentFunction(int adjustment, int node0, int node1, int mode) const
{
	OperatorFunction * func = 0;

	int alpha = getScaleIndex(node0,mode);
	int eta = getScaleIndex(node1,mode);
	int beta = getShapeIndex(mode);

	switch (adjustment) {
		case ADJUSTMENT_INTENSITY:
			func = new ExpWrapFunction(new LnZFunction(alpha,eta,beta));
			break;
		case ADJUSTMENT_TIME:
			func = new ExpWrapFunction(new LnYFunction(alpha,eta,beta));
			break;
	}
	return func;
}

/**
 * Retrieves sample chains for the specified function.
 * This method fills the array with the chain values obtained by
 * computing the function for the specified part of the sample chain.
 * @param func function
 * @param start index of first sample in sample set
 * @param length number of samples to be processed
 * @param arr array in which to store results.
 * @see #createParameterFunction()
 * @see #createAdjustmentFunction()
 * @see #createMeasureFunction()
 */
int BRASSEngine::getChain(OperatorFunction * func, int start, int length, double * arr)
{
	if (func == 0) return BRASSError::ERR_GENERAL;
	ChainOperator co = ChainOperator(func,start,length,arr);
	samples->execute(co);
	return 0;
}

void BRASSEngine::setModeNames(const char ** names, int name_cnt)
{
	mode_names.clear();
	for (int i = 0 ; i < name_cnt ; i++) 
	{
		cout << "MODE " << i + 1 << " : " << names[i] << endl;
		mode_names.push_back(names[i]);
	}
}

const string BRASSEngine::getModeName(int mode)
{
	int name_cnt = (int)mode_names.size();
	if (mode > 0 && mode <= name_cnt) 
	{
		return mode_names[mode - 1];
	} 
	else 
	{
		return string("") + "mode " + std::to_string(mode);
	}
}

/**
 * Sets the product node names.
 * Node names are used when generating warning messages.
 */
void BRASSEngine::setNodeNames(const char ** names, int name_cnt)
{
	node_names.clear();
	for (int i = 0 ; i < name_cnt ; i++)
	{
		cout << "NODE " << i << " : " << names[i] << endl;
		node_names.push_back(names[i]);
	}
}

/**
 * Returns the name of the specified node.
 * A generic name is returned if the product node names have 
 * not been specified. The names are specified by calling
 * setNodeNames().
 */
const string BRASSEngine::getNodeName(int node)
{
	int name_cnt = (int)node_names.size();
	if (node >= 0 && node < name_cnt) 
	{
		return node_names[node];
	} 
	else 
	{
		return string("") + "node " + std::to_string(node);
	}
}

/**
 * Copies the next message to the buffer.
 * The len parameter indicates the length of the buffer.
 * Returns true if a message was returned.
 */
bool BRASSEngine::nextMessage(char * msg, int len)
{
	string msg_str;
	if (msg_lst.next(msg_str) > 0)
	{
		strncpy(msg,msg_str.c_str(),len - 1);
		return true;
	}
	else 
	{
		msg[0] = 0;
		return false;
	}
}

/**
 * Abort the simulation.
 */
void BRASSEngine::requestCancel()
{
	status.requestCancel();
}

double BRASSEngine::computeWeibullMTTF(double dShape, double dScale)
{
	return exp(DCDFLIBInterface::computeLnGamma(1 + 1/dShape)) * dScale;
}