#include "SLCL/Errors.h"
#include "InternalErrors.h"
#include <string.h>
#include <stdio.h>
// Internals

#ifdef __cpluscplus
SLCL_ENTERCPP
#define SLCL_OPTIONAL_THREADSAFETY_KEYWORD thread_local
#elif __STDC_VERSION__ >= 201112L
#include <threads.h>
#define SLCL_OPTIONAL_THREADSAFETY_KEYWORD thread_local
#else
#define SLCL_OPTIONAL_THREADSAFETY_KEYWORD
#endif

SLCL_OPTIONAL_THREADSAFETY_KEYWORD char slclerrmsg[1024];

void slclSeterr( const char* msg )
{
    memset(slclerrmsg, 0, sizeof(slclerrmsg));
    strcpy( slclerrmsg, msg );
}
char* slclGetErrorBuf()
{
    return slclerrmsg;
}



const char* slclStrerr()
{
    return slclerrmsg;
}
void slclPerror( const char* prefix )
{
    fprintf( stderr, "%s: %s\n", prefix, slclerrmsg );
}


#ifdef SLCL_TARGET_WIN
#include <windows.h>
void slclSetLastWinErr()
{
    memset(slclGetErrorBuf(), 0, SLCL_ERRORBUF_SIZE);
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        slclGetErrorBuf(),
        SLCL_ERRORBUF_SIZE,
        NULL);
}


#else
void slclSetLastWinErr(){}
#endif



#ifdef __cpluscplus
SLCL_EXITCPP
#endif


