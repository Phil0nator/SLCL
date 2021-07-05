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

/**
 * Map an area of memory for use within this program.
 * @param ptr a suggested virtual address for the memory ( page aligned on many systems )
 * @param size the number of bytes to allocate
 * @param prot the protections given to the memory
 * @returns either the allocated address, or SLCL_FAILED if an error occured
 */
extern slclmmap slclMmap( slclmmap ptr, size_t size, slclMapProt prot );

/**
 * Memory map a file
 * @param ptr a suggested virtual addres ( page aligned on many systems )
 * @param size the number of bytes to map
 * @param prot the protections given to the memory
 * @param file an already opened file ( @see fopen )
 * @returns either the allocated address, or SLCL_FAILED if an error occured
 */
extern slclmmap slclMmapFile( slclmmap ptr, size_t size, slclMapProt prot, FILE* file );

/**
 * Unmap an area of memory obtained from slclMmap
 * @param ptr the address
 * @param size the size to unmap ( on some systems this must be the same amount as was allocated )
 * @warning on windows systems the entire map found at ptr is always unmapped, size is ignored
 * @returns SLCL_SUCCESS or SLCL_ERROR
 */
extern slclerr_t slclMunmap( slclmmap ptr, size_t size );

/**
 * Unmap a memory mapped file
 * @param ptr the address
 * @param size the size to unmap ( on some systems this must be the same amount as was allocated )
 * @warning on windows systems the entire map found at ptr is always unmapped, size is ignored
 * @returns SLCL_SUCCESS or SLCL_ERROR
 */
extern slclerr_t slclMunmapFile(slclmmap ptr, size_t size);

/**
 * Change the memory protections for a given address.
 * (on many systems the address must be page aligned)
 * @param ptr the address
 * @param size number of bytes (some systems must be the same size as original mapping)
 * @param protection the new protections
 * @returns either SLCL_SUCCESS or SLCL_ERROR
 */
extern slclerr_t slclMprotect( slclmmap ptr, size_t size, slclMapProt protection );

/**
 * Sync the memory in a mmapped file with the disk.
 * When you don't call slclMsync before unmapping or closing a file,
 * there is no guarentee that the written data will be actually
 * written to the disk on some systems.
 * @param ptr the mmapped file
 * @param size size of the mmap in bytes
 * @returns either SLCL_SUCCESS or SLCL_ERROR
 */
slclerr_t slclMsync( slclmmap ptr, size_t size );

/**
 * Set the size of a given file.
 * @param file an open file
 * @param newsize the new size of the file
 * @returns either SLCL_SUCCESS or SLCL_ERROR
 */
slclerr_t slclFtruncate( FILE* file, size_t newsize );

#ifdef __cplusplus
SLCL_EXITCPP
#endif

#endif