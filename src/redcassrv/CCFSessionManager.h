#if !defined(AFX_CCFSESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_)
#define AFX_CCFSESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SESSIONS 256

#include "SyncMutex.h"

class CCFSessionManager
{
public:
	CCFSessionManager();

	virtual ~CCFSessionManager();

	static CCFSessionManager * getInstance();

	/* retrieves specified session. */
	CCFEngine * retrieveSession(int index);

	/* destroys a session. */
	void destroySession(int session);

	/* creates a new session. */
	int createSession();

private:
	static CCFSessionManager _instance;
	CCFEngine * _sessions[MAX_SESSIONS];
	SyncMutex _mutex;
};

#endif // !defined(AFX_WEIBULLSESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_)
