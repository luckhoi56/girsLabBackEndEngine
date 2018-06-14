// DistributionManager.h: interface for the DistributionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTIONMANAGER_H__D1C3A03F_C78A_4546_B7DC_82EAD92A4F72__INCLUDED_)
#define AFX_DISTRIBUTIONMANAGER_H__D1C3A03F_C78A_4546_B7DC_82EAD92A4F72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricDistribution.h"
#include "SyncMutex.h"

#define MAX_DISTRIBUTIONS 32000

class DistributionManager  
{
public:
	int add(ParametricDistribution * dist);
	ParametricDistribution * retrieve(int handle);
	ParametricDistribution * remove(int handle);

	DistributionManager();
	virtual ~DistributionManager();
	static DistributionManager * getInstance() {return &instance;}

private:
	static DistributionManager instance;
	ParametricDistribution * distributions[MAX_DISTRIBUTIONS];
	SyncMutex mutex;
};

#endif // !defined(AFX_DISTRIBUTIONMANAGER_H__D1C3A03F_C78A_4546_B7DC_82EAD92A4F72__INCLUDED_)
