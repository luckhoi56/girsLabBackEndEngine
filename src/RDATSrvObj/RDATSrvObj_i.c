/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Feb 14 00:40:15 2012
 */
/* Compiler settings for C:\zhudf\PTI\svn.ptdev\native\redcas\RDATSrvObj\RDATSrvObj.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IRedcassrv = {0xCF739BAA,0x54E6,0x40E1,{0x80,0x22,0x14,0xAE,0xB5,0xD7,0x34,0x82}};


const IID LIBID_RDATSRVOBJLib = {0x0655D9D3,0x80B1,0x4E0A,{0xB7,0x7E,0x57,0xA8,0xCA,0xC3,0x95,0xDF}};


const IID DIID__IRedcassrvEvents = {0x511B4161,0x7EE5,0x4619,{0x99,0x5D,0xF3,0xFB,0x5D,0xC8,0x25,0x6F}};


const CLSID CLSID_Redcassrv = {0x9B87ED66,0xDE7A,0x4B33,{0xBE,0xDB,0x47,0x9A,0x58,0xDD,0x79,0xAD}};


#ifdef __cplusplus
}
#endif

