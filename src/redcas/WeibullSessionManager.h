// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/SessionManager.h,v 1.3 2001/08/24 13:03:55 droguett Exp $

// SessionManager.h: interface for the SessionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEIBULLSESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_)
#define AFX_WEIBULLSESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SESSIONS 256

#include "SyncMutex.h"
#include "BRASSEngine.h"

class WeibullSessionManager  
{
public: 
	WeibullSessionManager();

	virtual ~WeibullSessionManager();

	static WeibullSessionManager * getInstance();

	/* retrieves specified session. */
	BRASSEngine * retrieveSession(int index);

	/* destroys a session. */
	void destroySession(int session);

	/* creates a new session. */
	int createSession();

private:
	static WeibullSessionManager _instance;
	BRASSEngine * _sessions[MAX_SESSIONS];
	SyncMutex _mutex;
};

#endif // !defined(AFX_WEIBULLSESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_)
