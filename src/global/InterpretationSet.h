// InterpretationSet.h: interface for the InterpretationSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERPRETATIONSET_H__265CFB29_3E72_11D4_9881_002018557056__INCLUDED_)
#define AFX_INTERPRETATIONSET_H__265CFB29_3E72_11D4_9881_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Interpretation.h"

#define MAX_INTERPRETATIONS 5

/**
 * Set of interpretations. Can be used to construct 
 * the joint interpretation of a set of observations,
 * to which different rules of interpretation apply.
 *
 * Maximum number of interpretations that can be held
 * by this implementation is defined by the MAX_INTERPRETATIONS.
 * In general, the isFull() function should be used to
 * test whether more interpretations can be added.
 */
class InterpretationSet : public Interpretation  
{
public:

	InterpretationSet();
	virtual ~InterpretationSet();

	/**
	 * Computes the joint likelihood for the 
	 * interpretations added to the set.
	 */
	double getLogLikelihood(const Parameters &params);

	/**
	 * Clears the interpretations from the set.
	 * Interpretations are deallocated.
	 */
	void clear();

	/**
	 * Adds an interpretation to the set.
	 * The interpretation that is added must be dynamically allocated,
	 * since it is deallocated by the InterpretationSet object when
	 * the set itself is deallocated.
	 * If the set is already full, the new interpretation is ignored.
	 */
	void addInterpretation(InterpretationPtr interpretation);

	/**
	 * Returns TRUE if no more interpretations can be added to the set.
	 */
	bool isFull();

private:
	int _cursor;
	InterpretationPtr _interpretations[MAX_INTERPRETATIONS];
};

#endif // !defined(AFX_INTERPRETATIONSET_H__265CFB29_3E72_11D4_9881_002018557056__INCLUDED_)
