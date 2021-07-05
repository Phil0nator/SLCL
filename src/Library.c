#include "SLCL/Library.h"
#include "InternalErrors.h"
#include <string.h>
#include <errno.h>

#ifdef __cpluscplus
SLCL_ENTERCPP
#endif

#if defined(SLCL_TARGET_WIN)
#include <windows.h>


typedef struct slclLibrary
{
	HMODULE winHandle;
} slclLibrary_t;


struct slclLibrary* slclLoadLibrary(const char* filepath)
{

	HMODULE handle = LoadLibrary(filepath);
	if (handle == NULL)
	{
		slclSetLastWinErr();
		return SLCL_FAILED;
	}

	struct slclLibrary* out = malloc(sizeof(struct slclLibrary));
	if (out == NULL)
	{
		slclSeterr("Out of memory.");
		return SLCL_FAILED;
	}
	out->winHandle = handle;
	return out;

}
void* slclGetSymbol(struct slclLibrary* handle, const char* symbol)
{
	void* proc;
	if ((proc = GetProcAddress(handle->winHandle, symbol)) == NULL)
	{
		slclSetLastWinErr();
		return SLCL_FAILED;
	}
	return proc;
}
slclerr_t slclFreeLibrary(struct slclLibrary* handle)
{
	if ((FreeLibrary(handle->winHandle)) == 0)
	{
		slclSetLastWinErr();
		return SLCL_ERROR;
	}
	free(handle);
	return SLCL_SUCCESS;
}


#elif defined(SLCL_TARGET_UNIXBASED)
#include <unistd.h>
#include <dlfcn.h>




extern struct slclLibrary* slclLoadLibrary(const char* filepath)
{
	struct slclLibrary* out;
	if ( out = (struct slclLibrary*) dlopen( filepath, RTLD_LAZY ) == NULL)
	{
		slclSeterr( dlerror() );
		return SLCL_FAILED;
	}
	return out;
}
extern void* slclGetSymbol(struct slclLibrary* handle, const char* symbol)
{
	void* outfn;
	if ((outfn = dlsym( (void*) handle, symbol)) == NULL )
	{
		slclSeterr( dlerror() );
		return SLCL_FAILED;
	}
	return outfn;
}
extern slclerr_t slclFreeLibrary(struct slclLibrary* handle)
{
	if (dlclose(handle) == -1)
	{
		slclSeterr( dlerror() );
		return SLCL_ERROR;
	}
	return SLCL_SUCCESS;
}



#endif


#ifdef __cpluscplus
SLCL_EXITCPP
#endif