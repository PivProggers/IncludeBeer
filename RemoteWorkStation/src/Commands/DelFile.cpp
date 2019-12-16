#include "pch.h"
#include "Command.h"

//DelFile::DelFile(string name, string parameters) {}


string DelFile::Run()
{
	return	_error_report = (remove(_parameters.c_str()) != 0)
		? "0" + GetLastError()
		: "successfully deleted";
}
