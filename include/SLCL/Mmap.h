#ifndef SLCL_MMAP
#define SLCL_MMAP

#include "Config.h"
#include <stdio.h>
#ifdef __cplusplus
SLCL_ENTERCPP
#endif

typedef void* slclmmap;
typedef int slclMapProt;
typedef int slclFileProt;

extern const slclMapProt SLCL_MAP_READONLY;
extern const slclMapProt SLCL_MAP_READWRITE;
extern const slclMapProt SLCL_MAP_EXEC;
extern const slclMapProt SLCL_MAP_NOACCESS;


extern slclmmap slclMmap( slclmmap ptr, size_t size, slclMapProt prot );
extern slclmmap slclMmapFile( slclmmap ptr, size_t size, slclMapProt prot, FILE* file );
extern slclerr_t slclMunmap( slclmmap ptr, size_t size );
extern slclerr_t slclMunmapFile(slclmmap ptr, size_t size);
extern slclerr_t slclMprotect( slclmmap ptr, size_t size, slclMapProt protection );

#ifdef __cplusplus
SLCL_EXITCPP
#endif

#endif