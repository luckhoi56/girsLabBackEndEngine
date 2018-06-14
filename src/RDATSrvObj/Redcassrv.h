// Redcassrv.h : Declaration of the CRedcassrv

#ifndef __REDCASSRV_H_
#define __REDCASSRV_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRedcassrv
class ATL_NO_VTABLE CRedcassrv : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRedcassrv, &CLSID_Redcassrv>,
	public IConnectionPointContainerImpl<CRedcassrv>,
	public IDispatchImpl<IRedcassrv, &IID_IRedcassrv, &LIBID_RDATSRVOBJLib>
{
public:
	CRedcassrv()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_REDCASSRV)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRedcassrv)
	COM_INTERFACE_ENTRY(IRedcassrv)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CRedcassrv)
END_CONNECTION_POINT_MAP()

private:
	short m_iHandle;

// IRedcassrv
public:
	STDMETHOD(creates)(/*[in]*/ int type, /*[in]*/ double p1, /*[in]*/ double p2, /*[out, retval]*/ int * piHandle);
	STDMETHOD(getVariance)(/*[in]*/ int iHandle, /*[out, retval]*/ double * pdVariance);
	STDMETHOD(getMedian)(/*[in]*/ int iHandle, /*[out, retval]*/ double * pdMedian);
	STDMETHOD(getMean)(/*[in]*/ int iHandle, /*[out, retval]*/ double * pdMean);
	STDMETHOD(getQuantile)(/*[in]*/ int iHandle, /*[in]*/ double z, /*[out, retval]*/ double * pdQuantile);
	STDMETHOD(getCDF)(/*[in]*/ int iHandle, /*[in]*/ double x, /*[out, retval]*/ double * pdCDF);
	STDMETHOD(getPDF)(/*[in]*/ int iHandle, /*[in]*/ double x, /*[out, retval]*/ double * pdPDF);
	STDMETHOD(create)(/*[in]*/ int type, /*[in]*/ double p1, /*[in]*/ double p2, /*[in]*/ double p3, /*[in]*/ double p4, /*[out retval]*/ int * piHandle);
	STDMETHOD(destroy)(/*[in]*/ int iHandle);
};

#endif //__REDCASSRV_H_
