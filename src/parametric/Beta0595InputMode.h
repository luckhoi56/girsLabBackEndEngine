// Beta0595InputMode.h: interface for the Beta0595InputMode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BETA0595INPUTMODE_H__15DDEC84_A2F4_4DDC_985C_E451F9407169__INCLUDED_)
#define AFX_BETA0595INPUTMODE_H__15DDEC84_A2F4_4DDC_985C_E451F9407169__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricInputMode.h"

class Beta0595InputMode : public ParametricInputMode  
{
public:
	Beta0595InputMode();
	Beta0595InputMode(double zlo, double zhi);
	virtual ~Beta0595InputMode();
	virtual int convert(double i1, double i2, double & o2, double & o3);
private:

	double intersect_phi(double r, double z, double q, double phi0, double phi1);

	double upper_z;
	double lower_z;
};

#endif // !defined(AFX_BETA0595INPUTMODE_H__15DDEC84_A2F4_4DDC_985C_E451F9407169__INCLUDED_)
