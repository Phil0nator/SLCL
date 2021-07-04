#include "SLCL/Network.h"
#include "SLCL/Errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{
    struct slclSock* sock = slclOpenSock( SLCL_AF_INET, SLCL_SOCK_STREAM, 0 );

    if (sock == SLCL_FAILED)
    {
        slclPerror("socktest");
        exit(1);
    }
    else
    {
        puts("ok");
    }
    slclConnect(sock, slclCreateStrAddress( SLCL_AF_INET, "127.0.0.1", 5500 ) );


    while (1)
    {
        struct slclSock* nsock;
        if ((nsock = slclAccept(sock)) != SLCL_FAILED)
        {
            puts("New Connection");
            while( 1 )
            {
                char buffer[1024];
                memset(buffer, 0, 1024);
                slclerr_t packet = slclRecv( sock, buffer, 1024 );
                if (packet == SLCL_ERROR)
                {
                    fputs("Connection lost.", stderr);
                    break;
                }
                fwrite( buffer, packet, 1, stdout );

                if ( strncmp( buffer, "exit", 4 ) == 0 )
                {
                    slclCloseSock( sock );
                    puts("Connection closed.");
                    break;
                }

            }
        }
        else
        {
            slclPerror("socktest");
            exit(1);
        }
    }


    return 0;
}
