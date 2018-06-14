// $Id: NetOperator.cpp 169 2005-01-28 14:17:28Z saulo $

#include "NetOperator.h"

/**
 * Global counter for assigning operator ID values.
 */
int NetOperator::next_id = 1;

NetOperator::NetOperator(void)
{
	next = 0;
	operator_id = next_id++;
}

/**
 * Copy constructor.
 * This constructor copies the operator id from the source.
 */
NetOperator::NetOperator(const NetOperator & op)
{
	next = 0;
	operator_id = op.operator_id;
}

/**
 * Destructor.
 * This destructor recursively destroys operators attached to it in the
 * operator list. THe operator lists are maintained by SampleArray for 
 * each parameter.
 */
NetOperator::~NetOperator(void)
{	
	delete next;
}

/**
 * Returns true if the operator list contains the specified operator.
 */
bool NetOperator::contains(const NetOperatorPtr op) const
{
	if (op == 0) return true;
	int id = op->getOperatorID();
	const NetOperator * it = this;
	while (it != 0) {
		if (it->operator_id == id) return true;
		it = it->next;
	}
	return false;
}


/**
 * Attaches the operator list at the end of the list headed by this operator.
 *
 * This method returns the head of the list after absorbtion. The
 * actual operators passed in are being inserted into the list, so 
 * copies should be made before calling this function.
 * @param operator_lst list of operators to be absorbed.
 */
NetOperatorPtr NetOperator::absorb(NetOperatorPtr operator_lst)
{
	if (this->next == 0) {
		next = operator_lst;
	} else next->absorb(operator_lst);
	return this;
}

/**
 * Removes any double operators from the list.
 *
 * Operator instances that are removed from the list are 
 * added to the trash list. The reduction takes place by
 * searching all subsequent operators in the list for 
 * identical operator ids.
 *
 * This method is called just before the sampling procedure
 * starts, but after absorb() is called.
 * @see #absorb()
 */
NetOperatorPtr NetOperator::reduce(NetOperatorPtr & trash)
{
	if (next != 0) next = next->reduce(trash);

	NetOperatorPtr op = next;
	bool found = false;

	while (op != 0 && !found) {
		found = (operator_id == op->operator_id);
		if (!found) op = op->next;
	}

	if (found) {
		op = next;
		next = trash;
		trash = this;
		return op;
	} else return this;
}

/**
 * Absorbs operators from other chains if necessary.
 * 
 * The method returns a pointer to the head of the operator
 * list after absorbtion.
 *
 * This method implements the default behavior, which is to 
 * call absorb() for the next operator in the list.
 * It must be overridden for model operators that
 * require that operators from other lists are added to this
 * list. 
 *
 * This is necessary for instance in the case where we model
 * that one failure rate is exactly twice as high as the other
 * which removes a degree of freedom from the model. In that
 * case, a list of the combined operators for the two models
 * must be created. Then, reduce() can be called to remove
 * double operators from the list.
 *
 * It is assumed that absorb() is called for variables with
 * higher index numbers first.
 *
 * This method is called just before the sampling starts, and
 * also before the chains are reduced by calling reduce().
 * @see #reduce(NetOperatorPtr)
 */
NetOperatorPtr NetOperator::absorb(NetOperatorPtr * operator_arr, int index)
{
	if (next != 0) next = next->absorb(operator_arr,index);
	return this;
}

/**
 * Sorts the operators in ascending order.
 * The order is based on the dependent variable, i.e., if the operator 
 * determines the value of another variable, then the index of that other
 * variable is the order of this operator. This way, the operators
 * are sorted such that dependent variables with lower indeces are set first.
 * This is necessary to guarantee that in case of cascaded dependencies all
 * variables are set correctly.
 * ORDER_LAST is an order constant that comes after all other operators.
 */
NetOperatorPtr NetOperator::reorder(void) 
{
	if (next != 0) next = next->reorder();
	int order = this->getOrder();
	if (next != 0 && order > next->getOrder()) {
		NetOperatorPtr op = this;
		while (op->next != 0 && order > op->next->getOrder())
		{
			op = op->next;
		}
		NetOperatorPtr nxt = op->next;
		op->next = this;
		op = this->next;
		this->next = nxt;
		return op;
	} else {
		return this;
	}
}
