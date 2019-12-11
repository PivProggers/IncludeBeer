#include "Command.h"

RunAppliñation::RunAppliñation(string name, string parameters) {
	_name = name;
	_parameters = parameters;
}

string RunAppliñation::Run() {
	if (_name.empty() || _parameters.empty()) {
		cout << "Initialize object of the class before" << endl;
		return "0";
	}
	string result = "/K ";
	result += _name.c_str(); 
	result += " ";
	result += _parameters.c_str();

	if (ShellExecuteA(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", result.c_str(), 0, SW_SHOWNORMAL) > (HINSTANCE)32) {
		_error_report.clear();
		_error_report = "Succesfully opened file";
	}

}
