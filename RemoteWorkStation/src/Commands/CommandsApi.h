#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifdef COMMANDS_EXPORTS
#define COMMANDS_API __declspec(dllexport)
#else
#define COMMANDS_API __declspec(dllimport)
#endif