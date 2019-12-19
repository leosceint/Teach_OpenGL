#pragma once
#include <windows.h>
#include <time.h>
#include <assert.h>
#include <tchar.h>

#ifdef _EXPORTS
#define LIBIUP_API __declspec(dllexport)
#else
#define LIBIUP_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
	LIBIUP_API int LIBIUP_Main(int width, int height);
}
#endif