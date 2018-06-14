// Status.cpp: implementation of the Status class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/


#include "Status.h"
#include <string.h>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Status::Status()
{
	_text[0] = 0;
	_progress = 0;
	_code = STATUS_IDLE;
	_cancelRequested = false;
}

Status::~Status()
{

}

void Status::setStatus(int progress, const char *text)
{
	// if (_mutex.obtain()) {
		_progress = progress;
		if (text != 0) {
			strncpy(_text, text, STATUS_TEXT_LENGTH);
		} else {
			_text[0] = 0;
		}
		// _mutex.release();
	// } else {
#ifdef _DEBUG
		cout << "STATUS MUTEX FAILED: SET STATUS" << endl;
#endif
	// }
}

void Status::setProgress(int progress)
{
	// if (_mutex.obtain()) {
		_progress = progress;
		// _mutex.release();
	// } else {
#ifdef _DEBUG
		cout << "STATUS MUTEX FAILED: SET PROGRESS" << endl;
#endif
	// }
}

void Status::getStatus(char *buffer, int maxChars)
{
	// if (_mutex.obtain()) {
		if (_text != 0L) {
			strncpy(buffer,_text,maxChars);
		} else {
			strncpy(buffer,"IDLE",maxChars);
		}
		// _mutex.release();
	// } else {
#ifdef _DEBUG
		cout << "STATUS MUTEX FAILED: GET STATUS" << endl;
#endif
	// }
}

int Status::getProgress()
{
	int progress = -1;

	// if (_mutex.obtain()) {
		progress = _progress;
		// _mutex.release();
	// } else {
#ifdef _DEBUG
		cout << "STATUS MUTEX FAILED: GET PROGRESS" << endl;
#endif
	// }

	return progress;
}

void Status::requestCancel()
{
	// if (_mutex.obtain()) {
		_cancelRequested = true;
		strncpy(_text, "Canceling...", STATUS_TEXT_LENGTH);
		// _mutex.release();
	// } else {
#ifdef _DEBUG
		cout << "STATUS MUTEX FAILED: REQUEST CANCEL" << endl;
#endif
	// }
}

bool Status::isCancelRequested()
{
	bool cancelRequested = false;

	// if (_mutex.obtain()) {
		cancelRequested = _cancelRequested;
		// _mutex.release();
	// } else {
#ifdef _DEBUG
		cout << "STATUS MUTEX FAILED: IS CANCEL REQUESTED" << endl;
#endif
	// }

	return cancelRequested;
}

void Status::clear()
{
	// if (_mutex.obtain()) {
		_text[0] = 0;
		_progress = 0;
		_cancelRequested = false;
		_code = STATUS_IDLE;
		// _mutex.release();
	// } else {
#ifdef _DEBUG
		cout << "STATUS MUTEX FAILED: CLEAR" << endl;
#endif
	// }
}

void Status::setStatusCode(int code)
{
	// if (_mutex.obtain()) {
		_code = code;
		// _mutex.release();
	// }
}

int Status::getStatusCode()
{
	int code = 0;
	// if (_mutex.obtain()) {
		code = _code;
		// _mutex.release();
	// }
	return code;
}
