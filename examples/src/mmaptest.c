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

    FILE* testfile = fopen("test.txt", "w+");

    fwrite("This is a string in a file", 26, 1, testfile);
    fseek( testfile, 0, SEEK_END );
    size_t size = ftell( testfile );
    fseek( testfile, 0, SEEK_SET );
    
    size_t finalsize = size+36;
    void* fmap = slclMmapFile( NULL, finalsize, SLCL_MAP_READWRITE, testfile );
    slclFtruncate( testfile, finalsize );
    fclose(testfile);
    
    
    if (fmap == SLCL_FAILED)
    {
        slclPerror("fmap");
        exit(1);
    }
    
    puts((char*)fmap);
    printf("Size: %ld\n", size);

    strcpy( ((char*)fmap) + 26, "\nThis is a string in a mmapped file." );
    if (slclMunmapFile( fmap, finalsize ))
    {
        slclPerror("munmapfile");
        exit(1);
    }
    puts("Data written to test.txt");

    return 0;
}
