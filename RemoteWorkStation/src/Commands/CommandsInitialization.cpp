#include "pch.h"
#include "Command.h"
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS
using namespace std;


Command::Command(string name, string parameters){

	_name = name;
	_parameters = parameters;

}

//RunAppli�ation::RunAppli�ation(const char* name, const char* parameters) {
//	_name = name;
//		_parameters = parameters;
//		char result[250];   // array to hold the result.
//		const char * prob = " ";
//		strcpy_s(result, "/K"); 
//		strcat_s(result, prob);
//		strcat_s(result, _name);// copy string one into the result.
//		strcat_s(result, prob);
//		strcat_s(result, _parameters);
//		cout << result << endl;
//		//��� ���-�� ���������� ���� ���
//		/*for (int i = 0; i < parameters->capacity; ++i)
//		_parameters->push_back(static_cast<char*>(parameters[i]));*/
//		
//		ShellExecuteA(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", result, 0, SW_SHOWNORMAL);
//}
//
