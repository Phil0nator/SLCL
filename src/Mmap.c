#include "SLCL/Mmap.h"
#include "InternalErrors.h"
#include <string.h>
#include <errno.h>


#ifdef __cpluscplus
SLCL_ENTERCPP
#endif 

#if defined(SLCL_TARGET_WIN)
#include <windows.h>
#include <io.h>

const slclMapProt SLCL_MAP_READONLY = PAGE_READONLY;
const slclMapProt SLCL_MAP_READWRITE = PAGE_READWRITE;
const slclMapProt SLCL_MAP_EXEC = PAGE_EXECUTE;
const slclMapProt SLCL_MAP_NOACCESS = PAGE_NOACCESS;




slclmmap slclMmap(slclmmap ptr, size_t size, slclMapProt prot)
{
    void* out;
    if ( (out = VirtualAlloc(ptr, size, MEM_COMMIT, prot)) == NULL)
    {
        slclSetLastWinErr();
        return SLCL_FAILED;
    }
    return out;
}
slclmmap slclMmapFile(slclmmap ptr, size_t size, slclMapProt prot, FILE* file)
{
    HANDLE h;
    void* out;
    if ((h = CreateFileMapping((HANDLE)_get_osfhandle(_fileno(file)), 0, prot, 0, (int) size, "")) == NULL)
    {
        slclSetLastWinErr();
        return SLCL_FAILED;
    }

    int protflag = 0;
    switch ( prot & (~SLCL_MAP_EXEC) )
    {
    case PAGE_READONLY:
        protflag = FILE_MAP_READ;
        break;
    case PAGE_READWRITE:
        protflag = FILE_MAP_ALL_ACCESS;
        break;
    case PAGE_NOACCESS:
        protflag = FILE_MAP_TARGETS_INVALID;
        break;
    }

    if (prot & SLCL_MAP_EXEC) protflag |= FILE_MAP_EXECUTE;

    if ((out = MapViewOfFile(h, protflag, 0, 0, size)) == NULL)
    {
        slclSetLastWinErr();
        return SLCL_FAILED;
    }

    return out;

}
slclerr_t slclMunmap(slclmmap ptr, size_t size)
{
    if (VirtualFree(ptr, 0, MEM_RELEASE) == 0)
    {
        slclSetLastWinErr();
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}

slclerr_t slclMunmapFile(slclmmap ptr, size_t size)
{
    if (UnmapViewOfFile(ptr) == 0)
    {
        slclSetLastWinErr();
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}
slclerr_t slclMsync( slclmmap ptr, size_t size )
{
    if ( FlushViewOfFile( ptr, size ) == 0 )
    {
        slclSetLastWinErr();
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}
slclerr_t slclMprotect(slclmmap ptr, size_t size, slclMapProt protection)
{
    if ((VirtualProtect(ptr, size, protection, 0)) == 0)
    {
        slclSetLastWinErr();
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}
slclerr_t slclFtruncate( FILE* file, size_t newsize )
{
    size_t currpos = ftell(file);
    fseek(file, newsize, SEEK_SET);
    if ( SetEndOfFile( (HANDLE)_get_osfhandle( _fileno(file) ) ) == 0 )
    {
        fseek(file, currpos, SEEK_SET);
        slclSetLastWinErr();
        return SLCL_ERROR;
    }
    fseek(file, currpos, SEEK_SET);
    return SLCL_SUCCESS;
}


#elif defined(SLCL_TARGET_UNIXBASED)
#include <sys/mman.h>
#include <unistd.h>
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

slclmmap slclMmapFile( slclmmap ptr, size_t size, slclMapProt prot, FILE* file )
{
    void * out;
    if ((out = mmap( ptr, size, prot, MAP_SHARED, fileno(file), 0 )) == MAP_FAILED)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    return out;
}

slclerr_t slclMsync( slclmmap ptr, size_t size )
{
    if (msync( ptr, size, MS_SYNC ) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}

slclerr_t slclMunmap( slclmmap ptr, size_t size )
{
    
    if (munmap( ptr, size ) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}
slclerr_t slclMunmapFile(slclmmap ptr, size_t size)
{
    if (munmap(ptr, size) == -1)
    {
        slclSeterr(strerror(errno));
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}
slclerr_t slclMprotect( slclmmap ptr, size_t size, slclMapProt protection )
{
    if (mprotect( ptr, size, protection ) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}

slclerr_t slclFtruncate( FILE* file, size_t newsize )
{
    if (ftruncate( fileno(file), newsize ) == -1 )
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