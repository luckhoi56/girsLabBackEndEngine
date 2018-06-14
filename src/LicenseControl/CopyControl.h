#pragma once

#include <string>
#include "LicenseHandleMgr.h"

using namespace std;

// The following defines the validation type for event data
#define POSITIVEVALUE	0
#define MODULECODE		1

#define AUTORIZED_MODULES    VAR_UDEF_NUM_1
#define LIGHT_MODULES        VAR_UDEF_NUM_2   //Light vs Full version for single user only

#define COPY_CTRL_MAINTENANCE_EXPIRATION VAR_UDEF_DATE_5		
#define COPY_CTRL_SAVED_SESSION_ID		 VAR_UDEF_NUM_5

#define DEMOMODE	0
#define TRIALMODE	1
#define FULLMODE	2

class CCopyControl
{
public:
	CCopyControl(void);
	~CCopyControl(void);

	LONG	m_compno;
	string	m_strErrorMessage;

	LONG	getSessionID(MODULE_LICENSE_INFO * fLicenseHandle);
	LONG	getComputerID();
	string	getErrorMessage();

	int		getPlatformMode();

	void	Open_License_File(MODULE_LICENSE_INFO * fLicenseHandle);
	int		getDemoDaysLeft(MODULE_LICENSE_INFO * fLicenseHandle);
	int		getTrialDaysLeft(MODULE_LICENSE_INFO * fLicenseHandle);
	bool    statusChanged(MODULE_LICENSE_INFO * fLicenseHandle);
	void	setAppPath(string strAppPath);
	long	pp_copycheck_ctrl(long handle);
	LONG	Id_This_Computer_HD();
	LONG	CheckError(LONG errornum);
	void	ConvertToDemo(long days,long *);
	bool	ManualActivateSoftware(long lUserCode1, long lUserCode2, long lLicenseID, string strPassword, 
		string strCompanyName, MODULE_LICENSE_INFO * fLicenseHandle);
	bool	ActivateSoftware(long lTriggerCode, long lTriggerData, long lLicenseID, string strPassword, 
		string strCompanyName, string strLicenseUpdate, MODULE_LICENSE_INFO * fLicenseHandle);
	bool	onlineActivation(string strID, string strPassword, MODULE_LICENSE_INFO * fLicenseHandle);
	int		getModuleMode(long lModuleCode, MODULE_LICENSE_INFO * fLicenseHandle);
	string	getRegCompanyName(MODULE_LICENSE_INFO * fLicenseHandle);
	int		GetMaintenanceExpirationDate(MODULE_LICENSE_INFO * pLicFileInfo);
	bool	checkLicenseStatus(MODULE_LICENSE_INFO *pLicFileInfo);
	bool	checkProductUpdate(MODULE_LICENSE_INFO *pLicFileInfo, long lAppMajor, long lAppMinor, long lAppRelease);
	string	getLicenseID(MODULE_LICENSE_INFO *pLicFileInfo);
	string	getLatestReleaseInformation();
	string	ConvertIntToString(int iValue);
	string	ConvertLongToString(long iValue);

private:
	bool	OnlineLicenseActivation(long lLicenseID, string strPassword, MODULE_LICENSE_INFO * fLicenseHandle);
	bool	OnlineInvoiceActivation(long ID, string strPassword, MODULE_LICENSE_INFO * fLicenseHandle);
	bool	ValidateInputData(long lData, int iValidationType);

	void	SetMaintainExpirationDate(MODULE_LICENSE_INFO * pLicFileInfo, long lDate);
	void	SetMaintainExpirationDate(MODULE_LICENSE_INFO * pLicFileInfo, long lMonth, long lDay, long lYear);

	void	SetFullVerModules(MODULE_LICENSE_INFO *pLicFileInfo, long lModules, 
		long lLicenseID, string strPassword, string strCompanyName,
		string strLicenseUpdate, bool bTrialMode = FALSE);
	void	SetLightVerModules(MODULE_LICENSE_INFO *pLicFileInfo, long lModules, 
		long lLicenseID, string strPassword, string strCompanyName,
		string strLicenseUpdate, bool bTrialMode = FALSE);
	bool	ClearTrialDate(MODULE_LICENSE_INFO * pLicFileInfo);
	bool	ExtendTrialDate(MODULE_LICENSE_INFO * pLicFileInfo, long days);
	bool	SetTrialDate(MODULE_LICENSE_INFO * pLicFileInfo, long days);
	bool	ReCreate_License_File(MODULE_LICENSE_INFO *pLicFileInfo);
	bool	Create_License_File(char * strPath, MODULE_LICENSE_INFO *pLicFileInfo, bool bCreateAliasLicense);
	bool	fileExist(const char * filename);
	bool	isExpDateValid(string strExpDate);

	string	m_strAppPath;
	unsigned __int64   MODULES_AUTORIZED_TO_RUN;
	string	m_strLatestReleaseInformation;
};
