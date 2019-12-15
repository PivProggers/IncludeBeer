#include "pch.h"
#include "Command.h"
using namespace std;

string SendFile::Run()
{
#ifdef _WIN32
	HANDLE h_in = CreateFile((LPCWSTR)_parameters.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (h_in == INVALID_HANDLE_VALUE)
	{
		_error_report = "0" + GetLastError();
		return "0";
	}

	BOOL i = FALSE;
	LPVOID buffer[BUF_SIZE] = { 0 };
	DWORD red = 0;
	string resultOfReading;
	do
	{
		i = ReadFile(h_in, buffer, BUF_SIZE, &red, NULL);
		if (i && red > 0)
			resultOfReading += (const char*)buffer;
	} while (i && red != 0);

	return resultOfReading;
#else

#endif

}