#include "SLCL/Mmap.h"
#include "SLCL/Errors.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    void* page = slclMmap( NULL, 4096, SLCL_MAP_READWRITE );
    if (page == SLCL_FAILED)
    {
        slclPerror("mmap");
        exit(1);
    }
    strcpy( page, "This is a string being copied into mapped memory." );
    puts(page);
    if (slclMunmap( page, 4096 ) == SLCL_ERROR)
    {
        slclPerror("munmap");
        exit(1);
    }

    return 0;
}
