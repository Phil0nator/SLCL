#ifndef SLCL_LIBRARY
#define SLCL_LIBRARY

#include "Config.h"

#ifdef __cplusplus
SLCL_ENTERCPP
#endif

// Opaque types:

struct slclLibrary;


extern struct slclLibrary* slclLoadLibrary(const char* filepath);
extern void* slclGetSymbol(struct slclLibrary* handle, const char* symbol);
extern slclerr_t slclFreeLibrary(struct slclLibrary* handle);


#ifdef __cplusplus
SLCL_EXITCPP
#endif

#endif