//
// File:      $Id: NHObservation.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#if !defined(AFX_NHOBSERVATION_H__4CA96153_6298_4016_A44D_854BB20C68CC__INCLUDED_)
#define AFX_NHOBSERVATION_H__4CA96153_6298_4016_A44D_854BB20C68CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

using namespace std;


/**
 * Data class for nonhomogeneous observations.
 *
 * A single class is used to store the information, based on the 
 * assumption that all non-homogeneous models have the same format
 * in terms of their data requirements (three double values).
 * The first two double values represent the actual observation, while the third value
 * indicates its interpretation and it is solely used in the Non-Homogeneous mixed models.
 * By the default, the third parameter is set to -1. An interpretation type is specified 
 * by assigning values using the same predefined constants in NHModel.h:
 * #define NHINTERPRETATION_DEFAULT		       -1
 * #define NHINTERPRETATION_5TH_95TH			1
 * #define NHINTERPRETATION_MEAN_SDEV			2
 * #define NHINTERPRETATION_EVENTS_DEMANDS		3
 * #define NHINTERPRETATION_EVENTS_TIME		    4
 * 
 */

class NHObservation  
{
public:
	NHObservation(double a, double b, double c = 0, double d = 0);
	NHObservation();
	virtual ~NHObservation();
//	double field1;
//	double field2;
//	double field3;
//	double field4;

	double median;
	double sigma;
	double events;
	double exposure;

	double factor; // 'normalization' factor used by some models.
};

// an STL list of observations.
typedef list<NHObservation> NHObservationList;

// an iterator for the observation list.
typedef NHObservationList::iterator NHObservationIterator;

#endif // !defined(AFX_NHOBSERVATION_H__4CA96153_6298_4016_A44D_854BB20C68CC__INCLUDED_)
