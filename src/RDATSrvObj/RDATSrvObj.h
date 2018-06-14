/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Feb 14 00:40:15 2012
 */
/* Compiler settings for C:\zhudf\PTI\svn.ptdev\native\redcas\RDATSrvObj\RDATSrvObj.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __RDATSrvObj_h__
#define __RDATSrvObj_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IRedcassrv_FWD_DEFINED__
#define __IRedcassrv_FWD_DEFINED__
typedef interface IRedcassrv IRedcassrv;
#endif 	/* __IRedcassrv_FWD_DEFINED__ */


#ifndef ___IRedcassrvEvents_FWD_DEFINED__
#define ___IRedcassrvEvents_FWD_DEFINED__
typedef interface _IRedcassrvEvents _IRedcassrvEvents;
#endif 	/* ___IRedcassrvEvents_FWD_DEFINED__ */


#ifndef __Redcassrv_FWD_DEFINED__
#define __Redcassrv_FWD_DEFINED__

#ifdef __cplusplus
typedef class Redcassrv Redcassrv;
#else
typedef struct Redcassrv Redcassrv;
#endif /* __cplusplus */

#endif 	/* __Redcassrv_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IRedcassrv_INTERFACE_DEFINED__
#define __IRedcassrv_INTERFACE_DEFINED__

/* interface IRedcassrv */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRedcassrv;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CF739BAA-54E6-40E1-8022-14AEB5D73482")
    IRedcassrv : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE destroy( 
            /* [in] */ int iHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE create( 
            /* [in] */ int type,
            /* [in] */ double p1,
            /* [in] */ double p2,
            /* [in] */ double p3,
            /* [in] */ double p4,
            /* [retval][out] */ int __RPC_FAR *piHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getPDF( 
            /* [in] */ int iHandle,
            /* [in] */ double x,
            /* [retval][out] */ double __RPC_FAR *pdPDF) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getCDF( 
            /* [in] */ int iHandle,
            /* [in] */ double x,
            /* [retval][out] */ double __RPC_FAR *pdCDF) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getQuantile( 
            /* [in] */ int iHandle,
            /* [in] */ double z,
            /* [retval][out] */ double __RPC_FAR *pdQuantile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getMean( 
            /* [in] */ int iHandle,
            /* [retval][out] */ double __RPC_FAR *pdMean) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getMedian( 
            /* [in] */ int iHandle,
            /* [retval][out] */ double __RPC_FAR *pdMedian) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getVariance( 
            /* [in] */ int iHandle,
            /* [retval][out] */ double __RPC_FAR *pdVariance) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE creates( 
            /* [in] */ int type,
            /* [in] */ double p1,
            /* [in] */ double p2,
            /* [retval][out] */ int __RPC_FAR *piHandle) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRedcassrvVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRedcassrv __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRedcassrv __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRedcassrv __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *destroy )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int iHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *create )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int type,
            /* [in] */ double p1,
            /* [in] */ double p2,
            /* [in] */ double p3,
            /* [in] */ double p4,
            /* [retval][out] */ int __RPC_FAR *piHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getPDF )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int iHandle,
            /* [in] */ double x,
            /* [retval][out] */ double __RPC_FAR *pdPDF);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getCDF )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int iHandle,
            /* [in] */ double x,
            /* [retval][out] */ double __RPC_FAR *pdCDF);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getQuantile )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int iHandle,
            /* [in] */ double z,
            /* [retval][out] */ double __RPC_FAR *pdQuantile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getMean )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int iHandle,
            /* [retval][out] */ double __RPC_FAR *pdMean);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getMedian )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int iHandle,
            /* [retval][out] */ double __RPC_FAR *pdMedian);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getVariance )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int iHandle,
            /* [retval][out] */ double __RPC_FAR *pdVariance);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *creates )( 
            IRedcassrv __RPC_FAR * This,
            /* [in] */ int type,
            /* [in] */ double p1,
            /* [in] */ double p2,
            /* [retval][out] */ int __RPC_FAR *piHandle);
        
        END_INTERFACE
    } IRedcassrvVtbl;

    interface IRedcassrv
    {
        CONST_VTBL struct IRedcassrvVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRedcassrv_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRedcassrv_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRedcassrv_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRedcassrv_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRedcassrv_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRedcassrv_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRedcassrv_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRedcassrv_destroy(This,iHandle)	\
    (This)->lpVtbl -> destroy(This,iHandle)

#define IRedcassrv_create(This,type,p1,p2,p3,p4,piHandle)	\
    (This)->lpVtbl -> create(This,type,p1,p2,p3,p4,piHandle)

#define IRedcassrv_getPDF(This,iHandle,x,pdPDF)	\
    (This)->lpVtbl -> getPDF(This,iHandle,x,pdPDF)

#define IRedcassrv_getCDF(This,iHandle,x,pdCDF)	\
    (This)->lpVtbl -> getCDF(This,iHandle,x,pdCDF)

#define IRedcassrv_getQuantile(This,iHandle,z,pdQuantile)	\
    (This)->lpVtbl -> getQuantile(This,iHandle,z,pdQuantile)

#define IRedcassrv_getMean(This,iHandle,pdMean)	\
    (This)->lpVtbl -> getMean(This,iHandle,pdMean)

#define IRedcassrv_getMedian(This,iHandle,pdMedian)	\
    (This)->lpVtbl -> getMedian(This,iHandle,pdMedian)

#define IRedcassrv_getVariance(This,iHandle,pdVariance)	\
    (This)->lpVtbl -> getVariance(This,iHandle,pdVariance)

#define IRedcassrv_creates(This,type,p1,p2,piHandle)	\
    (This)->lpVtbl -> creates(This,type,p1,p2,piHandle)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_destroy_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int iHandle);


