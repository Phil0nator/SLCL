#ifndef SLCL_CFG
#define SLCL_CFG

// Simple Lightweight Cross-Platform Layer

#define SLCL_VERSION 0.01
#define SLCL_VERSION_S  "0.01"


#if defined(WIN32) || defined(WIN64)
#   define SLCL_TARGET_WIN
#   define SLCL_OSNAME "Windows"
#elif defined(__linux__)
#   define SLCL_TARGET_LINUX
#   define SLCL_TARGET_UNIXBASED
#   define SLCL_OSNAME "Linux"
#elif defined(__MACH__) || defined(__APPLE__)
#   define SLCL_TARGET_DARWIN
#   define SLCL_TARGET_UNIXBASED
#   define SLCL_OSNAME "MacOS"
#elif defined(unix)
#   define SLCL_TARGET_UNIX
#   define SLCL_TARGET_UNIXBASED
#   define SLCL_OSNAME "Unix"
#elif defined(__FreeBSD__)
#   define SLCL_TARGET_BSD
#   define SLCL_TARGET_UNIXBASED
#   define SLCL_OSNAME "BSD"
#else
#   define SLCL_TARGET_UNKWN
#   define SLCL_OSNAME "Unkown"
#endif 

#define SLCL_ENTERCPP extern "C" {
#define SLCL_EXITCPP }

typedef int slclerr_t;
#include <stddef.h>

#define SLCL_FAILED ((void*)-1)
#define SLCL_ERROR  (-1)
#define SLCL_LITTLE_ENDIAN ((1 == *(unsigned char *)&(const int){1}))





#endif
