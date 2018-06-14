// Gamma0595InputMode.h: interface for the Gamma0595InputMode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMMA0595INPUTMODE_H__5A40E817_D7B6_477D_96C4_D2E9B9C560C2__INCLUDED_)
#define AFX_GAMMA0595INPUTMODE_H__5A40E817_D7B6_477D_96C4_D2E9B9C560C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricInputMode.h"

class Gamma0595InputMode : public ParametricInputMode  
{
public:
	Gamma0595InputMode();
	Gamma0595InputMode(double plo, double phi);
	virtual ~Gamma0595InputMode();
	virtual int convert(double i1, double i2, double & o2, double & o3) ;
private:
	int computeShape(double r, double & k) const;
	int computeShape(double r, double plo, double phi, double & k) const;
	double upper_z;
	double lower_z;
};

#endif // !defined(AFX_GAMMA0595INPUTMODE_H__5A40E817_D7B6_477D_96C4_D2E9B9C560C2__INCLUDED_)