void __RPC_STUB IRedcassrv_destroy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_create_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int type,
    /* [in] */ double p1,
    /* [in] */ double p2,
    /* [in] */ double p3,
    /* [in] */ double p4,
    /* [retval][out] */ int __RPC_FAR *piHandle);


void __RPC_STUB IRedcassrv_create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_getPDF_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int iHandle,
    /* [in] */ double x,
    /* [retval][out] */ double __RPC_FAR *pdPDF);


void __RPC_STUB IRedcassrv_getPDF_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_getCDF_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int iHandle,
    /* [in] */ double x,
    /* [retval][out] */ double __RPC_FAR *pdCDF);


void __RPC_STUB IRedcassrv_getCDF_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_getQuantile_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int iHandle,
    /* [in] */ double z,
    /* [retval][out] */ double __RPC_FAR *pdQuantile);


void __RPC_STUB IRedcassrv_getQuantile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_getMean_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int iHandle,
    /* [retval][out] */ double __RPC_FAR *pdMean);


void __RPC_STUB IRedcassrv_getMean_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_getMedian_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int iHandle,
    /* [retval][out] */ double __RPC_FAR *pdMedian);


void __RPC_STUB IRedcassrv_getMedian_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_getVariance_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int iHandle,
    /* [retval][out] */ double __RPC_FAR *pdVariance);


void __RPC_STUB IRedcassrv_getVariance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRedcassrv_creates_Proxy( 
    IRedcassrv __RPC_FAR * This,
    /* [in] */ int type,
    /* [in] */ double p1,
    /* [in] */ double p2,
    /* [retval][out] */ int __RPC_FAR *piHandle);


void __RPC_STUB IRedcassrv_creates_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRedcassrv_INTERFACE_DEFINED__ */



#ifndef __RDATSRVOBJLib_LIBRARY_DEFINED__
#define __RDATSRVOBJLib_LIBRARY_DEFINED__

/* library RDATSRVOBJLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_RDATSRVOBJLib;

#ifndef ___IRedcassrvEvents_DISPINTERFACE_DEFINED__
#define ___IRedcassrvEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IRedcassrvEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IRedcassrvEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("511B4161-7EE5-4619-995D-F3FB5DC8256F")
    _IRedcassrvEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IRedcassrvEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IRedcassrvEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IRedcassrvEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IRedcassrvEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IRedcassrvEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IRedcassrvEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IRedcassrvEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IRedcassrvEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IRedcassrvEventsVtbl;

    interface _IRedcassrvEvents
    {
        CONST_VTBL struct _IRedcassrvEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IRedcassrvEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IRedcassrvEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IRedcassrvEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IRedcassrvEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IRedcassrvEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IRedcassrvEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IRedcassrvEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IRedcassrvEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Redcassrv;

#ifdef __cplusplus

class DECLSPEC_UUID("9B87ED66-DE7A-4B33-BEDB-479A58DD79AD")
Redcassrv;
#endif
#endif /* __RDATSRVOBJLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
