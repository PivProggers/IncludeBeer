#pragma once
#include <string>
#include "pch.h"
#include <windows.h>
#include "ShellAPI.h"
#include "CommandsApi.h"
#include "cereal\archives\xml.hpp"
#include "cereal\types\vector.hpp"


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
public:
	COMMANDS_API Command(string name, string parameters);
	COMMANDS_API Command() {};

	COMMANDS_API string GetName() { return this->_name; }
	COMMANDS_API string GetParameters() { return this->_parameters; }
	COMMANDS_API void SetName(string name) { this->_name = name; }
	COMMANDS_API void GetParameters(string parameters) { this->_parameters = parameters; }

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(_name, _parameters); // serialize things by passing them to the archive
	}

protected:
	string _name;
	string _parameters;
	//COMMANDS_API virtual string makeReport();
};


//Нужно придумать логику передачи на сервер и исполнения там
//Вот это г... снизу должно принимать в себя объект типа Command после того как он десериализован, и выполнять все. 
//Для винды версия в .cpp лежит, там соответственно тоже надо поменять и придумать


class RunAppliсation : public Command {
public:
	COMMANDS_API RunAppliсation(string name, string parameters);
	COMMANDS_API ~RunAppliсation() {};

		

	#ifndef OS_WIN
	COMMANDS_API string run() {
			//тут как-то это выполняется
			return makeReport();
		}
	#endif

	//COMMANDS_API string makeReport() {};
};