#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <exception>

#ifdef _WINDOWS
//#define printf              OutputDebugStringF
#endif

#ifdef snprintf
#undef snprintf
#endif
#define snprintf my_snprintf

bool error(const char* format, ...);
void PrintException(std::exception* pex, const char* pszThread);
//int OutputDebugStringF(const char* pszFormat, ...)

#endif