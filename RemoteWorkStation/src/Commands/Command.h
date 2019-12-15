#pragma once
#include "pch.h"
#include "CommandsApi.h"
#include "cereal\archives\xml.hpp"
#include "cereal\types\vector.hpp"
#include "ShellAPI.h"

using namespace std;

#ifdef _WIN32
#define OS_WIN
#endif

class Command {
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

	COMMANDS_API virtual string Run() { return "0"; };
protected:
	string _name;
	string _parameters;
	string _error_report = "0";	// 1/0 - success	/.../-code of error
};



class RunApplication : public Command {
public:
	COMMANDS_API RunApplication(string name, string parameters) ;
	COMMANDS_API ~RunApplication() {};
	COMMANDS_API string Run();


	//COMMANDS_API string makeReport() {};
};

class DelFile : public Command {
public:
	COMMANDS_API DelFile(string name, string parameters) {};
	COMMANDS_API ~DelFile() {};
	COMMANDS_API string Run();


	//COMMANDS_API string makeReport() {};
};

class SendFile : protected Command {
public:
	COMMANDS_API SendFile(string name, string parameters) {};
	COMMANDS_API ~SendFile() {};
	COMMANDS_API string Run();
	//COMMANDS_API string makeReport() {};
};

//class RecieveFile : public Command {
//public:
//	COMMANDS_API RecieveFile(string name, string parameters) {};
//	COMMANDS_API ~RecieveFile() {};
//	COMMANDS_API string Run();
//	//COMMANDS_API string Run();
//	//COMMANDS_API string makeReport() {};
//};