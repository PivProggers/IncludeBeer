#include "pch.h"
#include "Command.h"


#define _CRT_SECURE_NO_WARNINGS
using namespace std;


Command::Command(string name, string parameters){
	_name = name;
	_parameters = parameters;

}

string Command::MakeReport() {
	return _error_report + "\n";
}
