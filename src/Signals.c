#include "SLCL/Signals.h"
#include "InternalErrors.h"
#include <string.h>
#include <errno.h>

#ifdef __cpluscplus
SLCL_ENTERCPP
#endif

#if defined(SLCL_TARGET_WIN)
#include <signal.h>

const signum_t SLCL_SIGABRT = SIGABRT;
const signum_t SLCL_SIGFPE = SIGFPE;
const signum_t SLCL_SIGILL = SIGILL;
const signum_t SLCL_SIGINT = SIGINT;
const signum_t SLCL_SIGSEGV = SIGSEGV;
const signum_t SLCL_SIGTERM = SIGTERM;


sigaction_t slclSigaction( signum_t sig, sigaction_t action)
{

    sigaction_t oldhandle = signal( sig, action );
    if (oldhandle == SIG_ERR)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    return oldhandle;

}


#elif defined(SLCL_TARGET_UNIXBASED)
#include <signal.h>

const signum_t SLCL_SIGABRT = SIGABRT;
const signum_t SLCL_SIGFPE = SIGFPE;
const signum_t SLCL_SIGILL = SIGILL;
const signum_t SLCL_SIGINT = SIGINT;
const signum_t SLCL_SIGSEGV = SIGSEGV;
const signum_t SLCL_SIGTERM = SIGTERM;

typedef void(*unix_sigaction_t)(int);


sigaction_t slclSigaction( signum_t sig, sigaction_t action)
{
    __sighandler_t oldhandle = signal( sig, (unix_sigaction_t) action );
    if (oldhandle == SIG_ERR)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    return ( sigaction_t ) oldhandle;
}


#endif


#ifdef __cpluscplus
SLCL_EXITCPP
#endif