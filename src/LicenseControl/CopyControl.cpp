#include "StdAfx.h"
#include "CopyControl.h"
#include "softwarekey\keylib.h"
#include "softwarekey\Skca.h"
#include "time.h"
#include "Winsock2.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

CCopyControl::CCopyControl(void)
{
	MODULES_AUTORIZED_TO_RUN = 0;
	m_strLatestReleaseInformation = "";
}

CCopyControl::~CCopyControl(void)
{
}

LONG CCopyControl::getComputerID()
{
	m_compno = pp_compno(COMPNO_HDSERIAL, "", "C");
	return m_compno;
}

LONG CCopyControl::getSessionID(MODULE_LICENSE_INFO * fLicenseHandle)
{
	// See if we have a previous session code
	// (refer to Delayed Trigger Codes in the index for information)
	long sessionCode = 0;
	
	if (fLicenseHandle->m_lfhandle != 0)
	{
		pp_getvarnum(fLicenseHandle->m_lfhandle, COPY_CTRL_SAVED_SESSION_ID, &sessionCode);
	}
	
	if (sessionCode == 0)
	{
		sessionCode = pp_cenum();
		pp_setvarnum(fLicenseHandle->m_lfhandle, COPY_CTRL_SAVED_SESSION_ID, sessionCode);
	}

	return sessionCode;
}

string CCopyControl::getErrorMessage()
{
	return m_strErrorMessage;
}

void CCopyControl::Open_License_File(MODULE_LICENSE_INFO * fLicenseHandle)
{
	ifstream licenseFile(fLicenseHandle->m_LicFileName.c_str());
	if (licenseFile.is_open())
	{
		licenseFile.close();
		fLicenseHandle->m_status = pp_lfopen((char *)fLicenseHandle->m_LicFileName.c_str(), 
			LF_CREATE_HIDDEN, LF_FILE, "RDATV20", &(fLicenseHandle->m_lfhandle));
		fLicenseHandle->m_bNewLicense = false;

		checkLicenseStatus(fLicenseHandle);
	}
	else
	{
		// The license file does not exist yet
		fLicenseHandle->m_status = pp_lfopen((char *)fLicenseHandle->m_LicFileName.c_str(), 
			LF_CREATE_MISSING | LF_CREATE_HIDDEN, LF_FILE, "RDATV20", &(fLicenseHandle->m_lfhandle));
		getComputerID();
		pp_copyadd(fLicenseHandle->m_lfhandle, COPYADD_ERASEALL, m_compno);	
		ConvertToDemo(30, &(fLicenseHandle->m_lfhandle));

		fLicenseHandle->m_bNewLicense = true;
	}
}

int	CCopyControl::getDemoDaysLeft(MODULE_LICENSE_INFO * fLicenseHandle)
{
	long daysleft;
	char extype[2];
	if (fLicenseHandle->m_status == PP_TRUE)
	{
		pp_getvarchar(fLicenseHandle->m_lfhandle, VAR_EXPIRE_TYPE, extype);
		if (extype[0] == 'D')
		{
			// for demo version, use pp_daysleft function directly
			pp_daysleft(fLicenseHandle->m_lfhandle,&daysleft);
		}
		else
		{
			// For other mode, set expiration type to D first and convert it 
			// back to normal after use daysleft function
			pp_setvarchar(fLicenseHandle->m_lfhandle, VAR_EXPIRE_TYPE, "D");
			pp_daysleft(fLicenseHandle->m_lfhandle,&daysleft);
			pp_setvarchar(fLicenseHandle->m_lfhandle, VAR_EXPIRE_TYPE, extype);
		}
		return daysleft;
	}

	return 30;	
}

int	CCopyControl::getTrialDaysLeft(MODULE_LICENSE_INFO * fLicenseHandle)
{
	long daysleft = 0;
	char extype[2];
	if (fLicenseHandle->m_status == PP_TRUE)
	{
		pp_getvarchar(fLicenseHandle->m_lfhandle, VAR_EXPIRE_TYPE, extype);
		if (extype[0] == 'P')
		{
			// for demo version, use pp_daysleft function directly
			pp_daysleft(fLicenseHandle->m_lfhandle,&daysleft);
		}
	}
	
	return daysleft;	
}

bool CCopyControl::onlineActivation(string strID, string strPassword, MODULE_LICENSE_INFO * fLicenseHandle)
{
	bool bActivationResult = false;
	long ID;

	if ( (strcmp((strID.substr(0, 1)).c_str(), "I") == 0) ||
		(strcmp((strID.substr(0, 1)).c_str(), "i") == 0))
	{
		// This is an invoice ID
		strID = strID.substr(1, strID.length()-1);
		ID = atoi((char *)strID.c_str());
		bActivationResult = OnlineInvoiceActivation(ID, strPassword, fLicenseHandle);
	}
	else if ((strcmp(strID.substr(0, 1).c_str(), "L") == 0)	||
		(strcmp(strID.substr(0, 1).c_str(), "l") == 0))
	{
		// This is a license ID
		strID = strID.substr(1, strID.length()-1);
		ID = atoi((char *)strID.c_str());
		bActivationResult = OnlineLicenseActivation(ID, strPassword, fLicenseHandle);
	}

	return bActivationResult;
}

