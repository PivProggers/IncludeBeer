#pragma once
#include <string>
#include "pch.h"
#include <windows.h>
#include "ShellAPI.h"
#include "CommandsApi.h"
using namespace std;

#ifdef _WIN32
#define OS_WIN
#endif

class Command {

#ifndef OS_WIN
public:
	COMMANDS_API virtual string run();
#else
#endif

protected:
	const char* _name;
	const char* _parameters;
	//COMMANDS_API virtual string makeReport();
};

class RunAppliñation : public Command {
public:
	COMMANDS_API RunAppliñation(const char* name, const char* parameters);
	COMMANDS_API ~RunAppliñation() {};

	#ifndef OS_WIN
	COMMANDS_API string run() {
			//òóò êàê-òî ıòî âûïîëíÿåòñÿ
			return makeReport();
		}
	#endif

	//COMMANDS_API string makeReport() {};
};