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
    struct slclSockaddr_in* addr = slclCreateStrAddress(SLCL_AF_INET, "127.0.0.1", 5500);

    if (addr == SLCL_FAILED)
    {
        slclPerror("socktest: addr");
        exit(1);
    }
    if (slclBind(sock, addr) == SLCL_ERROR)
    {
        slclPerror("socktest: bind");
        exit(1);
    }
    if (slclListen(sock, 4096) == SLCL_ERROR)
    {
        slclPerror("socktest: listen");
        exit(1);
    }
    slclFreeAddress(addr);

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
                slclerr_t packet = slclRecv( nsock, buffer, 1024 );
                if (packet == SLCL_ERROR)
                {
                    slclPerror("socktest: recv");
                    slclCloseSock(nsock);
                    break;
                }
                fwrite( buffer, packet, 1, stdout );

                if ( strncmp( buffer, "exit", 4 ) == 0 )
                {
                    slclCloseSock( nsock );
                    puts("Connection closed.");
                    break;
                }

            }
        }
        else
        {
            slclPerror("socktest");
            slclCloseSock(nsock);
            exit(1);
        }
    }


    return 0;
}
