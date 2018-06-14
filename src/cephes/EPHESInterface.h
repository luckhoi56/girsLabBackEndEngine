// EPHESInterface.h: interface for the CEPHESInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPHESINTERFACE_H__1C07519E_EE0A_41B0_ADFD_EA32E116C269__INCLUDED_)
#define AFX_EPHESINTERFACE_H__1C07519E_EE0A_41B0_ADFD_EA32E116C269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEPHESInterface  
{
public:
	static double exponentIntegralEn(int n, double x);
	CEPHESInterface();
	virtual ~CEPHESInterface();

};

#endif // !defined(AFX_EPHESINTERFACE_H__1C07519E_EE0A_41B0_ADFD_EA32E116C269__INCLUDED_)
