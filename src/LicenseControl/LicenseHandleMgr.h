// LicenseHandleMgr.h: interface for the CLicenseHandleMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LICENSEHANDLEMGR_H__24E9B8F6_3910_4FCE_86AD_CDE77060083A__INCLUDED_)
#define AFX_LICENSEHANDLEMGR_H__24E9B8F6_3910_4FCE_86AD_CDE77060083A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "SyncMutex.h"

#define MAX_HANDLES 100				// Maximum there are 100 license files can be opened at the same time

using namespace std;

struct MODULE_LICENSE_INFO
{
	bool	m_bNewLicense;				// Indicate if this license is a new license
	bool	m_bInvalidLicenseDetected;	// This variable is used to indicate a invalid license has been
										// detected in the system and the platform has been converted to demo.
	bool    m_network_to_local;			//if it is a network version but we changed it to Local due to lack of 
										//license or network disconnection.
	string	m_sModuleName;				// Used in network license; name for the related network module
	string	m_SemName;					
	string	m_LicFileName;				// The name of the license file
	bool    m_bDemoMode;				// Indicate if the current license is in demo mode;
   	LONG	m_lfhandle;					// the handle to the license;
	LONG    m_semhandle;
	LONG    m_status;
	unsigned __int64   LIGHT_INFO;		// This data contains the information for light modules; if one bit
										// in this data has been set to 1, the corresponding module will be
										// loaded in light mode; 
										// Please note that the corresponding bit in MODULE_INFO data must also
										// be set to 1 in order for authorize the module in light mode.
	unsigned __int64   MODULE_INFO;		// This data contains the information for all authorized modules; for
										// both full and light model, the corresponding bit in this data must
										// be set
	bool    m_TimeDepLic;				// Indicate if the current license is a time trial license; please
										// note that the entire license must be either time trial mode or not;
										// The time trial cannot be set to module level at this point
};

class CLicenseHandleMgr  
{
public:
	CLicenseHandleMgr();
	virtual ~CLicenseHandleMgr();
	static CLicenseHandleMgr * getInstance() {return &instance;}

	int getLicenseFileHandle(string strLicenseFilePath);
	int remove(int iHandle);

	MODULE_LICENSE_INFO * getHandle(int iHandle);

private:
	static CLicenseHandleMgr instance;
	MODULE_LICENSE_INFO * licenseFileHandles[MAX_HANDLES];
	SyncMutex mutex;
};

#endif // !defined(AFX_LICENSEHANDLEMGR_H__24E9B8F6_3910_4FCE_86AD_CDE77060083A__INCLUDED_)