bool CCopyControl::OnlineInvoiceActivation(long ID, string strPassword, MODULE_LICENSE_INFO * fLicenseHandle)
{
	// The input ID is an invoice ID; 
	char strLicenseIDList[8000];
	long result;
	long SessionCode;
	long RegKey1;
	long TcData;	// trigger code data

	SessionCode = pp_cenum();
	result = SK_GetTCData("secure.softwarekey.com","","/solo/unlock/getinvoicelicenseidlist.asp",ID, 
		(LPSTR)strPassword.c_str(), SessionCode, m_compno, &RegKey1, &TcData, strLicenseIDList);

	bool bActivationStatus = true;

	if (result != SWKERR_NONE)
	{
		bActivationStatus = false;
	}
	else
	{
		// Activate the license in the list one by one
		char * pch;
		char * nextToken;
		pch = strtok_s (strLicenseIDList,"|", &nextToken);
		while (pch != NULL)
		{
			if (!(OnlineLicenseActivation(atoi(pch), strPassword, fLicenseHandle)))
				bActivationStatus = false;
			pch = strtok_s (NULL, "|", &nextToken);
		}
	}

	return bActivationStatus;
}

/************************************************************************/
/* This function activate iQT using the input trigger code and          */
/* Event data. Please note that the data are encrypted so it must be    */
/* decoded first before use.                                            */
/************************************************************************/

bool CCopyControl::OnlineLicenseActivation(long lLicenseID, string strPassword, MODULE_LICENSE_INFO * fLicenseHandle)
{
	long TcValue;	// trigger code value
	long TcData;	// trigger code data
	long lGetRegresult, lGetDataResult;
	long RegKey1;
	long SessionCode;
	char LicenseUpdate[56] = {0};
	char company[56] = {0};
	char contact[56] = {0};
	char ignore[55] = {0};

	SessionCode = pp_cenum();

	// Unconditionally clear out our stored session code
	// to prevent hacking on the stored session code
	// (refer to Delayed Trigger Codes in the index for information)
	// Get registration data
	lGetRegresult = SK_GetRegData("secure.softwarekey.com", "", "/solo/customers/getregdata.asp", lLicenseID, (char *)strPassword.c_str(),
		company, contact, ignore, ignore, ignore, ignore, ignore, ignore, ignore);

	lGetDataResult = SK_GetTCData("secure.softwarekey.com","","/solo/unlock/getcode.asp",lLicenseID, (char *)strPassword.c_str(), SessionCode, m_compno, &RegKey1, &TcData, LicenseUpdate);

	// See if a valid code was entered
	// Also decode the Trigger Code additional number, if applicable
	TcValue = pp_tcode(RegKey1, SessionCode, m_compno, 151);

	if (lGetDataResult == 0)
	{
		return ActivateSoftware(TcValue, TcData, lLicenseID, strPassword, company, LicenseUpdate, fLicenseHandle);
	}
	else if (lGetDataResult == 100) //If invalid license information was entered...
		m_strErrorMessage = "Invalid License Information.  Please check your License ID and Password and try again, or contact technical support for help.";
	else if (lGetDataResult == 7)//if no more keys are available...
		m_strErrorMessage = "No more SoftwareKeys available!  Please contact technical support for help.";
	else if (lGetDataResult != 0)
	{
		char buffer[20];
		_itoa_s(lGetDataResult, buffer, 10);
		string strErrorCode = buffer;
		m_strErrorMessage = "Error #" + strErrorCode + " occured.  Please contact technical support for help.";
	}
	else
		m_strErrorMessage = "Invalid Code Entered!";

	return FALSE;
}

bool CCopyControl::ManualActivateSoftware(long lUserCode1, long lUserCode2, long lLicenseID, string strPassword, 
							   string strCompanyName, MODULE_LICENSE_INFO * fLicenseHandle)
{
	// Load session code from memory
	long sessionCode = 0;

	if (fLicenseHandle->m_lfhandle != 0)
	{
		pp_getvarnum(fLicenseHandle->m_lfhandle, COPY_CTRL_SAVED_SESSION_ID, &sessionCode);
	}

	long lTriggerCode = pp_tcode(lUserCode1, sessionCode, m_compno, 151);

	return ActivateSoftware(lTriggerCode, lUserCode2, lLicenseID, strPassword, strCompanyName,
		"", fLicenseHandle);
}

