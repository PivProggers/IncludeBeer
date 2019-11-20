#include "pch.h"
#include "Command.h"


#define _CRT_SECURE_NO_WARNINGS
using namespace std;


Command::Command(string name, string parameters){
	_name = name;
	_parameters = parameters;

}

string Command::Makereport() {
	    
}
RunAppliñation::RunAppliñation(string name, string parameters) {
		_name = name;
		_parameters = parameters;
		
}

string RunAppliñation::Run() {
	if (_name.empty() || _parameters.empty()) {
		cout << "Initialize object of the class before" << endl;
		return "0";
	}
	char result[250];   // array to hold the result
	const char * space = " ";
	strcpy_s(result, "/K"); 
	strcat_s(result, space);
	strcat_s(result, _name.c_str());// copy string one into the result.
	strcat_s(result, space);
	strcat_s(result, _parameters.c_str());
	if (ShellExecuteA(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", result, 0, SW_SHOWNORMAL) > 32) {
		_error_report.clear();
		_error_report = "Succesfully opened file";
	}

}
