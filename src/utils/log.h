#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <exception>
#include <utils/string>

#ifdef _WINDOWS
#define printf              OutputDebugStringF
#endif

#ifdef snprintf
#undef snprintf
#endif
#define snprintf my_snprintf

bool error(const char* format, ...);
void PrintException(std::exception* pex, const char* pszThread);

#endif