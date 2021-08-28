#include "SLCL/Network.h"
#include "SLCL/Errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{
    // Open an IPv4 socket
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

    // construct a sockaddr_in for localhost:5500
    struct slclSockaddr_in* addr = slclCreateStrAddress(SLCL_AF_INET, "127.0.0.1", 5500);

    if (addr == SLCL_FAILED)
    {
        slclPerror("socktest: addr");
        exit(1);
    }

    // bind to localhost:5500
    if (slclBind(sock, addr) == SLCL_ERROR)
    {
        slclPerror("socktest: bind");
        exit(1);
    }
    // begin listening for up to 4096 connections
    if (slclListen(sock, 4096) == SLCL_ERROR)
    {
        slclPerror("socktest: listen");
        exit(1);
    }
    // free resources used by the address
    slclFreeAddress(addr);

    while (1)
    {
        struct slclSock* nsock;
        // Accept an incoming connection
        if ((nsock = slclAccept(sock)) != SLCL_FAILED)
        {
            puts("New Connection");
            while( 1 )
            {
                char buffer[1024];
                memset(buffer, 0, 1024);
                // receive data into a buffer from the new connection
                slclerr_t packet = slclRecv( nsock, buffer, 1024 );
                if (packet == SLCL_ERROR)
                {
                    slclPerror("socktest: recv");
                    slclCloseSock(nsock);
                    break;
                }
                // print the incoming data to stdout
                fwrite( buffer, packet, 1, stdout );
                // leave the loop if the sender sent "exit"
                if ( strncmp( buffer, "exit", 4 ) == 0 )
                {
                    // free resources used by the incoming socket
                    slclCloseSock( nsock );
                    puts("Connection closed.");
                    break;
                }

            }
        }
        else
        {
            slclPerror("socktest");
            // free resources used by the incoming connection
            slclCloseSock(nsock);
            exit(1);
        }
    }


    return 0;
}
