// $Id: GroupedData.cpp 352 2005-03-11 17:08:23Z saulo $

#include "GroupedData.h"

/**
 * Constructor.
 * @param size number of records
 * @param failmode failure mode index or -1 (any failure mode).
 */
GroupedData::GroupedData(const int size, const int failmode, const int repair)
{
	records = new GroupedDataRecord[size];
	record_count = size;
	use_count = 0;
	fail_mode = failmode;
	repair_assumption = repair;
}

/**
 * Destructor.
 */
GroupedData::~GroupedData(void)
{
}

/**
 * Sets the information for the specified record.
 * @param index index of the record.
 * @param start observation interval start time
 * @param end observation interval end time
 * @param population observed population size
 * @param failures observed number of failures
 * @param strength statistical data strength (0-1).
 */
void GroupedData::setRecord(const int index, const double start, const double end,
		const double population, const double failures, double strength)
{
	records[index].start = start;
	records[index].end = end;
	records[index].events = failures;
	records[index].population = population;
	records[index].weight = strength;
}
