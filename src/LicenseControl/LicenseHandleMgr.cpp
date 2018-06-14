// LicenseHandleMgr.cpp: implementation of the CLicenseHandleMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CopyControl.h"
#include "LicenseHandleMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CCopyControl theCtrl;

CLicenseHandleMgr CLicenseHandleMgr::instance = CLicenseHandleMgr();

/**
 * This class is used to manage instances of License File Handle 
 * objects. It basically consists of a list in which instances can 
 * be stored, so that they can be retrieved based on a handle generated
 * by the manager. The class is not responsible for creating or destroying
 * instances, even though it is important that when a handle is 
 * no longer needed (or in fact destroyed), it is removed from the manager
 * list.
 *
 * Currently the number of distributions has a fixed maximum (MAX_DISTRIBUTIONS)
 */

CLicenseHandleMgr::CLicenseHandleMgr()
{
	for (int i = 0 ; i < MAX_HANDLES ; i++) licenseFileHandles[i] = 0;
}

CLicenseHandleMgr::~CLicenseHandleMgr()
{
	for (int i = 0 ; i < MAX_HANDLES ; i++) {
		delete licenseFileHandles[i];
		licenseFileHandles[i] = 0;
	}
}

/************************************************************************/
/* Remove the input handle from license file handle manager             */
/* Please note that the license file will be be closed within this 
/* function. It must be closed before loading this function to remove
/* it from license handle manager.
/************************************************************************/
int CLicenseHandleMgr::remove(int iHandle)
{
	if ((iHandle<0) || (iHandle>MAX_HANDLES))
		return -1;

	if (mutex.obtain()) {
		// Remove the handle from license file manager
		licenseFileHandles[iHandle] = 0;
		mutex.release();
	}

	return 1;
}

/************************************************************************/
/* Get the handle for the input license file; if the license file has   */
/* been opened before; return the handle directly                       */
/************************************************************************/
int CLicenseHandleMgr::getLicenseFileHandle(string strLicenseFilePath)
{
	int iRetval = 0;

	// First search through the license handle manager to check if the 
	// license file has been opened already
	for (int i = 0 ; i < MAX_HANDLES ; i++) {
		if (licenseFileHandles[i] != 0) {
			if (strLicenseFilePath.compare(licenseFileHandles[i]->m_LicFileName) == 0) {
				// This license file has already been opened; return the handle directly
				return i;
			}
		}
	}

	if (mutex.obtain()) {
		int i;
		for (i = 0 ; i < MAX_HANDLES && licenseFileHandles[i] != 0; i++) {}
		
		if (i == MAX_HANDLES) {
			iRetval = -1;
		} else {
			// Create a new license file handle
			licenseFileHandles[i] = new MODULE_LICENSE_INFO;
			licenseFileHandles[i]->m_bNewLicense = false;
			licenseFileHandles[i]->m_bInvalidLicenseDetected = false;
			licenseFileHandles[i]->m_bDemoMode = TRUE;
			licenseFileHandles[i]->LIGHT_INFO = 0;
			licenseFileHandles[i]->m_lfhandle   = 0;
			licenseFileHandles[i]->m_semhandle  = 0;
			licenseFileHandles[i]->m_LicFileName = strLicenseFilePath;
			licenseFileHandles[i]->m_SemName = "";
			licenseFileHandles[i]->m_sModuleName = "";
			licenseFileHandles[i]->m_network_to_local = FALSE;
			licenseFileHandles[i]->m_TimeDepLic = FALSE;

			// Open the license file
			theCtrl.Open_License_File(licenseFileHandles[i]);

			iRetval = i;
		}
		mutex.release();
	}

	return iRetval;
}

/************************************************************************/
/* Get the license file handle based on input handle index              */
/************************************************************************/
MODULE_LICENSE_INFO * CLicenseHandleMgr::getHandle(int iHandle)
{
	if ((iHandle <0) || (iHandle>=MAX_HANDLES)) {
		return NULL;
	}

	return licenseFileHandles[iHandle];
}