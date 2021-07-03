#include "SLCL/Network.h"
#include "SLCL/Errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{
    struct slclTcpServer* test = slclOpenTcpServer( SLCL_LOCALHOST, 5502 );
    
    if (test == SLCL_FAILED)
    {
        slclPerror("socktest");
        exit(1);
    }
    else
    {
        puts("ok");
    }

    while (1)
    {
        struct slclTcpSock* sock;
        if ((sock = slclAcceptTcpServer(test)) != SLCL_FAILED)
        {
            puts("New Connection");
            while( 1 )
            {
                char buffer[1024];
                memset(buffer, 0, 1024);
                slclerr_t packet = slclRecvTcpSock( sock, buffer, 1024 );
                if (packet == SLCL_ERROR)
                {
                    fputs("Connection lost.", stderr);
                    break;
                }
                fwrite( buffer, packet, 1, stdout );

                if ( strncmp( buffer, "exit", 4 ) == 0 )
                {
                    slclCloseTcpSock( sock, SLCL_SHUTDOWN_ALL );
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
