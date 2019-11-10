#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifdef TRANSPORT_EXPORTS
#define TRANSPORT_API __declspec(dllexport)
#else
#define TRANSPORT_API __declspec(dllimport)
#endif