// SyncMutex.cpp: implementation of the SyncMutex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SyncMutex.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define SYSTEM_MUTEX "bayes.dll.root.mutex"


SyncMutex::SyncMutex()
{
	_handle = CreateMutex(0L,FALSE,SYSTEM_MUTEX);
}

SyncMutex::SyncMutex(char * name)
{
	_handle = CreateMutex(0L,FALSE,name);
}

SyncMutex::~SyncMutex()
{
	CloseHandle(_handle);
}

bool SyncMutex::init()
{
	// no action
	return true;
}

void SyncMutex::cleanUp()
{
	// no action
}

bool SyncMutex::obtain()
{
	DWORD result = WAIT_FAILED;
	result = WaitForSingleObject(_handle,INFINITE);
#ifdef _DEBUG
	if (result == WAIT_FAILED) cout << "MUTEX OBTAIN FAILED: " << GetLastError() << endl;
	if (result == WAIT_TIMEOUT) cout << "MUTEX OBTAIN TIMEOUT" << endl;
#endif
	return result != WAIT_FAILED;
}

void SyncMutex::release()
{
	ReleaseMutex(_handle);
}
