/*****************************************************************************
*
*    Copyright (c) 2010 Concept Software, Inc.  All rights reserved.
*
****************************************************************************/

#ifndef SWKCL_H
#define SWKCL_H



#ifndef INTERNET_VERSION

/* error codes */
#define SWKERR_NONE									0
#define SWKERR_WINSOCK_STARTUP_ERROR				1
#define SWKERR_WINSOCK_CANNOT_RESOLVE_HOST			2
#define SWKERR_WINSOCK_CANNOT_CREATE_SOCKET			3
#define SWKERR_WINSOCK_CANNOT_CONNECT_TO_SERVER		4
#define SWKERR_WINSOCK_CANNOT_SEND_DATA				5
#define SWKERR_WINSOCK_CANNOT_READ_DATA				6
#define SWKERR_NO_MORE_SOFTWARE_KEYS_AVAILABLE		7
#define SWKERR_INVALID_SERVER_RESPONSE				8
#define SWKERR_CANNOT_ALLOCATE_MEMORY				9
#define SWKERR_WINSOCK_CANNOT_RESOLVE_PROXY			10
#define SWKERR_WININET_UNAVAILABLE					11
#define SWKERR_WININET_FUNCTION_UNAVAILABLE			12
#define SWKERR_NO_CONNECTION						13
#define SWKERR_INTERNAL_ERROR						14 
#define SWKERR_WINSOCK_CONNECT_ERROR				15
#define SWKERR_WINSOCK_BUFFER_OVERFLOW				16
#define SWKERR_PARTIAL_CONNECTION					17
#define SWKERR_INVALID_PROXY_LOGIN					18
#define SWKERR_SERVER_DOWN							19
#define SWKERR_FILE_ERROR							20
#define SWKERR_FTP_FILENOTFOUND						21
#define SWKERR_CANCEL								22
#define SWKERR_ERROR_CREATING_WINDOW				23
#define SWKERR_COULD_NOT_CREATE_FILE				24
#define SWKERR_ITEMNOTFOUND							25
#define SWKERR_INVALIDPASSWORD						26
#define SWKERR_UPLOADFAILED							27
#define SWKERR_CONNECTION_TIMEOUT					28
#define SWKERR_OS_INVALID							29
#define SWKERR_INVALID_PARAMETERS					30
#define SWKERR_HTTP_FILENOTFOUND					31

#define SWKERR_ENCRYPTION_FAILED                    32
#define SWKERR_SIGNING_FAILED                       33
#define SWKERR_DECRYPTION_FAILED                    34
#define SWKERR_VERIFICATION_FAILED                  35
#define SWKERR_CANONICALIZATION_FAILED              36
#define SWKERR_WEBSERVICE_RETURNED_FAILURE          37
#define SWKERR_LOG_ENTRYNOTFOUND                    38
#define SWKERR_LOG_CANNOT_ERASE                     39
#define SWKERR_LOG_CANNOT_OPEN                      40

#else

