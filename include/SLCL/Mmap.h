#ifndef SLCL_MMAP
#define SLCL_MMAP

#include "Config.h"

#ifdef __cplusplus
SLCL_ENTERCPP
#endif

typedef void* slclmmap;
typedef int slclMapProt;

extern const slclMapProt SLCL_MAP_READONLY;
extern const slclMapProt SLCL_MAP_READWRITE;
extern const slclMapProt SLCL_MAP_EXEC;
extern const slclMapProt SLCL_MAP_NOACCESS;

extern slclmmap slclMmap( slclmmap ptr, size_t size, slclMapProt prot );


#ifdef __cplusplus
SLCL_EXITCPP
#endif

#endif