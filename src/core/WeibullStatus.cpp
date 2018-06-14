// $Id: Status.cpp 522 2005-05-11 18:02:47Z fgroen $

#include "weibullstatus.h"

#include <string.h>

WeibullStatus::WeibullStatus(void)
{
	this->status = STATUS_READY;
	this->progress = 0;
	this->cancel = false;
}

WeibullStatus::~WeibullStatus(void)
{
}

/**
 * Returns the status code, along with a descriptive text.
 */
int WeibullStatus::getStatus(char * msg, int len) const
{
	int status = this->status;

	switch(status) 
	{
		case WeibullStatus::STATUS_SAMPLE:
			strncpy(msg,"Sampling...",len);
			break;
		case WeibullStatus::STATUS_BURNIN:
			strncpy(msg,"Burnin...",len);
			break;
		case WeibullStatus::STATUS_READY:
			strncpy(msg,"Ready.",len);
			break;
		case WeibullStatus::STATUS_CONVERGENCE:
			strncpy(msg,"Checking convergence...",len);
			break;
	}
	return status;
}
