#pragma once

#ifdef LET16DLLANS_EXPORTS

#define LET16DLLANS_API __declspec(dllexport)
#else
#define LET16DLLANS_API __declspec(dllimport)
#endif

LET16DLLANS_API int TestFunc(int);


