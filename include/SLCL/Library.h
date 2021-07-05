#ifndef SLCL_LIBRARY
#define SLCL_LIBRARY

#include "Config.h"

#ifdef __cplusplus
SLCL_ENTERCPP
#endif

// Opaque types:

/**
 * The os-dependent representation for dynamic libraries
 */
struct slclLibrary;

/**
 * Load a dynamic library from a file 'filepath'.
 * @param filepatht the path to the dynamic library file
 * @returns either a succesfully loaded opaque type, or SLCL_FAILED.
 * @note you can free all the resources used by the returned slclLibrary*
 * by calling slclFreeLibrary(  )
 */
extern struct slclLibrary* slclLoadLibrary(const char* filepath);

/**
 * Get a symbol from a dynamic library.
 * @param handle the slclLibrary pointer from a call to slclLoadLibrary
 * @param symbol the name of the function/symbol to search for
 * @returns either the given function pointer (as void*) or SLCL_FAILED
 */
extern void* slclGetSymbol(struct slclLibrary* handle, const char* symbol);

/**
 * Free all the resources used by a slclLibrary handle.
 * @param handle the slclLibrary to free
 * @returns either SLCL_SUCCESS or SLCL_ERROR
 * @warning on most systems, if the reference count to this handle reaches
 * zero ( through calls to the os's version of FreeLibrary ) any symbols
 * you might have retreived from it using [slclGetSymbol] or similar, will
 * be removed from your program's address space. Ensure that you are done
 * with all the symbols related to this library before freeing it.
 */
extern slclerr_t slclFreeLibrary(struct slclLibrary* handle);


#ifdef __cplusplus
SLCL_EXITCPP
#endif

#endif