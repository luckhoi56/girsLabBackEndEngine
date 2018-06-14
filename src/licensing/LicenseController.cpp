//
// $Id: LicenseController.cpp 152 2005-01-24 13:04:33Z saulo $
//

#include "stdafx.h"
#include "LicenseController.h"

#define REGISTRY_PATH "software\\Prediction Technologies\\ReDAT\\License"
#define REGISTRY_TIME_KEY_0 "checksum0"
#define REGISTRY_TIME_KEY_1 "checksum1"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LicenseController::LicenseController()
{

}

LicenseController::~LicenseController()
{

}

/**
 * Stores the license code for the specified configuration.
 */
bool LicenseController::storeLicenseCode(const char * config, const char * code)
{

	if (config != 0 && code != 0) {
		if (registry.Open(REGISTRY_PATH,FALSE)) {
			registry.Write(config,code);
			registry.Close();
			return true;
		}
	} else if (config != 0) {
		if (registry.Open(REGISTRY_PATH,FALSE)) {
			registry.DeleteValue(config);
			registry.Close();
			return true;
		}
	}
	return false;
}


/**
 * Retrieves the license code for the specified configuration.
 */
void LicenseController::retrieveLicenseCode(const char * config, char * code, int len)
{
	char * code_ptr = 0;

	if (code != 0) code[0] = 0;

	if (config != 0 && code != 0) {
		if (registry.Open(REGISTRY_PATH,TRUE)) {
			registry.Read(config,code_ptr);
			if (code_ptr) {
				strncpy(code,code_ptr,len);
			}
			delete [] code_ptr;
			registry.Close();
		}
	}
}

/**
 * Stores the time code.
 */
bool LicenseController::storeStartTime(__int64 time)
{
	__int32 * int_arr = (__int32*)&time;

	if (registry.Open(REGISTRY_PATH,FALSE)) {
		registry.Write(REGISTRY_TIME_KEY_0,int_arr[0]);
		registry.Write(REGISTRY_TIME_KEY_1,int_arr[1]);
		registry.Close();
		return true;
	}
	return false;
}

/**
 * Retrieves the time code.
 */
__int64 LicenseController::retrieveStartTime()
{
	__int64 time = 0;
	int * int_arr = (int*)&time;
	if (registry.Open(REGISTRY_PATH,TRUE)) {
		registry.Read(REGISTRY_TIME_KEY_0,int_arr[0]);
		registry.Read(REGISTRY_TIME_KEY_1,int_arr[1]);
		registry.Close();
	}
	return time;
}

/**
 * Constructs a character array unique to the computer.
 * Current implementation constructs an 8 character null-terminated
 * char array. Length of the buffer must be at least 9.
 */
void LicenseController::ConstructSystemID(char * buf, int len) const {

	if (len < 9) {
		buf[0] = 0;
		return;
	}

	char codes [] = {'a','b','c','d','e','f','g','h','i','j','k','m','o','p','q','r'};
	DWORD hdserial;
	BOOL hd_result = GetCDriveSerialNumber(&hdserial);
	hdserial ^= 20498345;
	for (int i = 0 ; i < 9 ; i++) {
		int code = hdserial & 15;
		buf[i] = codes[code];
		hdserial /= 16;
	}
	buf[8] = 0;
}


BOOL LicenseController::GetNicAddress(LPSTR pszNicAddress, UINT nBufSize) const
{
  BOOL bSuccess = FALSE;

/*
  //NIC address is 12 character string
  if (nBufSize < 13)  
    return FALSE;

  //the way we determine the NIC address is to call the RPC DCE function
  //UuidCreate. The standard format of the GUID returned contains
  //the NIC address in the last 12 characters. The added advantage to
  //this method is that we do not need to rely on a specific network
  //protocol needing to be installed on the client machine to determine
  //the NIC address. You could use this function as the basis for a
  //security scheme for a networked product of yours. Using the NIC address
  //is a guranteed way of uniquely identify a machine throughout the network.

  UUID Uuid;
  RPC_STATUS rpcStatus = UuidCreate(&Uuid);
  if (rpcStatus == RPC_S_OK)
  {
    unsigned char* pszGuid;  
    rpcStatus = UuidToString(&Uuid, &pszGuid);
    if (rpcStatus == RPC_S_OK)
    {
      char* pLastMinus = strrchr((char*)pszGuid, '-');
      if (pLastMinus)
      {
        strcpy(pszNicAddress, pLastMinus+1);
        bSuccess = TRUE;
      }

      //need to free created buffer
      RpcStringFree(&pszGuid);
    }
    else 
      TRACE("Error calling UuidToString, Error value is %d\n", rpcStatus);
  }
  else
    TRACE("Error calling UuidCreate, Error value is %d\n", rpcStatus);
*/
  return bSuccess;
}


BOOL LicenseController::GetCDriveSerialNumber(LPDWORD pdwSerialNumber) const
{
  //Again as with the NIC address, the serial number of the C drive
  //could be used as the basis of a security scheme for a networked 
  //product of yours
  BOOL bSuccess = (GetVolumeInformation("C:\\", NULL, 0, pdwSerialNumber,
                                        NULL, NULL, NULL, 0) != 0);
  return bSuccess;
}