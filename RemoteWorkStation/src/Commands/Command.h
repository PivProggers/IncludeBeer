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

class RunAppliation : Command {
	RunAppliation(string name, string* parameters) {
		_name = name;
		_parameters = new string;
		//тут как-то заполн€ют€ пол€ итд
		/*for (int i = 0; i < parameters->capacity; ++i)
			_parameters->push_back(static_cast<char*>(parameters[i]));*/
	}

	~RunAppliation();

	string run() {
		//тут как-то это выполн€етс€
		return makeReport();
	}
	
	string makeReport() {
	
	};
};