bool CCopyControl::ActivateSoftware(long lTriggerCode, long lTriggerData, long lLicenseID, string strPassword, 
		string strCompanyName, string strLicenseUpdate, MODULE_LICENSE_INFO * fLicenseHandle)
{
	LONG lMonth, lDay, lYear;

	switch(lTriggerCode)
	{
	case 1:    
		{
			// Code 1 authorize single user full version with no limitations
			// EventData contains the module to be activated
			lTriggerData = pp_ndecrypt(lTriggerData, 32);
			if (!(ValidateInputData(lTriggerData, MODULECODE)))
				return false;
			SetFullVerModules(fLicenseHandle, lTriggerData, lLicenseID, strPassword, strCompanyName, strLicenseUpdate);
			break;
		}

	case 3:    
		{
			// Code 3 increases the trial time of this computer
			PP_SETVARNUM(fLicenseHandle->m_lfhandle, COPY_CTRL_SAVED_SESSION_ID, 0);
			lTriggerData = pp_ndecrypt(lTriggerData, 33);
			if (!(ValidateInputData(lTriggerData, POSITIVEVALUE)))
				return false;
			ExtendTrialDate(fLicenseHandle, lTriggerData);
			break;
		}

	case 4:    
		{
			// Code 4 authorize single user full version with 7 days time trial
			lTriggerData = pp_ndecrypt(lTriggerData, 88);
			if (!(ValidateInputData(lTriggerData, MODULECODE)))
				return false;
			SetFullVerModules(fLicenseHandle, lTriggerData, lLicenseID, strPassword, strCompanyName, strLicenseUpdate,
				TRUE);
			break;
		}

	case 47:
		{
			// Code 47 extend the maintenance expiration date for standalone version;
			// event data contains the number of years
 			int curExpirationDate = GetMaintenanceExpirationDate(fLicenseHandle);
 			lTriggerData = pp_ndecrypt(lTriggerData, 77);
 			if (!(ValidateInputData(lTriggerData, POSITIVEVALUE)))
 				return FALSE;
			lYear = curExpirationDate / 10000;
			lMonth = (curExpirationDate - lYear * 10000)/100;
			lDay = curExpirationDate - lYear * 10000 - lMonth * 100;
			
 			SetMaintainExpirationDate(fLicenseHandle, lMonth, lDay, lYear +lTriggerData);

			break;
		}

	case 48:
		{
			// Code 48 is used to set the maintenance expiration date for RDAT Plus. The data must
			// be in the format of yymm
			pp_setvarnum(fLicenseHandle->m_lfhandle, COPY_CTRL_SAVED_SESSION_ID, 0);
			lTriggerData = pp_ndecrypt(lTriggerData, 123);

			// The expiration date is in the format of yymm. We cannot put day inside due to the 
			// limitation of lTriggerData. The maximum number is 16383

			if ((lTriggerData > 0) && (lTriggerData < 9912))
			{
				lYear = lTriggerData/100;
				lMonth = lTriggerData - lYear * 100;
				lDay = 1;
				lYear += 2000;
				SetMaintainExpirationDate(fLicenseHandle, lMonth, lDay, lYear);
			}

			break;
		}

	case 49:    
		{
			// This code should deauthorize- this computer
			if (CheckError(pp_copydelete(fLicenseHandle->m_lfhandle, m_compno)) == PP_SUCCESS)
			{
				pp_setvarchar (fLicenseHandle->m_lfhandle, VAR_EXPIRE_TYPE, "D");
				pp_setvardate (fLicenseHandle->m_lfhandle, VAR_EXP_DATE_HARD, 1, 1, 2000);
				pp_upddate (fLicenseHandle->m_lfhandle, 0);
				pp_setvarnum(fLicenseHandle->m_lfhandle, COPY_CTRL_SAVED_SESSION_ID, 0);
			}
			break;		
		}

		default:
		{
			// Invalid code was entered.
			m_strErrorMessage = "Invalid Code Entered!";
			return false;
		}
	}

	// Reset the demo and new license status
	fLicenseHandle->m_bDemoMode = false;
	fLicenseHandle->m_bNewLicense = false;
	
	return true;
}

bool	CCopyControl::ValidateInputData(long lData, int iValidationType)
{
	switch (iValidationType)
	{
	case POSITIVEVALUE:
		{
			if (lData<0)
			{
				// Invalid number of days, return FALSE;
				return FALSE;
			}
			break;
		}

	case MODULECODE:
		{
			if ((lData<=0) || (lData>16383))
			{
				// Invalid number of days, return FALSE;
				return FALSE;
			}
			break;
		}
	}

	return TRUE;
}

