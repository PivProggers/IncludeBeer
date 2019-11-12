#pragma once
#include <string>
using namespace std;

#ifdef _WIN32
#define OS_WIN
#endif

class Command {

#ifndef OS_WIN
public:
	virtual string run();
#else
#endif

protected:
	string _name;
	string* _parameters;
	virtual string makeReport();
};

class RunAppliñation : Command {
	RunAppliñation(string name, string* parameters) {
		_name = name;
		_parameters = new string;
		//òóò êàê-òî çàïîëíÿşòÿ ïîëÿ èòä
		/*for (int i = 0; i < parameters->capacity; ++i)
			_parameters->push_back(static_cast<char*>(parameters[i]));*/
	}

	~RunAppliñation() {};

	#ifndef OS_WIN
		string run() {
			//òóò êàê-òî ıòî âûïîëíÿåòñÿ
			return makeReport();
		}
	#endif

	string makeReport() {};
};