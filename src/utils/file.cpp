#ifdef WIN32
#include <windows.h>
#else
#include <io.h>
#endif
#include <utils/file.h>
#include <cstdio>

bool FileExists(const char* psz)
{
#ifdef WIN32
    return GetFileAttributes(psz) != -1;
#else
    return access(psz, 0) != -1;
#endif
}

int GetFilesize(FILE* file)
{
    int nSavePos = ftell(file);
    int nFilesize = -1;
    if (fseek(file, 0, SEEK_END) == 0)
        nFilesize = ftell(file);
    fseek(file, nSavePos, SEEK_SET);
    return nFilesize;
}