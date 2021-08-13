#ifndef SLCL_FSYS
#define SLCL_FSYS

#include "Config.h"


#ifdef __cplusplus
SLCL_ENTERCPP
#endif

typedef char* path;
typedef size_t path_size;

#ifdef SLCL_TARGET_WIN
#define SLCL_PATH_MAX 260
#else
#define SLCL_PATH_MAX 256 
#endif

typedef int slclMode_t;

extern const slclMode_t SLCL_IRWXU;
extern const slclMode_t SLCL_IRUSR;
extern const slclMode_t SLCL_IWUSR;
extern const slclMode_t SLCL_IXUSR;
extern const slclMode_t SLCL_IRWXG;
extern const slclMode_t SLCL_IRGRP;
extern const slclMode_t SLCL_IWGRP;
extern const slclMode_t SLCL_IXGRP;
extern const slclMode_t SLCL_IRWXO;
extern const slclMode_t SLCL_IROTH;
extern const slclMode_t SLCL_IWOTH;
extern const slclMode_t SLCL_IXOTH;
extern const slclMode_t SLCL_ISUID;
extern const slclMode_t SLCL_ISGID;
extern const slclMode_t SLCL_ISVTX;

extern const slclMode_t SLCL_DIRMODE_DEFAULT;


path slclGetCwd( path dest, path_size max );
slclerr_t slclChdir( path newcwd );

slclerr_t slclMkdir( path p, slclMode_t mode );

#ifdef __cplusplus
SLCL_EXITCPP
#endif


#endif