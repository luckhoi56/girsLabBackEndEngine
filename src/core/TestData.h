// $Id: TestData.h 352 2005-03-11 17:08:23Z saulo $

#pragma once

/**
 * Test data record.
 * Record is used to store information for one test.
 */
struct TestDataRecord 
{
	double start;	// start time
	double end;		// end time
	int mode;		// failure mode, 0 for no failure
	int condition;	// stress condition identifier
	double weight;	// weight (0 - 1) representing data strength (1 = full, 0 = none)
	double confidence; // confidence of fixes (design credit)
};

/**
 * Test data set.
 * This class is a container for the actual data. It consists of a fixed 
 * number of records. Each record has values for start time, end time, 
 * failure mode (0 for no failure) and test condition (0 for no acceleration).
 * 
 * Instances of the class are intended to be shared by multiple NetOperator 
 * instances: e.g. if two operators are created for the two Weibull parameters, 
 * then these operators share the test data set. Operators sharing the test data
 * are responsible for destroying the data set as soon as it is no longer in use.
 */
class TestData
{
public:
	TestData(int size);

	~TestData(void);
	/// Obtains a handle to the object. 
	TestData * obtain(void) {use_count++; return this;}
	/// Release the object. Returns true if no handles are left
	bool release() {return (use_count--) == 0;}
	/// Returns the number of handles for this object.
	int getUseCount(void) const {return use_count;}
	/// Returns the record with specified index
	const TestDataRecord * getRecord(int index) const {return &records[index];}
	/// Returns the number of records
	int getRecordCount(void) const {return record_count;}

	void setRecord(const int index, const double start, 
		const double end, const int mode, const int condition = 0, 
		double strength = 1, double credit = 0);

	const static int MODE_ANY = -1;
	const static int MODE_SUCCESS = 0;

private:
	int use_count;				// number of operators referring to this set
	int record_count;			// number of data records
	TestDataRecord * records;	// array of records
};
