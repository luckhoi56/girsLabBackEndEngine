//
// $Id: LicenseController.h 152 2005-01-24 13:04:33Z saulo $
//

#if !defined(AFX_LICENSECONTROLLER_H__D8948372_0504_483F_A2CE_98FE91500112__INCLUDED_)
#define AFX_LICENSECONTROLLER_H__D8948372_0504_483F_A2CE_98FE91500112__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RegisterWIN32.h"

class LicenseController  
{
public:
	LicenseController();
	virtual ~LicenseController();
	void ConstructSystemID(char * buf, int len) const;
	BOOL GetCDriveSerialNumber(LPDWORD pdwSerialNumber) const;
	BOOL GetNicAddress(LPSTR pszNicAddress, UINT nBufSize) const;

	bool storeStartTime(__int64 time);
	__int64 retrieveStartTime(void);

	bool storeLicenseCode(const char * config, const char * code);
	void retrieveLicenseCode(const char * config, char * code, int len);

private:

	CRegisterWIN32 registry;

};

#endif // !defined(AFX_LICENSECONTROLLER_H__D8948372_0504_483F_A2CE_98FE91500112__INCLUDED_)
