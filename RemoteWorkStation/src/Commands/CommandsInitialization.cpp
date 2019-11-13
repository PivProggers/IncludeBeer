#include "pch.h"
#include "Command.h"
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS
using namespace std;


Command::Command(string name, string parameters){

	_name = name;
	_parameters = parameters;

}

RunAppliñation::RunAppliñation(string name, string parameters) {
		_name = name;
		_parameters = parameters;
		char result[250];   // array to hold the result.
		const char * prob = " ";
		strcpy_s(result, "/K"); 
		strcat_s(result, prob);
		strcat_s(result, _name.c_str());// copy string one into the result.
		strcat_s(result, prob);
		strcat_s(result, _parameters.c_str());
		//òóò êàê-òî çàïîëíÿşòÿ ïîëÿ èòä
		/*for (int i = 0; i < parameters->capacity; ++i)
		_parameters->push_back(static_cast<char*>(parameters[i]));*/
		
		ShellExecuteA(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", result, 0, SW_SHOWNORMAL);
}

