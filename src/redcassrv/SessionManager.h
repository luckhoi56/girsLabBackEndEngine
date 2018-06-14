// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/SessionManager.h,v 1.3 2001/08/24 13:03:55 droguett Exp $

// SessionManager.h: interface for the SessionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_)
#define AFX_SESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SESSIONS 256

#include "Session.h"
#include "SyncMutex.h"

const int WEIBULL_SESSION  = 45;
const int NONHOMOG_SESSION = 46;

class SessionManager  
{
public: 
	SessionManager();

	virtual ~SessionManager();

	static SessionManager * getInstance();

	/* retrieves specified session. */
	Session * retrieveSession(int index);

	/* destroys a session. */
	void destroySession(int session);

	/* creates a new session. */
	int createSession(int type);

private:
	static SessionManager _instance;
	Session * _sessions[MAX_SESSIONS];
	SyncMutex _mutex;
};

#endif // !defined(AFX_SESSIONMANAGER_H__A3FC4F64_3697_47E1_A858_A82E309B6393__INCLUDED_)
