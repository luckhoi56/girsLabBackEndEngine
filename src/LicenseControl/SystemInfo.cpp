// SystemInfo.cpp: implementation of the SystemInfo class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SystemInfo.h"
#include <VersionHelpers.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define BUFSIZE 80

SystemInfo::SystemInfo()
{
}

SystemInfo::~SystemInfo()
{

}


string SystemInfo::GetWinVerStr()
{
	if (IsWindows8Point1OrGreater()) {
		return "Windows 8.1";
	}
	else if (IsWindows8OrGreater())
	{
		return "Windows 8";
	}
	else if (IsWindows7SP1OrGreater())
	{
		return "Windows 7 SP1";
	}
	else if (IsWindows7OrGreater())
	{
		return "Windows 7";
	}
	else if (IsWindowsVistaSP2OrGreater())
	{
		return "Windows Vista SP2";
	}
	else if (IsWindowsVistaSP1OrGreater())
	{
		return "Windows Vista SP1";
	}
	else if (IsWindowsVistaOrGreater())
	{
		return "Windows Vista";
	}
	else if (IsWindowsXPSP3OrGreater())
	{
		return "Windows XP SP3";
	}
	else if (IsWindowsXPSP2OrGreater())
	{
		return "Windows XP SP2";
	}
	else if (IsWindowsXPSP1OrGreater())
	{
		return "Windows XP SP1";
	}
	else if (IsWindowsXPOrGreater())
	{
		return "Windows XP";
	}

    return "Unknown";
}