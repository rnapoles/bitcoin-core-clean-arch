#ifndef THREAD_SYNC_H
#define THREAD_SYNC_H

#include <windows.h>
#include <cassert>

#if defined(_MSC_VER) && _MSC_VER < 1300
#define for  if (false) ; else for
#endif

#ifndef _MSC_VER
#define __forceinline  inline
#endif

// Wrapper to automatically initialize critical section
// Could use wxCriticalSection for portability, but it doesn't support TryEnterCriticalSection
class CCriticalSection
{
protected:
    CRITICAL_SECTION cs;
public:
    char* pszFile;
    int nLine;
    explicit CCriticalSection() { InitializeCriticalSection(&cs); }
    ~CCriticalSection() { DeleteCriticalSection(&cs); }
    void Enter() { EnterCriticalSection(&cs); }
    void Leave() { LeaveCriticalSection(&cs); }
    bool TryEnter() { return TryEnterCriticalSection(&cs); }
    CRITICAL_SECTION* operator&() { return &cs; }
};

// Automatically leave critical section when leaving block, needed for exception safety
class CCriticalBlock
{
protected:
    CRITICAL_SECTION* pcs;
public:
    CCriticalBlock(CRITICAL_SECTION& csIn) { pcs = &csIn; EnterCriticalSection(pcs); }
    CCriticalBlock(CCriticalSection& csIn) { pcs = &csIn; EnterCriticalSection(pcs); }
    ~CCriticalBlock() { LeaveCriticalSection(pcs); }
};

// WARNING: This will catch continue and break!
// break is caught with an assertion, but there's no way to detect continue.
// I'd rather be careful than suffer the other more error prone syntax.
// The compiler will optimise away all this loop junk.
#define CRITICAL_BLOCK(cs)     \
    for (bool fcriticalblockonce=true; fcriticalblockonce; assert(("break caught by CRITICAL_BLOCK!", !fcriticalblockonce)), fcriticalblockonce=false)  \
    for (CCriticalBlock criticalblock(cs); fcriticalblockonce && (cs.pszFile=__FILE__, cs.nLine=__LINE__, true); fcriticalblockonce=false, cs.pszFile=NULL, cs.nLine=0)

class CTryCriticalBlock
{
protected:
    CRITICAL_SECTION* pcs;
public:
    CTryCriticalBlock(CRITICAL_SECTION& csIn) { pcs = (TryEnterCriticalSection(&csIn) ? &csIn : NULL); }
    CTryCriticalBlock(CCriticalSection& csIn) { pcs = (TryEnterCriticalSection(&csIn) ? &csIn : NULL); }
    ~CTryCriticalBlock() { if (pcs) LeaveCriticalSection(pcs); }
    bool Entered() { return pcs != NULL; }
};

#define TRY_CRITICAL_BLOCK(cs)     \
    for (bool fcriticalblockonce=true; fcriticalblockonce; assert(("break caught by TRY_CRITICAL_BLOCK!", !fcriticalblockonce)), fcriticalblockonce=false)  \
    for (CTryCriticalBlock criticalblock(cs); fcriticalblockonce && (fcriticalblockonce = criticalblock.Entered()) && (cs.pszFile=__FILE__, cs.nLine=__LINE__, true); fcriticalblockonce=false, cs.pszFile=NULL, cs.nLine=0)

// Randomize the stack to help protect against buffer overrun exploits
#define IMPLEMENT_RANDOMIZE_STACK(ThreadFn)                         \
    {                                                               \
        static char nLoops;                                         \
        if (nLoops <= 0)                                            \
            nLoops = GetRand(50) + 1;                               \
        if (nLoops-- > 1)                                           \
        {                                                           \
            ThreadFn;                                               \
            return;                                                 \
        }                                                           \
    }

#define CATCH_PRINT_EXCEPTION(pszFn)     \
    catch (std::exception& e) {          \
        PrintException(&e, (pszFn));     \
    } catch (...) {                      \
        PrintException(NULL, (pszFn));   \
    }


#endif