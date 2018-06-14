//
// File:      $Id: NHObservation.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHObservation.h"

NHObservation::NHObservation()
{

}

NHObservation::~NHObservation()
{

}

NHObservation::NHObservation(double a, double b, double c, double d) 
{
//	field1 = a; 
//	field2 = b; 
//	field3 = c; 
//	field4 = d; 

	median = a;
	sigma = b;
	events = c;
	exposure = d;

	factor = 0;
}

