//
// File:      $Id: NHPriorLogLog.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHPRIORLOGLOG_H__566EA856_BF7F_4BE6_8E35_BF046377F063__INCLUDED_)
#define AFX_NHPRIORLOGLOG_H__566EA856_BF7F_4BE6_8E35_BF046377F063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHPrior.h"

class NHPriorLogLog : public NHPrior  
{
public:
	double getLogPrior(const Parameters &params);
	NHPriorLogLog();
	NHPriorLogLog(double md_nu, double md_tau, double ef_nu, double ef_tau);
	virtual ~NHPriorLogLog();
private:
	double md_nu;
	double md_tau;
	double ef_nu;
	double ef_tau;
};

#endif // !defined(AFX_NHPRIORLOGLOG_H__566EA856_BF7F_4BE6_8E35_BF046377F063__INCLUDED_)
