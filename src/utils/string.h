#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <string>
#include <vector>

using namespace std;

void ParseString(const string& str, char c, vector<string>& v);

// Safer snprintf
//  - prints up to limit-1 characters
//  - output string is always null terminated even if limit reached
//  - return value is the number of characters actually printed
int my_snprintf(char* buffer, size_t limit, const char* format, ...);

string strprintf(const char* format, ...);

template<typename T>
string HexNumStr(const T itbegin, const T itend, bool f0x=true){
    const unsigned char* pbegin = (const unsigned char*)&itbegin[0];
    const unsigned char* pend = pbegin + (itend - itbegin) * sizeof(itbegin[0]);
    string str = (f0x ? "0x" : "");
    for (const unsigned char* p = pend-1; p >= pbegin; p--)
        str += strprintf("%02X", *p);
    return str;
}

template<typename T>
string HexStr(const T itbegin, const T itend, bool fSpaces=true)
{
    const unsigned char* pbegin = (const unsigned char*)&itbegin[0];
    const unsigned char* pend = pbegin + (itend - itbegin) * sizeof(itbegin[0]);
    string str;
    for (const unsigned char* p = pbegin; p != pend; p++)
        str += strprintf((fSpaces && p != pend-1 ? "%02x " : "%02x"), *p);
    return str;
}

template<typename T>
void PrintHex(const T pbegin, const T pend, const char* pszFormat, bool fSpaces)
{
    printf(pszFormat, HexStr(pbegin, pend, fSpaces).c_str());
}

#endif