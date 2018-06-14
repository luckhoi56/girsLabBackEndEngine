// TwoStageGP.h: interface for the TwoStageGP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TWOSTAGEGP_H__91601A0B_4AAA_4B6B_8F05_2F3104FBEEF2__INCLUDED_)
#define AFX_TWOSTAGEGP_H__91601A0B_4AAA_4B6B_8F05_2F3104FBEEF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TwoStageGP  
{
public:
	static void threeDee(double k, double T);
	static void stepByStep(double k, double T);
	TwoStageGP();
	virtual ~TwoStageGP();

};

#endif // !defined(AFX_TWOSTAGEGP_H__91601A0B_4AAA_4B6B_8F05_2F3104FBEEF2__INCLUDED_)
