// $Id: WeibullModelLayout.cpp 275 2005-03-01 03:53:40Z fgroen $

#include "WeibullModelLayout.h"
#include "../core/SliceSamplerAids.h"

WeibullModelLayout::WeibullModelLayout(void)
{
}

WeibullModelLayout::~WeibullModelLayout(void)
{
}

/**
 * Allocates and sets up a Weibull model.
 * @param mode_cnt number of failure modes
 * @param node_cnt number of product nodes.
 */
ParamArray * WeibullModelLayout::createModel(const int mode_cnt, const int node_cnt)
{
	ParamArray * arr = new ParamArray(getSize(mode_cnt,node_cnt));
	for (int mode = 1 ; mode <= mode_cnt ; mode++) 
	{
		int shape = getShapeIndex(mode,node_cnt);
		//arr->setRange(shape,arr->RANGE_POSITIVE);
		//arr->setParam(shape,1);
		arr->setAid(shape,new PositiveSliceSamplerAid(0.5,4,1));
		for (int node = 0 ; node < node_cnt ; node++) 
		{
			int scale = getScaleIndex(node,mode,node_cnt);
			//arr->setRange(scale,arr->RANGE_REAL);
			//arr->setParam(scale,0);
			arr->setAid(shape,new DefaultSliceSamplerAid(0.5,4,0));
		}
	}
	return arr;
}