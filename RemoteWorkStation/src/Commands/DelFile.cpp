#include "pch.h"
#include "Command.h"

string DelFile::Run()
{
	_error_report = (remove(_name.c_str()) != 0)
		? "failed"
		: "successfully deleted";
	return "0";
}
