#include "SLCL/Signals.h"
#include "InternalErrors.h"
#include <string.h>
#include <errno.h>

#ifdef __cpluscplus
SLCL_ENTERCPP
#endif

#if defined(SLCL_TARGET_WIN)
#include <windows.h>

#elif defined(SLCL_TARGET_UNIXBASED)
#include <sys/mman.h>

#endif


#ifdef __cpluscplus
SLCL_EXITCPP
#endif