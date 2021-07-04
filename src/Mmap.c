#include "SLCL/Mmap.h"
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

const slclMapProt SLCL_MAP_READONLY = PROT_READ;
const slclMapProt SLCL_MAP_READWRITE = PROT_READ | PROT_WRITE;
const slclMapProt SLCL_MAP_EXEC = PROT_EXEC;
const slclMapProt SLCL_MAP_NOACCESS = PROT_NONE;

slclmmap slclMmap( slclmmap ptr, size_t size, slclMapProt prot )
{
    void* out;
    if ((out = mmap( ptr, size, prot, MAP_ANON | MAP_PRIVATE, 0, 0 )) == MAP_FAILED)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    return out;
}

extern slclmmap slclMmapFile( slclmmap ptr, size_t size, slclMapProt prot, FILE* file )
{
    void * out;
    if ((out = mmap( ptr, size, prot, MAP_ANON | MAP_PRIVATE, fileno(file), 0 )) == MAP_FAILED)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    return out;
}
extern slclerr_t slclMunmap( slclmmap ptr, size_t size )
{
    if (munmap( ptr, size ) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}
extern slclerr_t slclMprotect( slclmmap ptr, size_t size, slclMapProt protection )
{
    if (mprotect( ptr, size, protection ) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}


#endif


#ifdef __cpluscplus
SLCL_EXITCPP
#endif