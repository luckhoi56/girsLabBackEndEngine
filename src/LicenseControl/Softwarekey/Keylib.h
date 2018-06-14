/*****************************************************************************
*
*    Copyright (c) 2010 Concept Software, Inc.  All rights reserved.
*
****************************************************************************/

#ifndef KEYLIB_H
#define KEYLIB_H



#ifdef __cplusplus     /* Assume C declarations for C++ */
extern "C" {
#endif  /* __cplusplus */



/* computer number defines */
#define COMPNO_BIOS                 0x0001 /* no longer supported - retained only for backwards compatibility */
#define COMPNO_HDSERIAL             0x0002
#define COMPNO_HDLOCK               0x0004
#define COMPNO_HDTYPE               0x0008
#define COMPNO_NETNAME              0x0010
#define COMPNO_MACADDR              0x0020
#define COMPNO_FILESTART            0x0040
#define COMPNO_WINPRODID            0x0080
#define COMPNO_IPADDR               0x0100
#define COMPNO_MACADDR_WIN98	    0x0200
#define COMPNO_BIOS16			    0x0400 
#define COMPNO_BIOS32			    0x0800
#define COMPNO_MACADDR_WINNB        0x1000
#define COMPNO_BIOS64			    0x2000	
#define COMPNO_MACADDR_4200         0x4000
#define SUPPRESS_ERRORS			    0x8000  /* to suppress the MessageBoxes relating to Machnm1.exe */
#define COMPNO_ENHANCED			    0x10000	/* use new compid algs */
#define COMPNO_SERVER_MACADDR       0x20000
#define COMPNO_NO_WMI			    0x40000 /* used with COMPNO_ENHANCED, COPYCHK_ENHANCED, and COPYADD_ENHANCED to omit WMI initialization for use in a DLL */

#define PP_STD_THRESHOLD            20

/* pp_compno32drivers driver */
#define COMPNO32_BIOS			    0x0001
#define COMPNO64_BIOS			    0x0002


    
/* pp_compno32drivers() flags */
#define CHECK_DRIVER_STATUS		    0
#define INSTALL_DRIVER			    1
#define UNINSTALL_DRIVER		    2



/* pp_copycheck() action codes */
#define ACTION_MANUAL               0x0000
#define ACTION_AUTOADD              0x0001
#define COPYCHK_STRICT              0x0002
#define COPYCHK_ENHANCED		    0x0004



/* pp_copyadd() flags */
#define COPYADD_ADDDUPE             0x0001
#define COPYADD_ERASEALL            0x0002
#define COPYADD_ENHANCED		    0x0004



/* pp_copydelete() flags */
#define COPYDEL_ALL				    -1
#define COPYDEL_ENHANCED		    -2



/* pp_copyget() flags (slot) */
#define COPYGET_ENHANCED		    -1



/* pp_transfer() flags */
#define PP_TRANSFER_ENHANCED	    -1



/* license file type defines */
#define LF_FILE                     1
#define LF_REGISTRY                 2
#define LF_CRYPTO_BOX_VERSA         3
#define LF_CRYPTO_BOX_560           4
#define LF_INTERNET                 5
#define LF_FOLDER                   16384 /* Used in pp_lfpermset to set the permissions of folders */
#define LF_FOLDER_INHERIT           32768 /* Used in pp_lfpermset to set the permissions of folders and all sub containers and objects */



/* pp_lfopen() flags */
#define LF_CREATE_NORMAL            0x0000
#define LF_CREATE_RDONLY            0x0001
#define LF_CREATE_HIDDEN            0x0002
#define LF_CREATE_SYSTEM            0x0004
#define LF_CREATE_MISSING           0x0008
#define LFOPEN_NOLOOP               0x0010
#define LFOPEN_NOCACHE			    0x0020 /* no longer supported - retained only for backwards compatibility */
#define LFOPEN_OPENALWAYS   	    0x0040 /* used in pp_semopen to create file if not there */
#define LFOPEN_AS32BIT			    0x0080 /* used to force 64bit app to use 32bit locations to share license files */
#define LFOPEN_KEEPINMEMORY         0x0100
#define LFOPEN_READONLY				0x0200

	

/* pp_lfcreate() flags */
#define LFCREATE_OVERWRITE          0x0001



/* pp_semopen() flags */
#define SEM_FILE                    0
#define SEM_TCPIP                   1



/* pp_getdateex() and pp_gettimeex() flags */
#define PP_USE_UTC					1



/* pp_upddate() flags */
#define UPDDATE_FORCE               1
#define UPDDATE_NOTIME              2



/* pp_lfalias() flags */
#define LFALIAS_VERIFY_CHECKSUM     1
#define LFALIAS_FIND_RECENT         2
#define LFALIAS_OVERWRITE_OLDER     4
#define LFALIAS_CREATE_MISSING      8
#define LFALIAS_SCATTER				16
#define LFALIAS_SCATTER_BACK        0x80000000	/* high bit */
#define LFALIAS_NOCACHE				32 /* no longer supported - retained only for backwards compatibility */



/* pp_eztrig1dlg() flags */
#define EZTRIG1DLG_PROCESS          1
#define EZTRIG1DLG_DELAYED          2



/* return codes for pp_eztrial1() */
#define EZTRIAL1_ERROR              0
#define EZTRIAL1_RETAIL             1
#define EZTRIAL1_DEMO               2
#define EZTRIAL1_BINDING_FAILED     3
#define EZTRIAL1_EXPIRED            4
#define EZTRIAL1_CLOCKTURNEDBACK    5
#define EZTRIAL1_PERIODIC           6
#define EZTRIAL1_EXCEEDED_USERS     7



/* pp_eztrial1ex() flags */
#define EZTRIAL1EX_NO_NET			0x0001



/* pp_exportfile() and pp_importactfile() flags */
#define PPP_NOURLENCODE			    0x0001



/* pp_importactfile() flags */
#define PP_EZTRIGPROCESS            1
#define PPP_EZTRIGPROCESS		    1
#define PP_IPPROCESS                2



/* pp_netopen() flags */
#define NET_SINGLE		            0x0000
#define NET_DO_NOT_UPDATE	        0x0001
#define NET_ALWAYS_CONNECTED        0x0002



/* pp_sysinfo() flags */
#define PP_TERMSERV		            0x0001
#define PP_VMWARE		            0x0002
#define PP_VIRTUALPC	            0x0004
#define PP_64BIT		            0x0008
#define PP_VMLDTCHECK               0x0010
#define PP_HYPERV                   0x0020
#define SYSINFO_NO_WMI              0x0040
#define PP_VIRTUALBOX               0x0080
#define PP_VMWARE2					0X0100
#define PP_VIRTUALPC2	            0x0200



/* pp_gotourl() flags */
#define GOTOURL_BUYNOW				1
#define GOTOURL_USE_CREATEPROCESS	2



/* miscellaneous defines */
#define NO_FLAGS				    0
#define TCODE_MAX                   50
#define PP_MAX_UDEF_STRINGS         10
#define PP_MAX_UDEF_NUMBERS         5
#define PP_MAX_UDEF_DATES           5		



/* result codes */
#define PP_FAILURE                  0
#define PP_FALSE                    0
#define PP_SUCCESS                  1
#define PP_TRUE                     1



/* error codes */
#define ERR_INVALID_ATTRIBUTES                  2
#define ERR_CANNOT_CHANGE_ATTRIBS               3 /* obsolete as of 4.6.0.5 */
#define ERR_HFILE_ERROR                         4
#define ERR_CANNOT_WRITE_FILE                   5
#define ERR_CANNOT_CLOSE_FILE                   6
#define ERR_CANNOT_OPEN_FILE                    7
#define ERR_CANNOT_READ_FILE                    8
#define ERR_CANNOT_CREATE_FILE                  9
#define ERR_CANNOT_DELETE_FILE                  10
#define ERR_FILE_WAS_CREATED                    11
#define ERR_INVALID_PASSWORD                    12
#define ERR_WRONG_PASSWORD                      13
#define ERR_INCORRECT_PARAMETERS                14
#define ERR_FILE_MISSING                        15
#define ERR_MEMORY_ALLOCATION                   16
#define ERR_MEMORY_FREE                         17
#define ERR_INVALID_LFHANDLE                    18
#define ERR_MEMORY_LOCK                         18
#define ERR_SLOT_NUM_INVALID                    19
#define ERR_SLOT_EMPTY                          20
#define ERR_SLOTS_FULL                          21
#define ERR_SLOT_ALREADY_ASSIGNED               22
#define ERR_NET_LIC_FULL                        23
#define ERR_COMPNO_NOT_FOUND                    24
#define ERR_VAR_NO_INVALID                      25
#define ERR_SOFT_EXPIRATION                     26
#define ERR_EXPTYPE_INVALID                     27
#define ERR_EXP_DATE_EMPTY                      28
#define ERR_STRING_TOO_LONG                     29
#define ERR_CURRENT_DATE_OLDER                  30
#define ERR_CANNOT_LOCK_FILE                    31
#define ERR_WRONG_LF_VERSION                    32
#define ERR_CORRUPT_LICENSE_FILE                33
#define ERR_SEM_FILE_LOCKED                     34
#define ERR_CORRUPT_CONTROL_FILE                35
#define ERR_WRONG_CF_SERIAL_NUM                 36
#define ERR_LF_LOCKED                           37
#define ERR_LF_CHECKSUM_INVALID                 38
#define ERR_NOT_APPLICABLE                      39
#define ERR_NOT_IMPLEMENTED_YET                 40
#define ERR_FILE_EXISTS                         41
#define ERR_REGISTRY_OPEN                       42
#define ERR_REGISTRY_QUERY                      43
#define ERR_REGISTRY_CLOSE                      44
#define ERR_REGISTRY_READ                       45
#define ERR_REGISTRY_SET                        46
#define ERR_CBOX_NOT_PRESENT                    47
#define ERR_CBOX_WRONG_TYPE                     48
#define ERR_CBOX_READ_RAM1_ERROR                49
#define ERR_CBOX_READ_RAM2_ERROR                50
#define ERR_CBOX_WRITE_RAM1_ERROR               51
#define ERR_CBOX_WRITE_RAM2_ERROR               52
#define ERR_CBOX_ID1_ERROR                      53
#define ERR_CBOX_ID2_ERROR                      54
#define ERR_CBOX_ID3_ERROR                      55
#define ERR_VAR_NOT_AVAILABLE                   56
#define ERR_DEMO_HAS_EXPIRED                    57
#define ERR_WINSOCK_STARTUP_ERROR               58
#define ERR_WINSOCK_CANNOT_RESOLVE_HOST         59
#define ERR_WINSOCK_CANNOT_CREATE_SOCKET        60
#define ERR_WINSOCK_CANNOT_CONNECT_TO_SERVER    61
#define ERR_WINSOCK_CANNOT_SEND_DATA            62
#define ERR_WINSOCK_CANNOT_READ_DATA            63
#define ERR_NO_MORE_SOFTWARE_KEYS_AVAILABLE     64
#define ERR_INVALID_SERVER_RESPONSE             65
#define ERR_CANNOT_ALLOCATE_MEMORY              66
#define ERR_WINSOCK_CANNOT_RESOLVE_PROXY        67
#define ERR_ALIAS_FILE_DOES_NOT_MATCH           68
#define ERR_INVALID_CODE_ENTERED                69
#define ERR_INVALID_REGKEY2_ENTERED             70
#define ERR_ACCESS_DENIED						71
#define ERR_NON_WIN32_OS						72
#define ERR_DRIVER_CORRUPT						73
#define ERR_MARKED_FOR_DELETE					74
#define ERR_INVALID_NETHANDLE					75
#define ERR_NO_MESSAGES							76
#define ERR_ALREADY_OPEN						77
#define ERR_INVALID_NETWORK_ADDRESS				78
#define ERR_NON_WIN64_OS						79
#define ERR_NOT_SUPPORTED_BY_OS					80
#define ERR_COULD_NOT_LOAD_DLL					81
#define ERR_FUNCTION_NOT_AVAILABLE				82
#define ERR_NO_DRIVES_FOUND						83
#define ERR_INTERNAL_BROADCAST					84
#define ERR_BIND_FAILED							85
#define ERR_CANNOT_CREATE_THREAD				86
#define ERR_NETWORK_CONNECTIVITY				87
#define ERR_COULD_NOT_INSTALL_DRIVER			88
#define ERR_NETWORK_RECONNECTED					89
#define ERR_NET_MULTI_STARTUP					90
#define ERR_BUFFER_TOO_SMALL					91

#define ERR_FILE_SIZE_MISMATCH					100
#define ERR_FILE_INCORRECT						101



/* compno returns for COMPNO_BIOS32 */
#define ERR_ACCESS_DENIED_BIOS32				-71
#define ERR_NON_WIN32_OS_BIOS32					-72
#define ERR_DRIVER_CORRUPT_BIOS32				-73
#define ERR_MARKED_FOR_DELETE_BIOS32			-74



/* file attributes defines */
#define PP_NORMAL                       0x0000
#define PP_RDONLY                       0x0001
#define PP_HIDDEN                       0x0002
#define PP_SYSTEM                       0x0004



/* expiration types */
#define EXP_NONE                       "N"
#define EXP_EXE_LIMIT                  "E"
#define EXP_SHAREWARE_VER              "S"
#define EXP_PAYMENT_LIMIT              "P"
#define EXP_DEMO_VERSION               "D"



/* getvar / setvar definitions - character fields */
#define VAR_COMPANY                    1
#define VAR_NAME                       2
#define VAR_ADDRESS1                   3
#define VAR_ADDRESS2                   4
#define VAR_ADDRESS3                   5
#define VAR_PHONE1                     6
#define VAR_PHONE2                     7
#define VAR_SERIAL_TEXT                8
#define VAR_EXPIRE_TYPE                9
#define VAR_UDEF_CHAR_1                10
#define VAR_UDEF_CHAR_2                11
#define VAR_UDEF_CHAR_3                12
#define VAR_UDEF_CHAR_4                13
#define VAR_UDEF_CHAR_5                14
#define VAR_UDEF_CHAR_6                15
#define VAR_UDEF_CHAR_7                16
#define VAR_UDEF_CHAR_8                17
#define VAR_UDEF_CHAR_9                18
#define VAR_UDEF_CHAR_10               19
#define VAR_EZTRIAL_COMPNO_DRIVE       20
#define VAR_EZTRIAL_SYSDIR_FILENAME    21
#define VAR_EZTRIG_COMPNO_DRIVE        22
#define VAR_EZTRIAL_REG_ALIAS1         23
#define VAR_EZTRIAL_REG_ALIAS2         24
#define VAR_EZTRIG_DLG_LABELS          25
#define VAR_AUTOCL_PROXY_ADDR          26
#define VAR_AUTOCL_URL_VISIT           27
#define VAR_AUTOCL_URL_BUYNOW          28
#define VAR_AUTOCL_URL_UNLOCK          29
#define VAR_AUTOCL_URL_REGISTER        30
#define VAR_EZTRIAL_COMPNO_FILE		   31
#define VAR_EZTRIG_COMPNO_FILE		   32
#define VAR_LICENSEPW                  33
#define VAR_INSTALLATIONID             34
#define VAR_LICENSEKEY                 35



/* getvar / setvar definitions - numeric fields */
#define VAR_SERIAL_NUM                 1
#define VAR_EXP_COUNT                  2
#define VAR_EXP_LIMIT                  3
#define VAR_LAN_COUNT                  4
#define VAR_LAN_LIMIT                  5
#define VAR_INSTALL_COUNT              6
#define VAR_INSTALL_LIMIT              7
#define VAR_AUTHORIZED_COMPS           8
#define VAR_UDEF_NUM_1                 9
#define VAR_UDEF_NUM_2                 10
#define VAR_UDEF_NUM_3                 11
#define VAR_UDEF_NUM_4                 12
#define VAR_UDEF_NUM_5                 13
#define VAR_LF_CHECKSUM                14
#define VAR_EZTRIAL_SOFT_BINDING       15
#define VAR_EZTRIAL_HARD_BINDING       16
#define VAR_EZTRIAL_COMPNO_THRESHOLD   17
#define VAR_EZTRIAL_CONVERT_COPIES     18
#define VAR_EZTRIAL_UPDATE_LAST_TIME   19
#define VAR_EZTRIAL_COMPNO_ALGORITHMS  20
#define VAR_EZTRIAL_FILE_VERSION       21
#define VAR_EZTRIG_FLAGS               22
#define VAR_EZTRIG_COMPNO_ALGORITHMS   23
#define VAR_EZTRIG_SEED                24
#define VAR_EZTRIG_REGKEY2SEED         25
#define VAR_EZTRIAL_DAYS_TO_RUN        26
#define VAR_EZTRIAL_TIMES_TO_RUN       27
#define VAR_LICENSEID                  28
#define VAR_CUSTOMERID                 29
	


/* getvar / setvar definitions - date fields */
#define VAR_EXP_DATE_SOFT              1
#define VAR_EXP_DATE_HARD              2
#define VAR_LAST_DATE                  3
#define VAR_LAST_TIME                  4
#define VAR_UDEF_DATE_1                5
#define VAR_UDEF_DATE_2                6
#define VAR_UDEF_DATE_3                7
#define VAR_UDEF_DATE_4                8
#define VAR_UDEF_DATE_5                9
#define VAR_LAST_DATE_UTC              10
#define VAR_LAST_TIME_UTC              11



/* for compatibility with previous versions */
#define VAR_PRODUCT              VAR_UDEF_CHAR_2
#define VAR_DISTRIBUTOR          VAR_UDEF_CHAR_3
#define VAR_USER_DEF_1           VAR_UDEF_CHAR_1
#define VAR_USER_DEF_2           VAR_UDEF_NUM_1
#define VAR_USER_DEF_3           VAR_UDEF_NUM_2
#define VAR_USER_DEF_4           VAR_UDEF_DATE_1


/* windows defines */
#ifndef PPPEXPORT
	#define PPPEXPORT
#endif

#ifndef WINAPI
	#define WINAPI              __stdcall
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



/* function name defines */
#define PP_ADDDAYS          pp_adddays
#define PP_BITCLEAR         pp_bitclear
#define PP_BITSET           pp_bitset
#define PP_BITTEST          pp_bittest
#define PP_CDPROTECT        pp_cdprotect
#define PP_CEDATE           pp_cedate
#define PP_CHECKSUM         pp_checksum
#define PP_CENUM            pp_cenum
#define PP_CHKVARCHAR       pp_chkvarchar
#define PP_CHKVARDATE       pp_chkvardate
#define PP_CHKVARNUM        pp_chkvarnum
#define PP_COMPNO           pp_compno
#define PP_COMPNOINIT       pp_compnoinit
#define PP_COMPNO32DRIVERS  pp_compno32drivers
#define PP_CONVERTV3        pp_convertv3
#define PP_COPYADD          pp_copyadd
#define PP_COPYCHECK        pp_copycheck
#define PP_COPYCHECKTH      pp_copycheckth
#define PP_COPYDELETE       pp_copydelete
#define PP_COPYGET          pp_copyget
#define PP_COUNTDEC         pp_countdec
#define PP_COUNTINC         pp_countinc
#define PP_CTCODES          pp_ctcodes
#define PP_DAYSLEFT         pp_daysleft
#define PP_DECRYPT          pp_decrypt
#define PP_ENCRYPT          pp_encrypt
#define PP_ERRORSTR         pp_errorstr
#define PP_EXPIRED          pp_expired
#define PP_EXPORTACTFILE    pp_exportactfile
#define PP_EZTRIAL1         pp_eztrial1
#define PP_EZTRIAL1EX       pp_eztrial1ex
#define PP_EZTRIAL1TEST     pp_eztrial1test
#define PP_EZTRIAL2         pp_eztrial2
#define PP_EZTRIG1          pp_eztrig1
#define PP_EZTRIG1DLG       pp_eztrig1dlg
#define PP_EZTRIG1EX        pp_eztrig1ex
#define PP_FILEDELETE       pp_filedelete
#define PP_GETCODE          pp_getcode
#define PP_GETCOMPNO        pp_getcompno
#define PP_GETCOMPNOXML     pp_getcompnoxml
#define PP_GETDATE          pp_getdate
#define PP_GETDATEEX        pp_getdateex
#define PP_GETTIME          pp_gettime
#define PP_GETTIMEEX        pp_gettimeex
#define PP_GETVARCHAR       pp_getvarchar
#define PP_GETVARDATE       pp_getvardate
#define PP_GETVARNUM        pp_getvarnum
#define PP_GOTOURL          pp_gotourl
#define PP_HDSERIAL         pp_hdserial
#define PP_IMPORTACTFILE    pp_importactfile
#define PP_INITLIB          pp_initlib
#define PP_LASTDAY          pp_lastday
#define PP_LANACTIVE        pp_lanactive
#define PP_LANCHECK         pp_lancheck
#define PP_LIBTEST          pp_libtest
#define PP_LFALIAS          pp_lfalias
#define PP_LFCLOSE          pp_lfclose
#define PP_LFCOPY           pp_lfcopy
#define PP_LFCREATE         pp_lfcreate
#define PP_LFDELETE         pp_lfdelete
#define PP_LFLOCK           pp_lflock
#define PP_LFOPEN           pp_lfopen
#define PP_LFPERMSET        pp_lfpermset
#define PP_LFUNLOCK         pp_lfunlock
#define PP_NDECRYPT         pp_ndecrypt
#define PP_NDECRYPTX        pp_ndecryptx
#define PP_NENCRYPT         pp_nencrypt
#define PP_NENCRYPTX        pp_nencryptx
#define PP_NETCLOSE         pp_netclose
#define PP_NETOPEN          pp_netopen
#define PP_NETTEST          pp_nettest
#define PP_NPDATE           pp_npdate
#define PP_PASSWORD         pp_password
#define PP_REDIR            pp_redir
#define PP_SEMCLOSE         pp_semclose
#define PP_SEMCOUNT         pp_semcount
#define PP_SEMOPEN          pp_semopen
#define PP_SEMTEST          pp_semtest
#define PP_SEMUSED          pp_semused
#define PP_SETVARCHAR       pp_setvarchar
#define PP_SETVARDATE       pp_setvardate
#define PP_SETVARNUM        pp_setvarnum
#define PP_SYSINFO          pp_sysinfo
#define PP_TCODE            pp_tcode
#define PP_TIMERCHECK       pp_timercheck
#define PP_TIMERSTART       pp_timerstart
#define PP_TRANSFER         pp_transfer
#define PP_UPDDATE          pp_upddate
#define PP_VALDATE          pp_valdate
#define PP_LIBVERSION       pp_libversion
#define PP_GET4108MAC       pp_get4108mac



/* function prototypes */
VOID PPPEXPORT WINAPI PP_ADDDAYS( LPLONG month, LPLONG day, LPLONG year, LONG days );
LONG PPPEXPORT WINAPI PP_BITCLEAR( LPLONG bit_field, LONG bit_number );
LONG PPPEXPORT WINAPI PP_BITSET( LPLONG bit_field, LONG bit_number );
LONG PPPEXPORT WINAPI PP_BITTEST( LONG bit_field, LONG bit_number );
LONG PPPEXPORT WINAPI PP_CDPROTECT(	LONG flags,	LPSTR filename,	LONG sectorsize, LONG numsectors, LONG validations, LONG seed1, LONG seed2, LONG seed3, LONG seed4);
VOID PPPEXPORT WINAPI PP_CEDATE( LONG cenum, LPLONG month, LPLONG day, LPLONG year );
LONG PPPEXPORT WINAPI PP_CHECKSUM( LPSTR filename, LPLONG checksum );
LONG PPPEXPORT WINAPI PP_CENUM( VOID );
LONG PPPEXPORT WINAPI PP_CHKVARCHAR( LONG type, LONG var_no );
LONG PPPEXPORT WINAPI PP_CHKVARDATE( LONG type, LONG var_no );
LONG PPPEXPORT WINAPI PP_CHKVARNUM( LONG type, LONG var_no );
LONG PPPEXPORT WINAPI PP_COMPNO32DRIVERS( LONG driver, LONG flags);
LONG PPPEXPORT WINAPI PP_COMPNO( LONG cnotype, LPSTR filename, LPSTR hard_drive );
LONG PPPEXPORT WINAPI PP_COMPNOINIT(LONG flags, PPLFHANDLE lfhandle);
LONG PPPEXPORT WINAPI PP_CONVERTV3( PPLFHANDLE handle, LPSTR v3_cf, LPSTR v3_cpf, LONG v3_sn );
LONG PPPEXPORT WINAPI PP_COPYADD( PPLFHANDLE handle, LONG flags, LONG value );
LONG PPPEXPORT WINAPI PP_COPYCHECK( PPLFHANDLE handle, LONG action, LONG comp_num );
LONG PPPEXPORT WINAPI PP_COPYCHECKTH( PPLFHANDLE handle, LONG action, LONG comp_num, LONG threshold );
LONG PPPEXPORT WINAPI PP_COPYDELETE( PPLFHANDLE handle, LONG comp_num );
LONG PPPEXPORT WINAPI PP_COPYGET( PPLFHANDLE handle, LONG slot, LPLONG comp_num );
LONG PPPEXPORT WINAPI PP_COUNTDEC( PPLFHANDLE handle, LONG var_no );
LONG PPPEXPORT WINAPI PP_COUNTINC( PPLFHANDLE handle, LONG var_no );
LONG PPPEXPORT WINAPI PP_CTCODES( LONG code, LONG cenum, LONG computer, LONG seed );
LONG PPPEXPORT WINAPI PP_DAYSLEFT( PPLFHANDLE handle, LPLONG daysleft );
VOID PPPEXPORT WINAPI PP_DECRYPT( LPSTR instr, LPSTR pwstr, LPSTR ret );
VOID PPPEXPORT WINAPI PP_ENCRYPT( LPSTR instr, LPSTR pwstr, LPSTR ret );
VOID PPPEXPORT WINAPI PP_ERRORSTR( LONG number, LPSTR buffer );
LONG PPPEXPORT WINAPI PP_EXPIRED( PPLFHANDLE handle );
LONG PPPEXPORT WINAPI PP_EXPORTACTFILE(LONG flags, PPLFHANDLE lfhandle, LPSTR licensepath, LPSTR filepath, LPSTR server, LPSTR url, LPSTR key, LONG licenseid, LPSTR password, LONG compid, LPSTR version, LONG productid, LPSTR source);
LONG PPPEXPORT WINAPI PP_EZTRIAL1( LPSTR filename, LPSTR password, LPLONG errorcode, LPPPLFHANDLE lfhandle );
LONG PPPEXPORT WINAPI PP_EZTRIAL1EX( LPSTR filename, LPSTR password, LONG flags, LPLONG errorcode, LPPPLFHANDLE lfhandle );
LONG PPPEXPORT WINAPI PP_EZTRIAL1TEST( PPLFHANDLE lfhandle, LPLONG errorcode );
LONG PPPEXPORT WINAPI PP_EZTRIAL2( PPHWND hwndDlg, LPSTR filename, LPSTR password, LONG flags, LPLONG errorcode, LPPPLFHANDLE lfhandle );
LONG PPPEXPORT WINAPI PP_EZTRIG1( PPHWND hwnd, LPSTR filename, LPSTR password, LPLONG errorcode );
LONG PPPEXPORT WINAPI PP_EZTRIG1DLG( PPHWND hwnd, PPLFHANDLE handle, LONG flags, LPSTR labels, LONG usercode1, LONG usercode2, LONG tcseed, LONG regkey2seed, LPLONG tcvalue, LPLONG tcdata );
LONG PPPEXPORT WINAPI PP_EZTRIG1EX( PPLFHANDLE handle, LONG regkey1, LONG regkey2, LONG flags, LONG usercode1, LONG usercode2, LONG tcseed, LONG regkey2seed, LPLONG tcvalue, LPLONG tcdata );
LONG PPPEXPORT WINAPI PP_FILEDELETE( LPSTR filename );
LONG PPPEXPORT WINAPI PP_GETCODE( PPHWND hwnd, LPSTR str_title, LPSTR str_cenum, LPSTR str_comp, LPSTR str_code );
LONG PPPEXPORT WINAPI PP_GETCOMPNO(LONG flags, PPLFHANDLE lfhandle, LPLONG compno);
LONG PPPEXPORT WINAPI PP_GETCOMPNOXML(LONG flags, PPLFHANDLE lfhandle, LPSTR buffer, LONG bufferSize);
VOID PPPEXPORT WINAPI PP_GETDATE( LPLONG month, LPLONG day, LPLONG year, LPLONG dayofweek );
VOID PPPEXPORT WINAPI PP_GETDATEEX( LONG flags, LPLONG month, LPLONG day, LPLONG year, LPLONG dayofweek );
VOID PPPEXPORT WINAPI PP_GETTIME( LPLONG hours, LPLONG minutes, LPLONG seconds, LPLONG hseconds );
VOID PPPEXPORT WINAPI PP_GETTIMEEX( LONG flags, LPLONG hours, LPLONG minutes, LPLONG seconds, LPLONG hseconds );
LONG PPPEXPORT WINAPI PP_GETVARCHAR( PPLFHANDLE handle, LONG var_no, LPSTR buffer );
LONG PPPEXPORT WINAPI PP_GETVARDATE( PPLFHANDLE handle, LONG var_no, LPLONG month_hours, LPLONG day_minutes, LPLONG year_seconds );
LONG PPPEXPORT WINAPI PP_GETVARNUM( PPLFHANDLE handle, LONG var_no, LPLONG value );
LONG PPPEXPORT WINAPI PP_GOTOURL( PPHWND hwnd, PPLFHANDLE lfhandle, LONG flags, LPSTR url );
LONG PPPEXPORT WINAPI PP_HDSERIAL( LPSTR drive );
LONG PPPEXPORT WINAPI PP_IMPORTACTFILE(LONG flags, PPLFHANDLE lfhandle, LONG type, LONG compID, LPLONG regkey1, LPLONG regkey2, LPLONG session, LPSTR licenseupd); 
VOID WINAPI PP_INITLIB( HINSTANCE hInstance ); /* this function is retained only for backwards compatibility; remove if you have errors related to HINSTANCE when compiling */
VOID PPPEXPORT WINAPI PP_LASTDAY( LONG month, LPLONG day, LONG year );
LONG PPPEXPORT WINAPI PP_LANACTIVE( PPLFHANDLE handle );
LONG PPPEXPORT WINAPI PP_LANCHECK( PPLFHANDLE handle );
LONG PPPEXPORT WINAPI PP_LIBTEST( LONG testnum );
VOID PPPEXPORT WINAPI PP_LIBVERSION(LPLONG ver1, LPLONG ver2, LPLONG ver3, LPLONG ver4);
LONG PPPEXPORT WINAPI PP_LFALIAS( PPLFHANDLE handle, LPSTR filename, LONG flags, LONG type, LPSTR password, LPPPLFHANDLE recenthandle );
LONG PPPEXPORT WINAPI PP_LFCLOSE( PPLFHANDLE handle );
LONG PPPEXPORT WINAPI PP_LFCOPY( PPLFHANDLE handle, LPSTR filename, LONG lftype );
LONG PPPEXPORT WINAPI PP_LFCREATE( LPSTR file, LONG flags, LONG lftype, LPSTR password, LONG attrib );
LONG PPPEXPORT WINAPI PP_LFDELETE( LPSTR filename, LONG flags, LONG type, LPSTR password );
LONG PPPEXPORT WINAPI PP_LFLOCK( PPLFHANDLE mem_handle );
LONG PPPEXPORT WINAPI PP_LFOPEN( LPSTR filename, LONG flags, LONG type, LPSTR password, LPPPLFHANDLE handle );
LONG PPPEXPORT WINAPI PP_LFPERMSET( LPSTR filename, LONG flags, LONG type );
LONG PPPEXPORT WINAPI PP_LFUNLOCK( PPLFHANDLE mem_handle );
LONG PPPEXPORT WINAPI PP_COMPNO32DRIVERS( LONG DRIVER, LONG FLAGS);
LONG PPPEXPORT WINAPI PP_NDECRYPT( LONG number, LONG seed );
LONG PPPEXPORT WINAPI PP_NDECRYPTX( LPSTR buffer, LPLONG value1, LPLONG value2, LPLONG value3, LPLONG value4, LONG seed );
LONG PPPEXPORT WINAPI PP_NENCRYPT( LONG number, LONG seed );
VOID PPPEXPORT WINAPI PP_NENCRYPTX( LPSTR buffer, LONG value1, LONG value2, LONG value3, LONG value4, LONG seed );
LONG PPPEXPORT WINAPI PP_NETCLOSE(PPLFHANDLE lNetHandle);
LONG PPPEXPORT WINAPI PP_NETOPEN(LPSTR network_password, LONG flags, LONG number1, LPSTR string1, LPPPLFHANDLE nethandle, LPLONG errorcode);
LONG PPPEXPORT WINAPI PP_NETTEST(PPLFHANDLE lNetHandle);
VOID PPPEXPORT WINAPI PP_NPDATE( LPLONG month, LPLONG day, LPLONG year, LONG dop );
LONG PPPEXPORT WINAPI PP_PASSWORD( LPSTR instr );
LONG PPPEXPORT WINAPI PP_REDIR( LPSTR drive );
LONG PPPEXPORT WINAPI PP_SEMCLOSE( PPLFHANDLE handle );
LONG PPPEXPORT WINAPI PP_SEMCOUNT( PPLFHANDLE handle, LONG semtype, LPSTR prefix_server, LPSTR name, LPLONG number );
LONG PPPEXPORT WINAPI PP_SEMOPEN( PPLFHANDLE handle, LONG semtype, LPSTR prefix_server, LPSTR name, LPPPLFHANDLE sem_handle );
LONG PPPEXPORT WINAPI PP_SEMTEST( PPLFHANDLE sem_handle );
LONG PPPEXPORT WINAPI PP_SEMUSED( PPLFHANDLE handle, LONG semtype, LPSTR prefix_server, LPSTR name, LPLONG number );
LONG PPPEXPORT WINAPI PP_SETVARCHAR( PPLFHANDLE handle, LONG var_no, LPSTR buffer );
LONG PPPEXPORT WINAPI PP_SETVARDATE( PPLFHANDLE handle, LONG var_no, LONG month_hours, LONG day_minutes, LONG year_seconds );
LONG PPPEXPORT WINAPI PP_SETVARNUM( PPLFHANDLE handle, LONG var_no, LONG value );
LONG PPPEXPORT WINAPI PP_SYSINFO(LONG flags);
LONG PPPEXPORT WINAPI PP_TCODE( LONG number, LONG cenum, LONG computer, LONG seed );
LONG PPPEXPORT WINAPI PP_TIMERCHECK( LONG timestamp, LONG minutes );
LONG PPPEXPORT WINAPI PP_TIMERSTART( VOID );
LONG PPPEXPORT WINAPI PP_TRANSFER( PPLFHANDLE handle, LPSTR filename, LPSTR password, LONG comp_num );
LONG PPPEXPORT WINAPI PP_UPDDATE( PPLFHANDLE handle, LONG flag );
LONG PPPEXPORT WINAPI PP_VALDATE( PPLFHANDLE handle );
LONG PPPEXPORT WINAPI PP_GET4108MAC( VOID );

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus     /* End of extern "C" { */
}
#endif  /* __cplusplus */

#endif
