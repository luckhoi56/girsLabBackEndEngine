// DistributionManager.cpp: implementation of the DistributionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DistributionManager.h"
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * This class is used to manage instances of GenericDistribution 
 * objects. It basically consists of a list in which instances can 
 * be stored, so that they can be retrieved based on a handle generated
 * by the manager. The class is not responsible for creating or destroying
 * instances, even though it is important that when a distribution is 
 * no longer needed (or in fact destroyed), it is removed from the manager
 * list.
 *
 * Currently the number of distributions has a fixed maximum (MAX_DISTRIBUTIONS)
 */

DistributionManager DistributionManager::instance = DistributionManager();

DistributionManager::DistributionManager()
{
	for (int i = 0 ; i < MAX_DISTRIBUTIONS ; i++) distributions[i] = 0;
}

DistributionManager::~DistributionManager()
{
	for (int i = 0 ; i < MAX_DISTRIBUTIONS ; i++) {
		distributions[i]->releaseMemory();
		delete distributions[i];
		distributions[i] = 0;
	}
}

GenericDistribution * DistributionManager::remove(int handle)
{
	GenericDistribution * retval = 0;

	if (handle < 0) return 0;

	if (mutex.obtain()) {
		retval = distributions[handle];
		distributions[handle] = 0;

		mutex.release();
	}

	return retval;
}

GenericDistribution * DistributionManager::retrieve(int handle)
{
	if (handle < 0) return 0;
	GenericDistribution * retval = distributions[handle];
	return retval;
}

/** 
 * Adds a distribution, and returns handle to it. Handle has a value of 
 * 1 or greater. Returns -1 if distribution could not be added 
 * (manager is full).
 */
int DistributionManager::add(GenericDistribution * dist)
{
	int retval = -1;

	if (mutex.obtain()) {
		int i;

		for (i = 0 ; i < MAX_DISTRIBUTIONS && distributions[i] != 0; i++) {}

		if (i == MAX_DISTRIBUTIONS) {
			retval = -1;
		} else {
			distributions[i] = dist;
			retval = i;
		}
	
		mutex.release();
	}

	return retval;
}
