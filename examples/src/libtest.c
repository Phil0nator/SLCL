#include "SLCL/Library.h"
#include "SLCL/Errors.h"

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
    struct slclLibrary* lib;
    // Load the dynamic library
    if ((lib = slclLoadLibrary( "liblibtestlib.so" )) == SLCL_FAILED)
    {
        slclPerror("loadlib");
        exit(1);
    }
    typedef void (*fnt) ();
    fnt superAwesomeFn;
    // Extract a symbol
    if ( ( superAwesomeFn = slclGetSymbol( lib, "superAwesomeSymbol" ) ) == SLCL_FAILED )
    {
        slclPerror("loadsym");
        exit(1);
    }

    // call the dynamic library function
    superAwesomeFn();

    return 0;
}
