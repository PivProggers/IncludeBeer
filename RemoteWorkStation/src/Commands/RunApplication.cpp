#include "pch.h"
#include "Command.h"

RunApplication::RunApplication(string name, string parameters) {
	_name = name;
	_parameters = parameters;
}

string RunApplication::Run() {
	if (_name.empty()) {
		_error_report = "Name of application wasn't initialized...";
		return "-1";
	}
	string result;

#ifdef _WIN32
	result = "/K ";
#endif

	result += _name.c_str(); 
	result += " ";
	result += _parameters.c_str();

#ifdef _WIN32
	if (ShellExecuteA(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", result.c_str(), 0, SW_SHOWNORMAL) > (HINSTANCE)32) {
		_error_report.clear();
		_error_report = "Succesfully runned file";
	}
	else
		_error_report = "Failed with running file";
#else
	if (system(result) != 0)
		_error_report = "Succesfully runned file";
	else
		_error_report = "Failed with running file";
#endif

	return "0";
}
