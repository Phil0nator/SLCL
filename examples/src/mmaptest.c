#include "SLCL/Mmap.h"
#include "SLCL/Errors.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    // Map a page to an address of the kernel's choice, with rw permissions
    void* page = slclMmap( NULL, 4096, SLCL_MAP_READWRITE );
    if (page == SLCL_FAILED)
    {
        slclPerror("mmap");
        exit(1);
    }
    // copy a string into the page
    strcpy( page, "This is a string being copied into mapped memory." );
    // print the page
    puts(page);

    // Unmap the page
    if (slclMunmap( page, 4096 ) == SLCL_ERROR)
    {
        slclPerror("munmap");
        exit(1);
    }

    // Open a file
    FILE* testfile = fopen("test.txt", "w+");

    // write a string to the file
    fwrite("This is a string in a file", 26, 1, testfile);
    
    // get the size of the file
    fseek( testfile, 0, SEEK_END );
    size_t size = ftell( testfile );
    fseek( testfile, 0, SEEK_SET );
    
    // determine the final size of the file
    // (allow for 36 extra bytes)
    size_t finalsize = size+36;
    // map the file to memory
    void* fmap = slclMmapFile( NULL, finalsize, SLCL_MAP_READWRITE, testfile );
    // truncate the file to the final size
    slclFtruncate( testfile, finalsize );
    // close file
    fclose(testfile);
    
    
    if (fmap == SLCL_FAILED)
    {
        slclPerror("fmap");
        exit(1);
    }

    // print the contents of the file from the memory map
    puts((char*)fmap);
    printf("Size: %ld\n", size);

    // write to the memory map
    strcpy( ((char*)fmap) + 26, "\nThis is a string in a mmapped file." );
    if (slclMunmapFile( fmap, finalsize ))
    {
        slclPerror("munmapfile");
        exit(1);
    }
    puts("Data written to test.txt");

    return 0;
}
