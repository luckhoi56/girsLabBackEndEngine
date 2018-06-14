// $Id: BRASSError.h 711 2005-06-06 15:56:42Z marcelo $

#pragma once

#include <string>
#include <list>

using namespace std;

/** This class defines error codes for the BRASS engine.
 */
class BRASSError
{
public:
	BRASSError(void);
	~BRASSError(void);

	/// General error.
	static const int ERR_GENERAL = -1;

	/// validation error: loop encountered.
	static const int ERR_LOOP_IN_MODEL = -1100;
	/// validation error: variable without density operator.
	static const int ERR_NO_OPERATOR = -1101;

	/// invalid failure mode .
	static const int ERR_INVALID_FM = -100;
	/// invalid node.
	static const int ERR_INVALID_NODE = -101;
	/// invalid upper or lower bound.
	static const int ERR_INVALID_BOUNDS = -102;
	/// invalid estimate or errror factor.
	static const int ERR_INVALID_ESTIMATE = -103;
	/// index out of bounds
	static const int ERR_OUT_OF_BOUNDS = -104;

	/// invalid parameters supplied to setup procedure.
	static const int ERR_INVALID_SETUP = -201;
	/// invalid parameters supplied for an acceleration factor.
	static const int ERR_INVALID_ACCELERATION = -202;
	/// error encountered while specifying test data.
	static const int ERR_INVALID_TESTDATA = -203;
	/// error encountered while specifying adjustment model.
	static const int ERR_INVALID_ADJUSTMENT = -204;
	/// error encountered while specifying prior information.
	static const int ERR_INVALID_PRIOR = -205;
	/// no samples in the engine / no analyis results.
	static const int ERR_NO_SAMPLES = -206;
	/// invalid input for measure computation.
	static const int ERR_INVALID_MEASURE = -207;
	/// no joint density available.
	static const int ERR_NO_JOINTDENSITY = -208;

	/// poor convergence of the MCMC process detected.
	static const int ERR_POOR_CONVERGENCE = -209;
	/// posterior density is not proper (sampler runaway).
	static const int ERR_IMPROPER_POSTERIOR = -210;

	/// abort the simulation.
	static const int CANCELED = -300;

};

/**
 * Entry in list of messages.
 */
struct MsgListEntry
{
	string msg;
};

/**
 * List of messages.
 */
class MsgList
{
public:
	MsgList() {}
	~MsgList() {}
	int add(const string & msg);
	int size(void) const;
	void clear();
	int next(string & msg);
private:
	list<MsgListEntry> msg_lst;
};