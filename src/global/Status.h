// Status.h: interface for the Status class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUS_H__E5645137_453A_11D4_988C_002018557056__INCLUDED_)
#define AFX_STATUS_H__E5645137_453A_11D4_988C_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "SyncMutex.h"

#define STATUS_TEXT_LENGTH 80

#define STATUS_OK			0	// action was completed
#define STATUS_CANCELED		1	// action was canceled
#define STATUS_ACTIVE		2	// action in progress
#define STATUS_IDLE			0	// no current action
#define STATUS_INVALID_DATA 90	// data supplied to action invalid
#define STATUS_IO_ERROR		91	// IO error occurred
#define STATUS_INSUFFICIENT_DATA 92 // there is not sufficient data for a stable analysis
#define STATUS_ERROR		100 // action failed

class Status  
{
public:
	virtual int getStatusCode();
	virtual void setStatusCode(int code);
	virtual void clear();
	virtual bool isCancelRequested();
	virtual void requestCancel();
	virtual int getProgress();
	virtual void getStatus(char * buffer, int maxChars);
	virtual void setStatus(int progress, const char * text);
	virtual void setProgress(int progress);
	Status();
	virtual ~Status();
private:
	int _code;
	int _progress;
	char _text[STATUS_TEXT_LENGTH + 1];
	bool _cancelRequested;
	// SyncMutex _mutex;
};

typedef Status * StatusPtr;

#endif // !defined(AFX_STATUS_H__E5645137_453A_11D4_988C_002018557056__INCLUDED_)
