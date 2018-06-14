// $Id: TestData.cpp 169 2005-01-28 14:17:28Z saulo $

#include "testdata.h"

/**
 * Constructs a testdata object of the specified size.
 * @param size required number of records.
 */
TestData::TestData(int size)
{
	records = new TestDataRecord[size];
	record_count = size;
	use_count = 0;
}

TestData::~TestData(void)
{
	delete [] records;
}

/**
 * Sets the information for the specified record.
 * @param index record index
 * @param start test start time
 * @param end test stop time
 * @param mode test failure mode. 0 for success.
 * @param condition test stress condition.
 * @param confidence confidence of fixes
 */
void TestData::setRecord(const int index, const double start, 
						 const double end, const int mode, const int condition,
						 const double weight, const double confidence)
{
	records[index].start = start;
	records[index].end = end;
	records[index].mode = mode;
	records[index].condition = condition;
	records[index].weight = weight;
	records[index].confidence = confidence;
}
