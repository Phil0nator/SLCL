#ifndef SLCL_ERRINC
#define SLCL_ERRINC

#include "Config.h"


#ifdef __cplusplus
SLCL_ENTERCPP
#endif

extern const char* slclStrerr();
extern void slclPerror( const char* prefix );

#ifdef __cplusplus
SLCL_EXITCPP
#endif


#endif