#pragma once
#include <string>
#include "pch.h"
#include <windows.h>
#include "ShellAPI.h"
#include "CommandsApi.h"
//#include "..\..\cereal-1.3.0\include\cereal\cereal.hpp"
//#include "..\..\cereal-1.3.0\include\cereal\access.hpp"
//#include "..\..\cereal-1.3.0\include\cereal\macros.hpp"
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

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(_name, _parameters); // serialize things by passing them to the archive
	}

public:
	string _name;
	string _parameters;
	//COMMANDS_API virtual string makeReport();
};

//class RunAppliñation : public Command {
//public:
//	COMMANDS_API RunAppliñation(const char* name, const char* parameters);
////	COMMANDS_API RunAppliñation(class Archive & ar);
//	COMMANDS_API ~RunAppliñation() {};
//
//		
//
//	#ifndef OS_WIN
//	COMMANDS_API string run() {
//			//òóò êàê-òî ýòî âûïîëíÿåòñÿ
//			return makeReport();
//		}
//	#endif
//
//	//COMMANDS_API string makeReport() {};
//};