bool CCopyControl::statusChanged(MODULE_LICENSE_INFO *pLicFileInfo)
{
	long result;
	long day, month, year;
	char extype[2];
	unsigned __int64 ModulesType = 0;
	unsigned __int64 ModuleLight = 0;
	string strMsg;
	string Month, Day, Year;
	static BOOL bClockFaultMsg = FALSE;

	// since we opened the License File, check the protection.

	// Controls the dialogs that are displayed. when this function exits
	// we are done with the main part of the program, clean up and exit
	long CPAlgorithm;
	
	// assume we are running off a local drive. (peer server)
	BOOL LocalDrive = true;

	// see if we are authorized to run on this computer or network
	if (pp_redir((char *)(m_strAppPath.c_str())))
	{
		LocalDrive = false;
	}

	// on peer servers, set copy protection ID algorithm to
	// BIOS (1) and HDSERIAL number (2)  - 1 + 2 = 3
	CPAlgorithm = COMPNO_BIOS | COMPNO_HDSERIAL;
     
   result = PP_SUCCESS;
   if (LocalDrive)
	{
		result = pp_copycheck_ctrl(pLicFileInfo->m_lfhandle);
		CheckError(result);
	}

	if (result == PP_SUCCESS)
	{
		/*****************************************************************/
		/* Check Version to see what version was license file was created */
		char strVersion[50];
		pp_getvarchar(pLicFileInfo->m_lfhandle,VAR_USER_DEF_1,strVersion);

		/***********AUTORIZED MODULES***************/
		// passed copy protection test - let's see what modules we should enable

		MODULES_AUTORIZED_TO_RUN = 0x0000FFFF;
		if (pp_getvarnum(pLicFileInfo->m_lfhandle, AUTORIZED_MODULES ,(long *)&ModulesType) == PP_SUCCESS)
		{
			MODULES_AUTORIZED_TO_RUN &= ModulesType;
			pLicFileInfo->MODULE_INFO = MODULES_AUTORIZED_TO_RUN;		
		}

		unsigned __int64 MODULES_LIGHT_VERSION = 0x0000FFFF;
		if (pp_getvarnum(pLicFileInfo->m_lfhandle, LIGHT_MODULES ,(long *)&ModuleLight) == PP_SUCCESS)
		{
			MODULES_LIGHT_VERSION &= ModuleLight;
			pLicFileInfo->LIGHT_INFO = MODULES_LIGHT_VERSION;		
		}
        
        /****EXPIRATION TYPE******/
		// passed copy protection test - let's see if we are in Payment mode
		pp_getvarchar(pLicFileInfo->m_lfhandle, VAR_EXPIRE_TYPE, extype);

		if (extype[0] == 'P')  //Time Trial Mode
		{
			// turn on our payment indicator
			pLicFileInfo->m_TimeDepLic = TRUE;
			pp_getvardate(pLicFileInfo->m_lfhandle, VAR_EXP_DATE_HARD, &month, &day, &year);
			Month = ConvertIntToString(month);
			Day = ConvertIntToString(day);
			Year = ConvertIntToString(year);
			result = pp_expired(pLicFileInfo->m_lfhandle);
			if (result == PP_TRUE)
			{
				// This computer Payment is past due
				m_strErrorMessage = "Your trial period is over. Please call for assistance.";

				pLicFileInfo->m_bDemoMode = TRUE;
			}
			else if (result == PP_FALSE)
			{
				// So far, so good. Let's make sure they didn't turn the clock back.
				if (pp_valdate(pLicFileInfo->m_lfhandle) == PP_FALSE)
				{
					if (bClockFaultMsg == FALSE)
					{
						m_strErrorMessage = "Your clock has been turned back. Please correct and re-run application.";
						bClockFaultMsg = TRUE;
					}
					pLicFileInfo->m_bDemoMode  = true;
					pLicFileInfo->m_bDemoMode = TRUE;
					return false;
				}
				else
				{
					pLicFileInfo->m_bDemoMode  = false;
				}
			}
			else
			{
				pLicFileInfo->m_bDemoMode  = false;
			}
		}

		/**************************************/
		else if (extype[0] == 'S')  //Shareware
		{		
			
			int i = 0;
			i++;
			if (pp_expired(pLicFileInfo->m_lfhandle))
			{
				// This computer Payment is past due
				m_strErrorMessage = "This shareware has expired. Please call for assistance.";
				pLicFileInfo->m_bDemoMode  = true;
			}
			else
			{
				// So far, so good. Let's make sure they didn't turn the clock back.
				if (pp_valdate(pLicFileInfo->m_lfhandle) == PP_FALSE)
				{
					if (bClockFaultMsg == FALSE)
					{
				        bClockFaultMsg = TRUE;
						m_strErrorMessage = "Your clock has been turned back. Please correct and re-run application.";
					}
					pLicFileInfo->m_bDemoMode  = true;
				}
				else
				{
					pLicFileInfo->m_bDemoMode  = false;
				}
			}
		}
		else if (extype[0] == 'E')  //execution Count
		{

            if (pp_expired(pLicFileInfo->m_lfhandle) == PP_TRUE)
			{
				// This computer Payment is past due
				m_strErrorMessage = "This software has expired. Please call for assistance.";
				pLicFileInfo->m_bDemoMode  = true;
			}
			else
			{
				CheckError(pp_countinc( pLicFileInfo->m_lfhandle, VAR_EXP_COUNT ));
				pLicFileInfo->m_bDemoMode  = false;
			}
		}
		else if (extype[0] == 'N')
		{
			// Turn on all menu options since everything is okay
			pLicFileInfo->m_bDemoMode  = false;

		}
		else if (extype[0] == 'D')
		{
			pLicFileInfo->m_bDemoMode  = true;
		}
	
		else 
		{
			// Turn on all menu options to Demo
			ConvertToDemo(30,&pLicFileInfo->m_lfhandle);
			pLicFileInfo->m_bDemoMode  = true;
		}
	}
	else
	{
		// License File is okay, but this computer is not authorized.
		// See if this is in demo mode
		pp_getvarchar(pLicFileInfo->m_lfhandle, VAR_EXPIRE_TYPE, extype);
		if (extype[0] == 'D')
		{
			// Turn on the demo indicator
			pp_getvardate(pLicFileInfo->m_lfhandle, VAR_EXP_DATE_HARD, &month, &day, &year);
			Month = ConvertIntToString(month);
			Day = ConvertIntToString(day);
			Year = ConvertIntToString(year);
			result = pp_expired(pLicFileInfo->m_lfhandle);
			if (result == PP_TRUE) 
			{
				// This demo has expired
				m_strErrorMessage = "This demo has expired. Please call for assistance.";
				pLicFileInfo->m_bDemoMode  = true;				
			}
			else
			{
				// So far, so good. Let's make sure they didn't turn the clock back
				if (pp_valdate(pLicFileInfo->m_lfhandle) == PP_FALSE)
				{
					if (bClockFaultMsg == FALSE)
					{
					    m_strErrorMessage = "Your clock has been turned back. Please correct and re-run application.";
						bClockFaultMsg = TRUE;
					}
				}
				else
				{
					pLicFileInfo->m_bDemoMode  = true;
				}
			}
		}
		else
		{
			// This is not a demo but the copy protection failed the test
			// convert this copy back to a 30-day demo.
			m_strErrorMessage = "Could not read the copy control license file!\nPlease try again later.";

			return FALSE;
		}
	}

	return TRUE;
}

