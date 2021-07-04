#include "SLCL/Errors.h"
#include <string.h>
#include <stdio.h>
// Internals

char slclerrmsg[1024];

void slclSeterr( const char* msg )
{
    memset(slclerrmsg, 0, sizeof(slclerrmsg));
    strcpy( slclerrmsg, msg );
}
char* slclGetErrorBuf()
{
    return slclerrmsg;
}

#ifdef __cpluscplus
SLCL_ENTERCPP
#endif


const char* slclStrerr()
{
    return slclerrmsg;
}
void slclPerror( const char* prefix )
{
    fprintf( stderr, "%s: %s\n", prefix, slclerrmsg );
}



#ifdef __cpluscplus
SLCL_EXITCPP
#endif


