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

	string Run();
	string Makereport();
protected:
	string _name;
	string _parameters;
	string _error_report = "0";
	//COMMANDS_API virtual string makeReport();
};


//����� ��������� ������ �������� �� ������ � ���������� ���
//��� ��� �... ����� ������ ��������� � ���� ������ ���� Command ����� ���� ��� �� ��������������, � ��������� ���. 
//��� ����� ������ � .cpp �����, ��� �������������� ���� ���� �������� � ���������


class RunAppli�ation : public Command {
public:
	COMMANDS_API RunAppli�ation(string name, string parameters);
	COMMANDS_API ~RunAppli�ation() {};
	COMMANDS_API string Run();


	//COMMANDS_API string makeReport() {};
};

class DelAppli�ation : public Command {
public:
	COMMANDS_API DelAppli�ation(string name, string parameters);
	COMMANDS_API ~DelAppli�ation() {};
	COMMANDS_API string Run();


	//COMMANDS_API string makeReport() {};
};

class SendFile : public Command {
public:
	COMMANDS_API SendFile(string name, string parameters);
	COMMANDS_API ~SendFile() {};
	COMMANDS_API string Run();
	//COMMANDS_API string makeReport() {};
};