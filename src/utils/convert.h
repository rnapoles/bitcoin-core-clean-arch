#ifndef UTILS_CONVERT_H
#define UTILS_CONVERT_H

#include <string>
#include <types.h>

using namespace std;

inline string i64tostr(int64 n);
inline string itostr(int n);
inline int64 atoi64(const char* psz);
int64 atoi64(const string& str);
inline int atoi(const string& str);

#endif