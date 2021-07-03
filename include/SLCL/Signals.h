#ifndef SLCL_SIGNAL
#define SLCL_SIGNAL

#include "Config.h"

#ifdef __cplusplus
SLCL_ENTERCPP
#endif

typedef int signum_t;
typedef void (*sigaction_t)(int, int); /* Second integer value not used on unix systems */

extern const signum_t SLCL_SIGABRT;
extern const signum_t SLCL_SIGFPE;
extern const signum_t SLCL_SIGILL;
extern const signum_t SLCL_SIGINT;
extern const signum_t SLCL_SIGSEGV;
extern const signum_t SLCL_SIGTERM;


extern sigaction_t slclSigaction( signum_t signal, sigaction_t action);

#ifdef __cplusplus
SLCL_EXITCPP
#endif

#endif