#define SWKERR_NONE									0
#define SWKERR_WINSOCK_STARTUP_ERROR				ERR_WINSOCK_STARTUP_ERROR
#define SWKERR_WINSOCK_CANNOT_RESOLVE_HOST			ERR_WINSOCK_CANNOT_RESOLVE_HOST
#define SWKERR_WINSOCK_CANNOT_CREATE_SOCKET			ERR_WINSOCK_CANNOT_CREATE_SOCKET
#define SWKERR_WINSOCK_CANNOT_CONNECT_TO_SERVER		ERR_WINSOCK_CANNOT_CONNECT_TO_SERVER
#define SWKERR_WINSOCK_CANNOT_SEND_DATA				ERR_WINSOCK_CANNOT_SEND_DATA
#define SWKERR_WINSOCK_CANNOT_READ_DATA				ERR_WINSOCK_CANNOT_READ_DATA
#define SWKERR_NO_MORE_SOFTWARE_KEYS_AVAILABLE		ERR_NO_MORE_SOFTWARE_KEYS_AVAILABLE
#define SWKERR_INVALID_SERVER_RESPONSE				ERR_INVALID_SERVER_RESPONSE
#define SWKERR_CANNOT_ALLOCATE_MEMORY				ERR_CANNOT_ALLOCATE_MEMORY
#define SWKERR_WINSOCK_CANNOT_RESOLVE_PROXY			ERR_WINSOCK_CANNOT_RESOLVE_PROXY
#define SWKERR_WININET_UNAVAILABLE					11
#define SWKERR_WININET_FUNCTION_UNAVAILABLE			12
#define SWKERR_NO_CONNECTION						13
#define SWKERR_INTERNAL_ERROR						14 
#define SWKERR_WINSOCK_CONNECT_ERROR				15
#define SWKERR_WINSOCK_BUFFER_OVERFLOW				16
#define SWKERR_PARTIAL_CONNECTION					17
#define SWKERR_INVALID_PROXY_LOGIN					18
#define SWKERR_SERVER_DOWN							19
#define SWKERR_FILE_ERROR							20
#define SWKERR_FTP_FILENOTFOUND						21
#define SWKERR_CANCEL								22
#define SWKERR_ERROR_CREATING_WINDOW				23
#define SWKERR_COULD_NOT_CREATE_FILE				24
#define SWKERR_ITEMNOTFOUND							25
#define SWKERR_INVALIDPASSWORD						26
#define SWKERR_UPLOADFAILED							27
#define SWKERR_CONNECTION_TIMEOUT					28
#define SWKERR_OS_INVALID							29
#define SWKERR_INVALID_PARAMETERS					30
#define SWKERR_HTTP_FILENOTFOUND					31

#define SWKERR_ENCRYPTION_FAILED                    32
#define SWKERR_SIGNING_FAILED                       33
#define SWKERR_DECRYPTION_FAILED                    34
#define SWKERR_VERIFICATION_FAILED                  35
#define SWKERR_CANONICALIZATION_FAILED              36
#define SWKERR_WEBSERVICE_RETURNED_FAILURE          37
#define SWKERR_LOG_ ENTRYNOTFOUND                   38
#define SWKERR_LOG_CANNOT_ERASE                     39
#define SWKERR_LOG_CANNOT_OPEN                      40

#endif



/* universal flags */
#define NO_FLAGS                                    0
#define SK_SECURE_CONNECTION						32768
#define	SK_SOCKETS_ONLY								65536



/* SK_ConnectionState() flags */
#define SK_FULL_CONNECTIVITY						1
#define SK_WININET_CONNECTIVITY_ONLY				2
#define SK_CONNECTIVITY_QUICKCHECK					4
#define SK_CONNECTIVITY_FULLCHECK					8
#define SK_GET_PROXY								16
#define SK_PROXY_CONNECTION_TEST					32



/* SK_PostRegData() flags */
#define SK_NO_OVERWRITE								0x00
#define SK_OVERWRITE								0x01
#define SK_OPTIN1									0x02
#define SK_OPTIN2									0x04



/* SK_ProcEZTrig1() and SK_GetTCDataDlg() flags */
#define SK_HIDE_PROXY_BUTTON						1



/* file transfer flags and options */
#define SK_FILEOPT_HTTP								1
#define SK_FILEOPT_FTP								2

#define SK_FILEFLAG_VALIDATERETRY					0x01
#define SK_FILEFLAG_NOIDEDIT						0x02
#define SK_FILEFLAG_NOPASSEDIT						0x04

#define SK_STATUS_FILESIZE							1
#define SK_STATUS_RECEIVING							2
#define SK_STATUS_ERROR								3
#define SK_CALLBACK_CANCEL							4
#define SK_STATUS_FINISHED							5
#define SK_STATUS_SENDING							6



/* SK_GetHttp() flags */
#define SK_STRIP_HEADER								1



/* SK_Firewall() flags */
#define SK_RETURN_OS_FAILURE						0x01



/* SK_Firewall() protocol types */
#define FW_PROTOCOL_TCP								0
#define FW_PROTOCOL_UDP								1



/* SK_Firewall action */
#define SK_ADD_APP									0
#define SK_ADD_PORT									1


/* SK_UpdateCheck() flags */
#define SK_VERIFYDAYS								0x01



/* XmlActivationService flags */
#define SK_WEBSERVICES_FLAGS_USE_ENCRYPTION         0x00000001
#define SK_WEBSERVICES_FLAGS_USE_SIGNATURE          0x00000002
#define SK_WEBSERVICES_FLAGS_CONVERTTOLOCALTIME     0x00000004



