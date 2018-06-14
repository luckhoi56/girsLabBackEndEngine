#include "NetCorrelation.h"

NetCorrelation::NetCorrelation(void)
{
	next = 0;
}

NetCorrelation::~NetCorrelation(void)
{
	delete next;
}
