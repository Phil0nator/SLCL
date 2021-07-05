#include "SLCL/Library.h"
#include "SLCL/Errors.h"

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
    struct slclLibrary* lib;
    if ((lib = slclLoadLibrary( "liblibtestlib.so" )) == SLCL_FAILED)
    {
        slclPerror("loadlib");
        exit(1);
    }
    typedef void (*fnt) ();
    fnt superAwesomeFn;
    if ( ( superAwesomeFn = slclGetSymbol( lib, "superAwesomeSymbol" ) ) == SLCL_FAILED )
    {
        slclPerror("loadsym");
        exit(1);
    }
    superAwesomeFn();

    return 0;
}
