// $Id: TimedMeasure.cpp 169 2005-01-28 14:17:28Z saulo $

#include "timedmeasure.h"

TimedMeasure::TimedMeasure(int rec_cnt, int measure)
{
	this->records = new MeasureRecord[rec_cnt];
	this->record_count = rec_cnt;
	this->measure = measure;
	this->use_count = 0;
}

TimedMeasure::~TimedMeasure(void)
{
	delete [] records;
}


/**
 * Sets the information for the specified record.
 */
void TimedMeasure::setRecord(const int index, const double time, const double lower, 
							 const double upper, const double confidence)
{
	records[index].time = time;
	records[index].lower = lower;
	records[index].upper = upper;
	records[index].confidence = confidence;
}
