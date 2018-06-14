// SyncMutex.h: interface for the SyncMutex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCMUTEX_H__9931F62B_BCAE_45C3_9720_F2F7C0476782__INCLUDED_)
#define AFX_SYNCMUTEX_H__9931F62B_BCAE_45C3_9720_F2F7C0476782__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

/**
 * Mutex object. This object hides the mutex synchronization mechanism 
 * by the operating system. A mutex can be used to provide multiple threads
 * or processes mutually exclusive access to a method.
 *
 * By calling the default constructor, a mutex with default name is constructed.
 * Note that all mutexes created with the same (default) name should be 
 * considered to be the same mutex.
 */
class SyncMutex  
{
public:
	void release();
	bool obtain();
	static void cleanUp();
	static bool init();
	SyncMutex();
	SyncMutex(char * name);
	virtual ~SyncMutex();
private:
	HANDLE _handle;
};

#endif // !defined(AFX_SYNCMUTEX_H__9931F62B_BCAE_45C3_9720_F2F7C0476782__INCLUDED_)
