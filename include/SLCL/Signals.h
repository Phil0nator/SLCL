#ifndef SLCL_SIGNAL
#define SLCL_SIGNAL

#include "Config.h"

#ifdef __cplusplus
SLCL_ENTERCPP
#endif

typedef int signum_t;
typedef void (*sigaction_t)(int);

extern const signum_t SLCL_SIGABRT;
extern const signum_t SLCL_SIGFPE;
extern const signum_t SLCL_SIGILL;
extern const signum_t SLCL_SIGINT;
extern const signum_t SLCL_SIGSEGV;
extern const signum_t SLCL_SIGTERM;

/**
 * Set a new signal handler for a given signal type.
 * @param signal the signal to apply the handler to
 * @param action the handler for the signal ( function pointer )
 * @returns the old handler for this signal (function pointer)
 */
extern sigaction_t slclSigaction( signum_t signal, sigaction_t action);

#ifdef __cplusplus
SLCL_EXITCPP
#endif

#endif