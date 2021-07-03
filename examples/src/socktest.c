#include "SLCL/Network.h"
#include "SLCL/Errors.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    struct tcpserver* test = open_tcpserver( SLCL_LOCALHOST, 5500 );

    if (test == SLCL_FAILED)
    {
        slcl_perror("socktest");
        exit(1);
    }
    else
    {
        puts("ok");
    }

    while (1)
    {
        struct tcpsock* sock;
        if ((sock = accept_tcpserver(test)) != SLCL_FAILED)
        {
            puts("New Connection");
            while( 1 )
            {
                struct tcppacket* packet = recv_tcpsock( sock, 1024 );
                if (packet == SLCL_FAILED)
                {
                    fputs("Connection lost.", stderr);
                    break;
                }
                fwrite( read_tcppacket( packet ), sizeof_tcppacket( packet ), 1, stdout );
                free_tcppacket(packet);            
            }
        }
        else
        {
            slcl_perror("socktest");
            exit(1);
        }
    }

    exit:

    return 0;
}
