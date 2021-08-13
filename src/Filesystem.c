#include "SLCL/Filesystem.h"
#include <string.h>
#include "InternalErrors.h"

#ifdef SLCL_TARGET_WIN
path slclGetCwd( path dest, path_size max );
slclerr_t slclChdir( path newcwd );
#else
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

const slclMode_t SLCL_IRWXU = S_IRWXU;
const slclMode_t SLCL_IRUSR = S_IRUSR;
const slclMode_t SLCL_IWUSR = S_IWUSR;
const slclMode_t SLCL_IXUSR = S_IXUSR;
const slclMode_t SLCL_IRWXG = S_IRWXG;
const slclMode_t SLCL_IRGRP = S_IRGRP;
const slclMode_t SLCL_IWGRP = S_IWGRP;
const slclMode_t SLCL_IXGRP = S_IXGRP;
const slclMode_t SLCL_IRWXO = S_IRWXO;
const slclMode_t SLCL_IROTH = S_IROTH;
const slclMode_t SLCL_IWOTH = S_IWOTH;
const slclMode_t SLCL_IXOTH = S_IXOTH;
const slclMode_t SLCL_ISUID = S_ISUID;
const slclMode_t SLCL_ISGID = S_ISGID;
const slclMode_t SLCL_ISVTX = S_ISVTX;

const slclMode_t SLCL_DIRMODE_DEFAULT = 0755;


path slclGetCwd( path dest, path_size max )
{
    if ( getcwd( dest, max ) == NULL )
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    return dest;
}

slclerr_t slclChdir( path newcwd )
{
    if (chdir( newcwd )!=0)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}

#endif