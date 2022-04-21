#include <cstdio>
#include <cstdarg>
#include <windows.h>
#include <utils/log.h>

bool error(const char* format, ...)
{
    char buffer[50000];
    int limit = sizeof(buffer);
    va_list arg_ptr;
    va_start(arg_ptr, format);
    int ret = _vsnprintf(buffer, limit, format, arg_ptr);
    va_end(arg_ptr);
    if (ret < 0 || ret >= limit)
    {
        ret = limit - 1;
        buffer[limit-1] = 0;
    }
    printf("ERROR: %s\n", buffer);
    return false;
}

void PrintException(std::exception* pex, const char* pszThread)
{
    char pszModule[260];
    pszModule[0] = '\0';
    GetModuleFileName(NULL, pszModule, sizeof(pszModule));
    _strlwr(pszModule);

    char pszMessage[1000];
    if (pex)
        snprintf(pszMessage, sizeof(pszMessage),
            "EXCEPTION: %s       \n%s       \n%s in %s       \n", typeid(*pex).name(), pex->what(), pszModule, pszThread);
    else
        snprintf(pszMessage, sizeof(pszMessage),
            "UNKNOWN EXCEPTION       \n%s in %s       \n", pszModule, pszThread);
    printf("\n\n************************\n%s", pszMessage);

#if wxUSE_GUI
    if (wxTheApp){
        wxMessageBox(pszMessage, "Error", wxOK | wxICON_ERROR);
    }
#endif
    throw;
    //DebugBreak();
}

inline int OutputDebugStringF(const char* pszFormat, ...)
{
#ifdef __WXDEBUG__
    // log file
    FILE* fileout = fopen("debug.log", "a");
    if (fileout)
    {
        va_list arg_ptr;
        va_start(arg_ptr, pszFormat);
        vfprintf(fileout, pszFormat, arg_ptr);
        va_end(arg_ptr);
        fclose(fileout);
    }

    // accumulate a line at a time
    static CCriticalSection cs_OutputDebugStringF;
    CRITICAL_BLOCK(cs_OutputDebugStringF)
    {
        static char pszBuffer[50000];
        static char* pend;
        if (pend == NULL)
            pend = pszBuffer;
        va_list arg_ptr;
        va_start(arg_ptr, pszFormat);
        int limit = END(pszBuffer) - pend - 2;
        int ret = _vsnprintf(pend, limit, pszFormat, arg_ptr);
        va_end(arg_ptr);
        if (ret < 0 || ret >= limit)
        {
            pend = END(pszBuffer) - 2;
            *pend++ = '\n';
        }
        else
            pend += ret;
        *pend = '\0';
        char* p1 = pszBuffer;
        char* p2;
        while (p2 = strchr(p1, '\n'))
        {
            p2++;
            char c = *p2;
            *p2 = '\0';
            OutputDebugString(p1);
            *p2 = c;
            p1 = p2;
        }
        if (p1 != pszBuffer)
            memmove(pszBuffer, p1, pend - p1 + 1);
        pend -= (p1 - pszBuffer);
        return ret;
    }
#endif

#if wxUSE_GUI
    if (!wxTheApp)
    {
        // print to console
        va_list arg_ptr;
        va_start(arg_ptr, pszFormat);
        vprintf(pszFormat, arg_ptr);
        va_end(arg_ptr);
    }
#endif

    return 0;
}