// exe1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#define TIMEOUT 10000


int _tmain(int argc, _TCHAR* argv[])
{

	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(pi));


	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, _T("Event"));
	if (hEvent = NULL){
		_tprintf(_T("Can't create process %s (%d)\n"), _T("Event"), GetLastError());
		return -1;
	}


	BOOL rc = CreateProcess(_T("exe2.exe"), _T("exe2.exe"), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (!rc){
		_tprintf(_T("Can't create process %s (%d)\n"), _T("exe2.exe"), GetLastError());
		return -1;
	}


	_tprintf(_T("Wait process %d . . .\n"), pi.dwProcessId);
	DWORD ret = WaitForSingleObject(pi.hProcess, TIMEOUT);
	if (ret == WAIT_OBJECT_0){
		_tprintf(_T("BB\n"));
		return 0;
	}
	if (ret != WAIT_TIMEOUT){
		_tprintf(_T("Failed WaitForSingleObject (%d)\n"), GetLastError());
		return -1;
	}


	rc = SetEvent(hEvent);
	WaitForSingleObject(pi.hProcess, TIMEOUT);
	if (ret != WAIT_OBJECT_0){
		TerminateProcess(pi.hProcess, -1);
		return -1;
	}

	return 0;
}

