#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include <cstdio>

bool FileExists(const char* psz);
int GetFilesize(FILE* file);

#endif