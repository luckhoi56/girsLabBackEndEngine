// $Id: GroupedData.h 2184 2013-10-20 17:34:55Z Dongfeng Zhu $

#pragma once

/**
 * Grouped data record.
 */
struct GroupedDataRecord
{
	double start;
	double end;
	double weight;
	double population;
	double events;
};

/**
 * Grouped data set.
 *
 * This class is used to store data for grouped data,
 * i.e., to record the number of events within a population,
 * within a given observation interval.
 *
 * Data corresponds either to a single failure mode (with
 * index greater than zero) or the aggregate of all failure 
 * modes (fail_mode field == MODE_ANY).
 */
class GroupedData
{
public:
	GroupedData(const int size, const int failmode, const int repair);
	~GroupedData(void);
	GroupedData * obtain(void) {use_count++; return this;}
	bool release() {return (--use_count) == 0;}
	int getUseCount() const {return use_count;}
	const GroupedDataRecord * getRecord(const int index) const {return &records[index];}
	int getRecordCount() const {return record_count;}
	void setRecord(const int index, const double start, const double end,
		const double population, const double failures, double strength = 1);
	int getMode() const {return fail_mode;}
	int getRepair() const {return repair_assumption;}
	const static int MODE_ANY = -1;		// 'any failure mode' identifier.
	/// Grouped data: 'no repair' assumption
	static const int REPAIR_NONE = 6;
	/// Grouped data: as-good-as-old assumption
	static const int REPAIR_IMPERFECT = 5;

private:
	int use_count;					// number of references
	int record_count;				// number of data records
	GroupedDataRecord * records;    // pointer to records
	int fail_mode;					// failure mode index
	int repair_assumption;			// repair assumption
};