void CCopyControl::setAppPath(string strPath)
{
	m_strAppPath = strPath;
}

long CCopyControl::pp_copycheck_ctrl(long handle)
{
	getComputerID();
	if (pp_copycheck(handle, 0, m_compno) == PP_SUCCESS)
		return PP_SUCCESS;

	getComputerID();
	return pp_copycheck(handle, 0, m_compno);
}

LONG CCopyControl::CheckError(LONG errornum)
{
	// convert the error number to a string.
	char Buffer[80];
	pp_errorstr(errornum, Buffer);
	
	switch(errornum)
	{
		// ignore the first two cases
	case PP_FAILURE:
		break;
	case PP_SUCCESS:
		break;
	default:
		m_strErrorMessage = "Error occured" + ConvertIntToString(errornum) + Buffer;
		exit(1);
		break;
	}
	
	return errornum;
}

void CCopyControl::ConvertToDemo(long days,long *plfhandle)
{
	long month, day, year, dow; // month day year and day_of_week
	string MessageStr;

	// Set Demo Expiration to 30 days from today
	pp_getdate(&month, &day, &year, &dow);
	pp_adddays(&month, &day, &year, days);
	pp_setvarchar(*plfhandle, VAR_EXPIRE_TYPE, "D");
	
	if (CheckError(pp_setvardate(*plfhandle, VAR_EXP_DATE_HARD, month, day, year)) == PP_SUCCESS)
	{
		// convert to demo successfully
		m_strErrorMessage = "Demo Sets to 30 days from today";
	}
}

string	CCopyControl::ConvertIntToString(int iValue)
{
	string output;

	std::stringstream ss;
	ss << iValue;
	ss >>output;

	return output;
}

string	CCopyControl::ConvertLongToString(long iValue)
{
	string output;

	std::stringstream ss;
	ss << iValue;
	ss >>output;

	return output;
}

void CCopyControl::SetFullVerModules(MODULE_LICENSE_INFO *pLicFileInfo, long lModules, 
									long lLicenseID, string strPassword, string strCompanyName,
									string strLicenseUpdate, bool bTrialMode)
{
	long lAuthorizedModules;
	long lLightModules;

	pp_setvarnum(pLicFileInfo->m_lfhandle, COPY_CTRL_SAVED_SESSION_ID, 0);
	
	PP_SETVARNUM(pLicFileInfo->m_lfhandle, VAR_LICENSEID, lLicenseID);
	PP_SETVARCHAR(pLicFileInfo->m_lfhandle, VAR_LICENSEPW, (char *)strPassword.c_str());
	
	if (pLicFileInfo->m_TimeDepLic != bTrialMode)
	{
		// The current version is different with the input mode; reset the authorized module and light
		// module information
		CheckError(pp_setvarnum(pLicFileInfo->m_lfhandle, AUTORIZED_MODULES ,0));
		CheckError(pp_setvarnum(pLicFileInfo->m_lfhandle, LIGHT_MODULES ,0));
		lLightModules = 0;
		lAuthorizedModules = lModules;
	}
	else
	{
		// Get current authorized module information from the license file; set the new module
		// bit in the authorized module data
		PP_GETVARNUM(pLicFileInfo->m_lfhandle, AUTORIZED_MODULES ,&lAuthorizedModules);
		lAuthorizedModules |= lModules;
		
		// Remove the new modules from the light list; This is used to handle the situation when
		// user purchase the light version first, then convert to a full version
		
		// Get the light module definition in the current license file.
		PP_GETVARNUM(pLicFileInfo->m_lfhandle, LIGHT_MODULES , &lLightModules);
		lLightModules &= (~lModules);
	}
	
	if (pp_copycheck_ctrl(pLicFileInfo->m_lfhandle) != PP_SUCCESS)
		CheckError(pp_copyadd(pLicFileInfo->m_lfhandle, COPYADD_ERASEALL, m_compno));
	else
	{
		CheckError(pp_setvarnum(pLicFileInfo->m_lfhandle, AUTORIZED_MODULES ,lAuthorizedModules));
		CheckError(pp_setvarnum(pLicFileInfo->m_lfhandle, LIGHT_MODULES , lLightModules));

		if (!bTrialMode)
			CheckError(pp_setvarchar(pLicFileInfo->m_lfhandle, VAR_EXPIRE_TYPE, "N"));
		else
		{
			CheckError(pp_setvarchar(pLicFileInfo->m_lfhandle, VAR_EXPIRE_TYPE, "P"));
			SetTrialDate(pLicFileInfo, 7);
		} 

		// Set Company Name information
		CheckError(pp_setvarchar(pLicFileInfo->m_lfhandle, VAR_COMPANY, (char *)strCompanyName.c_str()));
		
		if (isExpDateValid(strLicenseUpdate)) {
			strLicenseUpdate = strLicenseUpdate.substr(0, 8);
			SetMaintainExpirationDate(pLicFileInfo, atoi(strLicenseUpdate.c_str()));
		}
		else {
			SetMaintainExpirationDate(pLicFileInfo, 0);
		}
	}
}