/* SKint_AsyncSocketProccessCommand() flags */
#define SK_SOCKET_FLAG_CONNECTIONONLY				1



/* windows defines */
#ifndef PPPEXPORT
    #define PPPEXPORT
#endif

#ifndef _WINDOWS
    #ifndef LONG
        #define LONG            long 
    #endif

    #ifndef LPLONG
        #define LPLONG          LONG *
    #endif

    #ifndef CHAR
        #define CHAR            char
    #endif

    #ifndef LPSTR
        #define LPSTR           CHAR *
    #endif

    #ifndef VOID
        #define VOID            void
    #endif

    #ifndef _WIN64
        #ifndef HINSTANCE
            #define HINSTANCE   LONG
        #endif
    #endif

    #ifndef HANDLE
        #define HANDLE          VOID *
    #endif

    #ifndef HGLOBAL
        #define HGLOBAL         HANDLE
    #endif
#endif

#ifndef PPLFHANDLE
    #ifdef _WIN64
        #define PPLFHANDLE      HGLOBAL
    #else
        #define PPLFHANDLE      LONG
    #endif
#endif

#ifndef PPHWND
    #ifdef _WIN64
        #define PPHWND          HWND
    #else
        #define PPHWND          LONG
    #endif
#endif

#ifndef LPPPLFHANDLE
    #ifdef _WIN64
        #define LPPPLFHANDLE    HGLOBAL *
    #else
        #define LPPPLFHANDLE    LPLONG
    #endif
#endif



