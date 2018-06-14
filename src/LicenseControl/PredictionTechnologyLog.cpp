// ItemLog.cpp: implementation of the CPredictionTechnologyLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PredictionTechnologyLog.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>
#include "SystemInfo.h"
#include "CopyControl.h"
#include <iostream>
#include <sstream>

#pragma comment(lib,"ws2_32.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCopyControl theCtrl;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPredictionTechnologyLog::CPredictionTechnologyLog()
{
}

CPredictionTechnologyLog::~CPredictionTechnologyLog()
{

}

string CPredictionTechnologyLog::getPostLicenseParameterString(MODULE_LICENSE_INFO *pLicFileInfo)
{
	std::string strPostValue = "";

	char strMachineName[255];
	DWORD nSize=sizeof(strMachineName);
	GetComputerNameA(strMachineName, &nSize);

	std::string computerName(strMachineName);

	SystemInfo sysInfo;

	if (pLicFileInfo->m_bDemoMode) {
		strPostValue += "ComputerName=" + computerName;
		strPostValue += "&Company= ";
		strPostValue += "&LicenseMode=Demo";
		strPostValue += "&LicenseID= ";
		strPostValue += "&ComputerID=" + theCtrl.ConvertLongToString(theCtrl.getComputerID());
		strPostValue += "&OperatingSys=" + sysInfo.GetWinVerStr();
	} else {
		strPostValue += "ComputerName=" + computerName;
		strPostValue += "&Company=" + theCtrl.getRegCompanyName(pLicFileInfo);
		strPostValue += "&LicenseMode=Full";
		strPostValue += "&LicenseID=" + theCtrl.getLicenseID(pLicFileInfo);
		strPostValue += "&ComputerID=" + theCtrl.ConvertLongToString(theCtrl.getComputerID());
		strPostValue += "&OperatingSys=" + sysInfo.GetWinVerStr();
	}

	return strPostValue;
}