void CCopyControl::SetLightVerModules(MODULE_LICENSE_INFO *pLicFileInfo, long lModules, 
									 long lLicenseID, string strPassword, string strCompanyName,
									 string strLicenseUpdate, bool bTrialMode)
{
	long lAuthorizedModules;
	long lLightModules;
	
	pp_setvarnum(pLicFileInfo->m_lfhandle, COPY_CTRL_SAVED_SESSION_ID, 0);
	
	PP_SETVARNUM(pLicFileInfo->m_lfhandle, VAR_LICENSEID, lLicenseID);
	PP_SETVARCHAR(pLicFileInfo->m_lfhandle, VAR_LICENSEPW, (char *)strPassword.c_str());
	
	if (pLicFileInfo->m_TimeDepLic != bTrialMode)
	{
		// The current version is differnt with the input mode; reset the authorized module and light
		// module information
		CheckError(pp_setvarnum(pLicFileInfo->m_lfhandle, AUTORIZED_MODULES ,0));
		CheckError(pp_setvarnum(pLicFileInfo->m_lfhandle, LIGHT_MODULES ,0));
		lAuthorizedModules = lModules;
		lLightModules = lModules;
	}
	else
	{
		// Get current authorized module information from the license file; set the new module
		// bit in the authorized module data
		PP_GETVARNUM(pLicFileInfo->m_lfhandle, AUTORIZED_MODULES ,&lAuthorizedModules);
		lAuthorizedModules |= lModules;
		
		// Get the light module definition in the current license file.
		PP_GETVARNUM(pLicFileInfo->m_lfhandle, LIGHT_MODULES , &lLightModules);
		
		lLightModules |= lModules;
	}
	
	if (pp_copycheck_ctrl(pLicFileInfo->m_lfhandle) != PP_SUCCESS)
		CheckError(pp_copyadd(pLicFileInfo->m_lfhandle, COPYADD_ERASEALL, m_compno));
	else
	{
		CheckError(pp_setvarnum(pLicFileInfo->m_lfhandle, AUTORIZED_MODULES ,lAuthorizedModules));
		CheckError(pp_setvarnum(pLicFileInfo->m_lfhandle, LIGHT_MODULES , lLightModules));
		CheckError(pp_setvarchar(pLicFileInfo->m_lfhandle, VAR_EXPIRE_TYPE, "N"));
		
		if (!bTrialMode)
			CheckError(pp_setvarchar(pLicFileInfo->m_lfhandle, VAR_EXPIRE_TYPE, "N"));
		else
		{
			CheckError(pp_setvarchar(pLicFileInfo->m_lfhandle, VAR_EXPIRE_TYPE, "P"));
			SetTrialDate(pLicFileInfo, 7);
		}

		CheckError(pp_setvarchar(pLicFileInfo->m_lfhandle, VAR_COMPANY,(char *)strCompanyName.c_str()));
		if (isExpDateValid(strLicenseUpdate)) {
			strLicenseUpdate = strLicenseUpdate.substr(0, 8);
			SetMaintainExpirationDate(pLicFileInfo, atoi(strLicenseUpdate.c_str()));
		}
		else {
			SetMaintainExpirationDate(pLicFileInfo, 0);
		}
	}
}

// This function will extend the current expiration date for input number of days.
// Please note that the input number of days could be a negative number which allows
// user to decrease the trial/demo time
// 
// Note: This function will not set the expiration type. User must set the type of expiration
// out of this function.
bool CCopyControl::ExtendTrialDate(MODULE_LICENSE_INFO * pLicFileInfo, long days)
{
	if (pLicFileInfo != NULL)
	{
		long month, day, year, row;
		
		// Extend trial date to the given date
		pp_getvardate(pLicFileInfo->m_lfhandle, VAR_EXP_DATE_HARD, &month, &day, &year);
		
		if ((year == 0) || (PP_EXPIRED(pLicFileInfo->m_lfhandle)))
		{
			PP_GETDATE(&month, &day, &year, &row);
		}
		pp_adddays(&month, &day, &year, days);
		
		// store the date and expiration type
		CheckError(pp_setvardate(pLicFileInfo->m_lfhandle, VAR_EXP_DATE_HARD, month, day, year));		
		
		// It is a good idea to update the last used date/time fields
		pp_upddate(pLicFileInfo->m_lfhandle, 0);
		
		return TRUE;
	}
	
	return FALSE;
}