#ifdef __cplusplus
extern "C" {
#endif



extern HINSTANCE hSKCAInstance;



/* function prototypes */
LONG PPPEXPORT WINAPI SK_FileDownload(PPHWND hWnd, LONG flags, LONG options, LPSTR server, INT port, LPSTR url, LPSTR savepath, LPSTR id, LPSTR password, LONG filesize, LPCSTR dlgText, INT CALLBACK func(LONG, LONG, LONG));
LONG PPPEXPORT WINAPI SK_FileUpload(PPHWND hwnd, LONG flags, LONG options, LPCSTR server, INT port, LPCSTR url, LPCSTR file, LPCSTR id, LPCSTR password, LPCSTR paramlist, INT CALLBACK func(LONG, LONG, LONG));
LONG PPPEXPORT WINAPI SK_Firewall(LONG action, LONG flags, LPSTR appname, LPSTR filename, LONG port, LONG protocol);
VOID PPPEXPORT WINAPI SK_GetErrorStr( LONG number, LPSTR buffer );
LONG PPPEXPORT WINAPI SK_GetHttp(LONG flags, LPSTR server, INT port, LPSTR url, LONG licenseid, LPSTR password, LPSTR* buffer, LONG buffersize, LONG bufferstart);
LONG PPPEXPORT WINAPI SK_GetLicenseStatus( LPSTR server, LPSTR proxy, LPSTR url, LONG licenseid, LPSTR pw, LPSTR status, LPLONG replacedby, LPSTR licenseupdate );
LONG PPPEXPORT WINAPI SK_GetLicenseStatusEx( LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LONG licenseid, LPSTR pw, LPSTR status, LPLONG replacedby, LPSTR licenseupdate );
LONG PPPEXPORT WINAPI SK_GetRegData(  LPSTR server, LPSTR proxy, LPSTR url, LONG licenseid, LPSTR pw, LPSTR companyname, LPSTR contactname, LPSTR email, LPSTR phone, LPSTR ud1, LPSTR ud2, LPSTR ud3, LPSTR ud4, LPSTR ud5 );
LONG PPPEXPORT WINAPI SK_GetRegDataEx( LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LONG licenseid, LPSTR pw, LPSTR companyname, LPSTR contactname, LPSTR email, LPSTR phone, LPSTR ud1, LPSTR ud2, LPSTR ud3, LPSTR ud4, LPSTR ud5 );
LONG PPPEXPORT WINAPI SK_GetTCData( LPSTR server, LPSTR proxy, LPSTR url, LONG licenseid, LPSTR pw, LONG cenum, LONG compno, LPLONG result1, LPLONG result2, LPSTR licenseupd );
LONG PPPEXPORT WINAPI SK_GetTCDataEx( LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LONG licenseid, LPSTR pw, LONG cenum, LONG compno, LPLONG result1, LPLONG result2, LPSTR licenseupd );
LONG PPPEXPORT WINAPI SK_GetTCDataDlg( PPHWND hwnd, PPLFHANDLE lfhandle, LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LONG usercode1, LONG usercode2, LPLONG regkey1, LPLONG regkey2, LPLONG licenseid, LPSTR password, LPSTR licenseupd );
VOID WINAPI SK_InitLib( HINSTANCE hInstance );
VOID PPPEXPORT WINAPI SK_LibVersion(LPLONG ver1, LPLONG ver2, LPLONG ver3, LPLONG ver4);
LONG PPPEXPORT WINAPI SK_Parse(LONG flags, LPSTR data, LONG item, LPSTR delimiter,  LPSTR value);
LONG PPPEXPORT WINAPI SK_PostCounters( LPSTR server, LPSTR proxy, LPSTR url, LONG licenseID, LONG counter1, LONG counter2, LONG counter3, LONG counter4, LONG counter5, LPSTR licenseupd );
LONG PPPEXPORT WINAPI SK_PostCounterEx( LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LONG LicenseID, LONG counter1, LONG counter2, LONG counter3, LONG counter4, LONG counter5, LPSTR newlicenseinfo );
LONG PPPEXPORT WINAPI SK_PostEvalData( LPSTR server, LPSTR proxy, LPSTR url, LPSTR firstname, LPSTR lastname, LPSTR email, LPSTR phone, LPSTR ud1, LPSTR ud2, LPSTR ud3, LPSTR ud4, LPSTR ud5, LPLONG regid );
LONG PPPEXPORT WINAPI SK_PostEvalDataEx( LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LPSTR firstname, LPSTR lastname, LPSTR email, LPSTR phone, LPSTR ud1, LPSTR ud2, LPSTR ud3, LPSTR ud4, LPSTR ud5, LPLONG regid );
LONG PPPEXPORT WINAPI SK_ProcEZTrig1( PPHWND hwnd, LPSTR filename, LPSTR password, LONG flags, LPLONG errorcode );
LONG PPPEXPORT WINAPI SK_PostStrings( LPSTR server, LPSTR proxy, LPSTR url, LONG LicenseID, LPSTR string1, LPSTR string2, LPSTR string3, LPSTR string4, LPSTR string5, LPSTR newlicenseinfo );
LONG PPPEXPORT WINAPI SK_PostStringEx( LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LONG LicenseID, LPSTR string1, LPSTR string2, LPSTR string3, LPSTR string4, LPSTR string5, LPSTR newlicenseinfo );
LONG PPPEXPORT WINAPI SK_ConnectionState(LONG flags, LPSTR server, LPSTR proxy, LPSTR proxyuser, LPSTR proxypw, LONG timeout);
LONG PPPEXPORT WINAPI SK_PostRegData( LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LONG LicenseID, LPSTR password, LPSTR companyname, LPSTR firstname, LPSTR lastname, LPSTR address1, LPSTR address2, LPSTR city, LPSTR state, LPSTR postalcode, LPSTR country, LPSTR phone, LPSTR fax, LPSTR email );
LONG PPPEXPORT WINAPI SK_UpdateCheck(LONG flags, LONG options, LPSTR server, INT port, LPSTR url, LONG productid, LPSTR prodname, LPSTR languagecode,
							    LONG ver1, LONG ver2, LONG ver3, LONG ver4, LPSTR action, LPSTR data, 
								LONG datasize, LPSTR message, LONG msgsize, LPSTR urlinfo, LPSTR latestversion, LPSTR releasedate, 
								LONG licenseid, LPSTR password, LPSTR status, LPLONG replacedby, LPSTR licenseupdate);
LONG PPPEXPORT WINAPI SK_EncodeData(LONG flags, LPCSTR in_data, LONG datalen, LPSTR out_buffer, LONG buffer_size);
LONG PPPEXPORT WINAPI SK_DecodeData(LONG flags, LPCSTR in_data, LPLONG datalen, LPSTR out_buffer, LONG buffer_size);
LONG PPPEXPORT WINAPI SK_CallWebService(LONG flags, LPSTR server, LPSTR proxy, LPSTR url, LPSTR request, LPSTR responseBuffer, LPLONG size);
LONG PPPEXPORT WINAPI SK_PrepareWebServiceRequest(LONG flags, LPSTR clientKey, LPSTR serverKey, LPSTR originalRequest, LPSTR requestBuffer, LPLONG size);
LONG PPPEXPORT WINAPI SK_PrepareWebServiceResponse(LONG flags, LPSTR clientKey, LPSTR serverKey, LPSTR originalResponse, LPSTR responseBuffer, LPLONG size);
LONG PPPEXPORT WINAPI SK_ActivateInstallation(LONG flags, LPSTR server, LPSTR proxy, LPSTR url,
                                LPSTR encryptionKeyId, LPSTR clientKey, LPSTR serverKey, LONG licenseId,
                                LPSTR password, LPSTR serialNumber, LPSTR licenseKey, LPSTR productVersion,
                                LONG productId, LONG productOptionId, LONG userCode1, LONG userCode2,
                                LONG requireRegistration, LPSTR installationId, LPSTR installationName, LPSTR systemIdentifier,
                                LPSTR reserved1, LPLONG reserved2, LONG reserved3,
                                /*return output arguments below*/
                                LPLONG resultCode,
                                LPSTR errorMessage,LPLONG errorMessageSize, LPSTR newInstallationId,LPLONG newInstallationIdSize,
                                LPLONG activationCode1, LPLONG activationCode2, LPSTR licenseUpdate, LPLONG licenseUpdateSize,
                                LPLONG activationsLeft, LPLONG minVersionLong, LPSTR minVersionStr, LPLONG minVersionStrSize,
                                LPSTR minVersionUrl, LPLONG minVersionUrlSize, LPSTR serverTimestamp, LPLONG serverTimestampSize,
                                LPSTR reserved4, LPLONG reserved5); 
LONG PPPEXPORT WINAPI SK_CheckInstallationStatus(LONG flags, LPSTR server, LPSTR proxy, LPSTR url,
                                LPSTR encryptionKeyId, LPSTR clientKey,
                                LPSTR serverKey, LPSTR installationId, LPSTR productVersion,
                                LPSTR systemIdentifier, LPSTR logPath, LPSTR reserved1, LPLONG reserved2,
                                LONG reserved3,
                                /*return output arguments below*/
                                LPLONG resultCode,
                                LPSTR errorMessage,LPLONG errorMessageSize,
                                LPSTR deactivatedDate, LPLONG deactivatedDateSize,
                                LPSTR licenseStatus, LPLONG licenseStatusSize,
                                LPSTR licenseUpdate, LPLONG licenseUpdateSize,
                                LPSTR expirationDate, LPLONG expirationDateSize,
                                LPSTR serverTimestamp, LPLONG serverTimestampSize,
                                LPSTR reserved4, LPLONG reserved5);
LONG PPPEXPORT WINAPI SK_DeactivateInstallation (LONG flags, LPSTR server, LPSTR proxy, LPSTR url,
                                LPSTR encryptionKeyId, LPSTR clientKey,
                                LPSTR serverKey, LPSTR installationId,
                                LPSTR systemIdentifier, LPSTR reserved1, LPLONG reserved2,
                                LONG reserved3,
                                /*return output arguments below*/
                                LPLONG resultCode,
                                LPSTR errorMessage,LPLONG errorMessageSize,
                                LPSTR deactivatedDate, LPLONG deactivatedDateSize, 
                                LPSTR serverTimestamp, LPLONG serverTimestampSize,
                                LPSTR reserved4, LPLONG reserved5);

VOID PPPEXPORT WINAPI SK_GenerateLogEntry(LONG flags, 
                                LPSTR logEntry, LPLONG logEntrySize, 
                                LPSTR logSessionID, LONG logSessionIDSize, 
                                LPSTR szEvent, LONG eventSize, 
                                LPSTR eventData, LONG eventDataSize);

VOID PPPEXPORT WINAPI SK_WriteLogEntry(LONG flags, LPSTR logPath, LPSTR logEntry, LONG logEntrySize);

LONG PPPEXPORT WINAPI SK_UpdateLogEntry(LONG flags, LPSTR logPath, LPSTR logSessionID, LPSTR szEvent);



#ifdef __cplusplus
}
#endif



#endif
