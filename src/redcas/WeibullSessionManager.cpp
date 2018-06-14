// WeibullSessionManager.cpp: implementation of the WeibullSessionManager class.
// $Header
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeibullSessionManager.h"

#ifdef _DEBUG
#include <iostream>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeibullSessionManager WeibullSessionManager::_instance = WeibullSessionManager();

WeibullSessionManager::WeibullSessionManager()
{
	for (int i = 0 ; i < MAX_SESSIONS ; i++) {
		_sessions[i] = 0L;
	}
}

WeibullSessionManager::~WeibullSessionManager()
{
	for (int i = 0 ; i < MAX_SESSIONS ; i++) {
		destroySession(i);
	}
}

WeibullSessionManager * WeibullSessionManager::getInstance() {
	return &_instance;
}

BRASSEngine * WeibullSessionManager::retrieveSession(int index)
{
	BRASSEngine * session = 0L;

	if (_mutex.obtain()) {

		if (index > -1 && index < MAX_SESSIONS) {
			session = _sessions[index];      
		}

		_mutex.release();

	} else {
#ifdef _DEBUG
		cout << "SESSIONMGR MUTEX FAILED: RETRIEVE" << endl;
#endif
	}

	return session;

}

void WeibullSessionManager::destroySession(int session)
{
	if (_mutex.obtain()) {
		delete _sessions[session];
		_sessions[session] = 0L;
		_mutex.release();
	} else {
#ifdef _DEBUG
		cout << "SESSIONMGR MUTEX FAILED" << endl;
#endif
	}
}

int WeibullSessionManager::createSession()
{
	int i = -1;

	if (_mutex.obtain()) {

		i = 0;

		while (_sessions[i] != 0L && i < MAX_SESSIONS) {
			i++;
		}

		if (i < MAX_SESSIONS) {
			_sessions[i] = new BRASSEngine();
		} else {
			i = -1;
		}

		_mutex.release();

	} else {
#ifdef _DEBUG
		cout << "SESSIONMGR MUTEX FAILED" << endl;
#endif
	}

	return i;
}