bool CCopyControl::SetTrialDate(MODULE_LICENSE_INFO * pLicFileInfo, long days)
{
	if (pLicFileInfo != NULL)
	{
		long month, day, year, row;
		
		// Extend trial date to the given date
		pp_getvardate(pLicFileInfo->m_lfhandle, VAR_EXP_DATE_HARD, &month, &day, &year);
		
		PP_GETDATE(&month, &day, &year, &row);
		pp_adddays(&month, &day, &year, days);
		
		// store the date and expiration type
		CheckError(pp_setvardate(pLicFileInfo->m_lfhandle, VAR_EXP_DATE_HARD, month, day, year));		
		
		// It is a good idea to update the last used date/time fields
		pp_upddate(pLicFileInfo->m_lfhandle, 0);
		
		return TRUE;
	}
	
	return FALSE;
}

// This function clear the trial date in the input license; the expiration date will be reset 
// to 0
bool CCopyControl::ClearTrialDate(MODULE_LICENSE_INFO * pLicFileInfo)
{
	if ((pp_setvardate(pLicFileInfo->m_lfhandle, VAR_EXP_DATE_HARD, 0, 0, 0)) == PP_SUCCESS)
		return TRUE;
	
	return FALSE;
}

// Set the maintenance expiration date for the input license file.
void	CCopyControl::SetMaintainExpirationDate(MODULE_LICENSE_INFO * pLicFileInfo, long lDate)
{
	// Set maintenance expiration date, if the date is empty, set the date one year from today
	long lYear, lMonth, lDay;
	if (lDate != 0)
	{
		lYear = lDate/10000;
		lMonth = (lDate - lYear * 10000)/100;
		lDay = (lDate - lYear * 10000 - lMonth * 100);
	}
	else
	{
		time_t t = time(NULL);
		struct tm timeinfo;
		time_t rawtime;
		localtime_s(&timeinfo, &rawtime);
		lYear = timeinfo.tm_year + 1901;
		lMonth = timeinfo.tm_mon;
		lDay = timeinfo.tm_mday;
	}
	
	CheckError(PP_SETVARDATE(pLicFileInfo->m_lfhandle, COPY_CTRL_MAINTENANCE_EXPIRATION, 
		lMonth, lDay, lYear));
}

void	CCopyControl::SetMaintainExpirationDate(MODULE_LICENSE_INFO * pLicFileInfo, long lMonth, long lDay, long lYear)
{
	CheckError(PP_SETVARDATE(pLicFileInfo->m_lfhandle, COPY_CTRL_MAINTENANCE_EXPIRATION, 
		lMonth, lDay, lYear));
}

int	CCopyControl::GetMaintenanceExpirationDate(MODULE_LICENSE_INFO * pLicFileInfo)
{
	LONG lMonth, lDay, lYear;
	
	PP_GETVARDATE(pLicFileInfo->m_lfhandle, COPY_CTRL_MAINTENANCE_EXPIRATION, 
		&lMonth, &lDay, &lYear);

	return lYear * 10000 + lMonth * 100 + lDay;
}

int	CCopyControl::getModuleMode(long lModuleCode, MODULE_LICENSE_INFO * fLicenseHandle)
{
	char extype[2];

	if ((fLicenseHandle->MODULE_INFO & lModuleCode) == lModuleCode)
	{
		pp_getvarchar(fLicenseHandle->m_lfhandle, VAR_EXPIRE_TYPE, extype);
		if (extype[0] == 'P')
			return TRIALMODE;
		else
			return FULLMODE;
	}

	return DEMOMODE;
}

string	CCopyControl::getRegCompanyName(MODULE_LICENSE_INFO * fLicenseHandle)
{
	char company[56] = {0};
	
	pp_getvarchar(fLicenseHandle->m_lfhandle, VAR_COMPANY, company);
	
	string strCompany(company);

	return strCompany;
}

bool	CCopyControl::checkLicenseStatus(MODULE_LICENSE_INFO *pLicFileInfo)
{
	long lLicenseID;
	char strPassword[20];

	if (pLicFileInfo != NULL)
	{
		char status[10] = {0};

		pp_getvarnum(pLicFileInfo->m_lfhandle, VAR_LICENSEID, &lLicenseID);
		pp_getvarchar(pLicFileInfo->m_lfhandle, VAR_LICENSEPW, strPassword);

		if (lLicenseID == 0)
		{
			// This is the demo version, return TRUE;
			return true;
		}

		// Check if the Internet connection is available
		long result;

		WSADATA wsadata;
		WSAStartup(0x101, &wsadata);
		BOOL bConnectState = (gethostbyname("www.microsoft.com") != NULL);
		WSACleanup();

		if(bConnectState)
		{
			result = SK_ConnectionState(0, "secure.softwarekey.com", "", "", "", 3);
			char LicenseUpdate[56] = {0};
			
			if (result == SWKERR_NONE)
			{
				// Check license status
				result = SK_GetLicenseStatus("secure.softwarekey.com", 0, "/solo/customers/getlicensestatus.asp", 
					lLicenseID, strPassword, status, 0, LicenseUpdate);
				if (result == 0)
				{
					if (toupper(status[0]) != 'O' && toupper(status[1]) != 'K')
					{
						// This software is not valid
						m_strErrorMessage = "This License is not authorized to run.  Please contact Technical Support for help.", "Licensing Error!";
						ReCreate_License_File(pLicFileInfo);						

						return true;
					}
					else if (toupper(status[0]) == 'O' && toupper(status[1]) == 'K')
					{
						// The software license is still ok; update the maintenance expiration date 
						string strLicenseUpdate = LicenseUpdate;

						if (isExpDateValid(strLicenseUpdate)) {
							strLicenseUpdate= strLicenseUpdate.substr(0,8);
							SetMaintainExpirationDate(pLicFileInfo,  atoi((char *)strLicenseUpdate.c_str()));
						}
					}
					
					return true;
				}
				else
				{
					{
						// This software is not valid
						m_strErrorMessage = "This License is not authorized to run.  Please contact Technical Support for help.";
						ReCreate_License_File(pLicFileInfo);						
						
						return true;
					}
				}
			}
			else
				// The internet connection is not available; return TRUE and let Toolkit continue run
				return true;
		}
		else
		{
			// The internet connection is not available; return TRUE and let Toolkit continue run
			return true;
		}
	}

	return false;
}		

