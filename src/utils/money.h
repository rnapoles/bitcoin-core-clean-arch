#ifndef UTILS_MONEY_H
#define UTILS_MONEY_H

#include <string>
#include <types.h>

using namespace std;

string FormatMoney(int64 n, bool fPlus=false);
bool ParseMoney(const char* pszIn, int64& nRet);

#endif