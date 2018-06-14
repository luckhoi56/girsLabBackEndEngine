// ItemLog.h: interface for the CPredictionTechnologyLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREDICTIONTECHNOLOGYLOG_H__277E9B49_A4E4_4930_AA68_929193B81542__INCLUDED_)
#define AFX_PREDICTIONTECHNOLOGYLOG_H__277E9B49_A4E4_4930_AA68_929193B81542__INCLUDED_

#include "LicenseHandleMgr.h"

class CPredictionTechnologyLog
{
public:
    string getPostLicenseParameterString(MODULE_LICENSE_INFO *pLicFileInfo);
    CPredictionTechnologyLog();
    virtual ~CPredictionTechnologyLog();
};

#endif // !defined(AFX_PREDICTIONTECHNOLOGYLOG_H__277E9B49_A4E4_4930_AA68_929193B81542__INCLUDED_)
