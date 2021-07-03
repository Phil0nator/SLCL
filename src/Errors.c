#include "SLCL/Errors.h"
#include <string.h>
#include <stdio.h>
// Internals

char slclerrmsg[1024];

void slcl_seterr( const char* msg )
{
    bzero(slclerrmsg, sizeof(slclerrmsg));
    strcpy( slclerrmsg, msg );
}

#ifdef __cpluscplus
SLCL_ENTERCPP
#endif


extern const char* slcl_strerr()
{
    return slclerrmsg;
}
extern void slcl_perror( const char* prefix )
{
    fprintf( stderr, "%s: %s\n", prefix, slclerrmsg );
}



#ifdef __cpluscplus
SLCL_EXITCPP
#endif