bool CCopyControl::checkProductUpdate(MODULE_LICENSE_INFO *pLicFileInfo,long lAppMajor, long lAppMinor, long lAppRelease)
{
	char action[8];
	char status[8];
	char location[1025];
	char UpdateInfoUrl[1025];
	char LatestVersion[1025];
	char ReleaseDate[1025];
	char UpdateMsg[1024];

	long newLicenseID;
	long result;

	long lLicenseID;
	char strPassword[20];
	PP_GETVARNUM(pLicFileInfo->m_lfhandle, VAR_LICENSEID, &lLicenseID);
	pp_getvarchar(pLicFileInfo->m_lfhandle, VAR_LICENSEPW, strPassword);


	result = SK_UpdateCheck(0, 0, "secure.softwarekey.com", 80, "/solo/customers/updatecheck.asp",
		290824, "RDAT", "en", lAppMajor, lAppMinor, lAppRelease, 0, action, location, 1025, UpdateMsg, 1025, UpdateInfoUrl,
		LatestVersion, ReleaseDate, lLicenseID, strPassword, status, &newLicenseID, 0);

	string strAction = action;

	if(_stricmp(strAction.c_str(), "None") != 0)
	{
		// validate if the latest version string is valid
		string strVer = LatestVersion;
		if (strVer.find('.') != string::npos)
		{
			std::stringstream ss;
			std::string s;
			ss << "RDAT Plus V-";
			ss << LatestVersion;
			ss << " has been released on ";
			ss << ReleaseDate;
			m_strLatestReleaseInformation = ss.str();
			return true;
		}
	}

	return false;
}		

bool CCopyControl::ReCreate_License_File(MODULE_LICENSE_INFO *pLicFileInfo)
{
	pLicFileInfo->m_bInvalidLicenseDetected = true;

	if (fileExist((const char *)pLicFileInfo->m_LicFileName.c_str()))
	{
		pp_lfdelete((char *)(const char *)pLicFileInfo->m_LicFileName.c_str(), LFOPEN_AS32BIT, LF_FILE, "");
		Create_License_File((char *)(const char *)pLicFileInfo->m_LicFileName.c_str(), pLicFileInfo, true);
	}
	else
		Create_License_File((char *)(const char *)pLicFileInfo->m_LicFileName.c_str(), pLicFileInfo, true);
	
	if (pLicFileInfo->m_status == PP_SUCCESS)
		return true;
	return false; 
}

bool CCopyControl::fileExist(const char * filename) 
{
	std::ifstream infile(filename);
	return infile.good();
}

bool CCopyControl::Create_License_File(char * strPath, MODULE_LICENSE_INFO *pLicFileInfo, bool bCreateAliasLicense)
{
	pLicFileInfo->m_status = pp_lfopen(strPath, LF_CREATE_MISSING | LF_CREATE_HIDDEN, LF_FILE, "RDATV20", &(pLicFileInfo->m_lfhandle));
	if (pLicFileInfo->m_status == PP_SUCCESS)
	{
		m_compno = pp_compno(COMPNO_HDSERIAL, "", "C");
		pp_copyadd(pLicFileInfo->m_lfhandle, COPYADD_ERASEALL, m_compno);	
	}

	if (pLicFileInfo->m_status == PP_SUCCESS)
		return true;
	return false; 
}

string	CCopyControl::getLicenseID(MODULE_LICENSE_INFO *pLicFileInfo)
{
	long lLicenseID;
	PP_GETVARNUM(pLicFileInfo->m_lfhandle, VAR_LICENSEID, &lLicenseID);

	std::stringstream ss;
	std::string s;
	ss << lLicenseID;
	ss >> s;

	return s;
}

string	CCopyControl::getLatestReleaseInformation()
{
	return m_strLatestReleaseInformation;
}

bool	CCopyControl::isExpDateValid(string strExpDate)
{
	size_t endpos = strExpDate.find_last_not_of(" \t");
	if (string::npos != endpos)
	{
		strExpDate = strExpDate.substr(0, endpos + 1);
	}

	if (strExpDate.size() >= 8) return true;

	return false;
}