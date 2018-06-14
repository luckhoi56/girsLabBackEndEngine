// $Id: TimedMeasure.h 1089 2005-08-08 16:31:42Z fgroen $

#pragma once

/**
 * Test data record.
 * Record is used to store information for one test.
 */
struct MeasureRecord 
{
	double time;
	double upper;	
	double lower;	
	double confidence;
};

/**
 * Container class for custom prior records.
 * Custom priors are specified in the form of upper and lower bounds
 * on some time dependent reliability measure.
 */
class TimedMeasure
{
public:
	TimedMeasure(int rec_cnt, int measure);
	~TimedMeasure(void);
	/// Obtains a handle to the object. 
	TimedMeasure * obtain(void) {use_count++; return this;}
	/// Release the object. Returns true if no handles are left
	bool release() {use_count--; return use_count == 0;}
	/// Returns the number of handles for this object.
	int getUseCount(void) const {return use_count;}
	/// Returns the record with specified index
	const MeasureRecord * getRecord(int index) const {return &records[index];}
	/// Returns the number of records
	int getRecordCount(void) const {return record_count;}

	void setRecord(const int index, const double time, 
		const double lower, const double upper, const double confidence);
	int getMeasure() const {return measure;}

	/// measure identifier: reliability (corresponds to BRASSEngine).
	const static int MEASURE_RELIABILITY = 11;
	/// measure identifier: cumulative failures (corresponds to BRASSEngine).
	const static int MEASURE_CUMULATIVE = 14;
	const static int MEASURE_IPER1000 = 12;
	/// measure identifier: failure intensity (corresponds to BRASSEngine).
	const static int MEASURE_INTENSITY = 13;

private:
	int use_count;				///< number of operators referring to this set
	int record_count;			///< number of data records
	MeasureRecord * records;	///< array of records
	int measure;				///< measure type identifier

};
