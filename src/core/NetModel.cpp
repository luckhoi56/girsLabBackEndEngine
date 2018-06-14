// $Id: NetModel.cpp 275 2005-03-01 03:53:40Z fgroen $

#include "netmodel.h"

NetModel::NetModel(void)
{
}

NetModel::~NetModel(void)
{
//	delete next;
}

////////////////////////////////////////////////////////////////////

IdentityModel::IdentityModel(const int index1, const int index2)
{
	index_to = index1;
	index_from = index2;
}

/**
 * Initializes the model link.
 * An important aspect of the init operation is for the 
 * model to determine in which 'direction' it will operate.
 * The function returns the index of the variable that will 
 * be set by the model (the dependent variable), or -1 if 
 * the input variable index does not determine in which direction
 * the link is evaluated.
 * @param var_index variable whose change in value triggered this call.
 */
int IdentityModel::init(const int var_index)
{
	if (direction = DIR_UNDECIDED) {

		if (var_index == index_to) {
			index_to = index_from;
			index_from = var_index;
		}

		if (var_index == index_from) {
			direction = DIR_DECIDED;
			return index_to;
		} else return -1;

	} else return -1;
}

double IdentityModel::update(float * vals, int & next_var)
{
	if (direction == DIR_UNDECIDED) return -1;
	vals[index_to] = vals[index_from];
	next_var = index_to;
	return 0;
}

/**
 * Returns true if the model depends on the specified variable.
 */
bool IdentityModel::appliesTo(const int var_index) const
{
	return (var_index == index_from) || (var_index == index_to);
}

/**
 * Returns true if the model considers the variable as its dependent variable.
 */
bool IdentityModel::specifies(const int var_index) const
{
	return (direction == DIR_DECIDED) && (var_index == index_to);
}

/**
 * Returns the index of the variable specified by this model.
 */
int IdentityModel::getSpecified(void) const
{
	if (direction == DIR_UNDECIDED) return -1;
	return index_to;
}

NetModel * IdentityModel::copy() const
{
	return new IdentityModel(index_to,index_from);
}

void IdentityModel::print(ostream & os) const
{
	os << "(" << index_from << " => " << index_to << ")";
}

//////////////////////////////////////////////////////////////////

/**
 * Destructor.
 * If necessary, deletes the referred model.
 * Recursively deletes other references in the list.
 */
NetModelRef::~NetModelRef(void) 
{
	setModel(0); 
	delete next;
}

/**
 * Returns a copy of the reference list headed by this reference.
 */
NetModelRefPtr NetModelRef::copy(void) const
{
	NetModelRefPtr cpy = new NetModelRef();
	cpy->setModel(model);
	if (next != 0) cpy->setNext(next->copy());
	return cpy;
}

/**
 * Returns true if the list contains a reference to the model.
 */
bool NetModelRef::contains(const NetModel * model) const
{
	const NetModelRef * ref = this;
	while (ref != 0) {
		if (ref->model == model) return true;
		ref = ref->next;
	}
	return false;
}

/**
 * Sets the model for the reference.
 * Previous references are removed, and the old model is
 * destroyed if necessary.
 */
void NetModelRef::setModel(NetModel * md) 
{
	if (model != 0 && model->release()) {
		delete model;
	}
	model = md;
	if (model != 0) {
		model->obtain();
	}
}

/**
 * Stream operator for parameter arrays.
 */
ostream & operator<<(ostream & os, const NetModel & m)
{
	m.print(os);
	return os;
}