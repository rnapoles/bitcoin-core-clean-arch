#ifndef BLOCKFILE_H
#define BLOCKFILE_H

#include <cstdio>

FILE* OpenBlockFile(unsigned int nFile, unsigned int nBlockPos, const char* pszMode="rb");
FILE* AppendBlockFile(unsigned int& nFileRet);

#endif