// SessionManager.cpp: implementation of the SessionManager class.
// $Header
//////////////////////////////////////////////////////////////////////


#include "SessionManager.h"
#include "NHSession.h"

#ifdef _DEBUG
#include <iostream>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SessionManager SessionManager::_instance = SessionManager();

SessionManager::SessionManager()
{
	for (int i = 0 ; i < MAX_SESSIONS ; i++) {
		_sessions[i] = 0L;
	}
}

SessionManager::~SessionManager()
{
	for (int i = 0 ; i < MAX_SESSIONS ; i++) {
		destroySession(i);
	}
}

SessionManager * SessionManager::getInstance() {
	return &_instance;
}

Session * SessionManager::retrieveSession(int index)
{
	Session * session = 0L;

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

void SessionManager::destroySession(int session)
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

int SessionManager::createSession(int type)
{

	int i = -1;

	if (_mutex.obtain()) {

		i = 0;

		while (_sessions[i] != 0L && i < MAX_SESSIONS) {
			i++;
		}

		if (i < MAX_SESSIONS) {

			switch (type) {

			case NONHOMOG_SESSION :
				_sessions[i] = new NHSession();
				break;
			default :
				i = -1;
				break;
			}

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

