#ifndef SLCL_ERRINC
#define SLCL_ERRINC

#include "Config.h"


#ifdef __cplusplus
SLCL_ENTERCPP
#endif

/**
 * Get the last slcl error as a string.
 * 
 * @warning if a new error occurs after a call to slclStrerr, the 
 * originally returned pointer will become invalid. Any persistent use
 * of error messages from this function should be copied into your buffer 
 * before any further slcl functions are used.
 * 
 * @returns a cstring of the last error
 * 
 * 
 */
extern const char* slclStrerr();
/**
 * Print the last error message into stderr in the format:
 * {prefix}: {last error}
 * @param prefix a bit of text to prepend to the error message
 * (as is seen in the above format)
 */
extern void slclPerror( const char* prefix );


#ifdef __cplusplus
SLCL_EXITCPP
#endif


#endif