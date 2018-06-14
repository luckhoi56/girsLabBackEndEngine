// $Id: WeibullModelLayout.h 2238 2014-02-22 15:48:40Z Dongfeng Zhu $

#pragma once

#include "../core/ParamArray.h"

/**
 * Definition of layout of variables for a Weibull model.
 *
 * This class defines how the shape and scale variables 
 * for the Weibull model should be laid out in the 
 * parameter array. Note that some other classes may also
 * have internal knowledge of the layout, even though this
 * class should be used in general.
 */ 
class WeibullModelLayout
{
public:
	WeibullModelLayout(void);
	~WeibullModelLayout(void);

	/**
	 * Returns the index of the shape parameter for give mode and node.
	 * Failure modes start counting at 1. Nodes start counting at 0.
	 */
	static int getShapeIndex(const int mode,const int node_cnt) 
		{return (mode - 1) * (node_cnt + 1);}
	/**
	 * Returns the index of the scale parameter for give mode and node.
	 * Failure modes start counting at 1. Nodes start counting at 0.
	 */
	static int getScaleIndex(const int node, const int mode, const int node_cnt)  
		{return (mode - 1) * (node_cnt + 1) + node + 1;}

	/**
	 * Returns true if index is a shape parameter in the model.
	 */
	static int isShapeParameter(const int index, const int node_cnt) 
		{return (index % (node_cnt + 1) == 0);}

	/**
	 * Returns true if index is a scale parameter for the specified node.
	 */
	static int isScaleParameter(const int index, const int node, const int node_cnt) 
	{return (index % (node_cnt + 1)) == (node + 1);}

	/**
	 * Returns the number of variables required for the model.
	 */
	static int getSize(const int mode_cnt, const int node_cnt)
		{return mode_cnt * (node_cnt + 1);}

	static ParamArray * createModel(const int mode_cnt, const int node_cnt);

};
