#include "SLCL/Network.h"
#include <stdint.h>
#include <stddef.h>
#include "InternalErrors.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cpluscplus
SLCL_ENTERCPP
#endif

// General declarations
typedef struct tcppacket
{
    char* data; // Base pointer of memory
    char* ptr;  // Position pointer in owned memory
    size_t size;// Size of all memory
    
} tcppacket_t;

struct tcppacket* create_tcppacket()
{
    tcppacket_t* out = malloc(sizeof(tcppacket_t));
    // Data is malloc'ed by default
    out->data = malloc(1);
    out->ptr = out->data;
    out->size = 1;

    return out;
}

void realloc_tcppack( struct tcppacket* dest, size_t finalSize )
{
    // Check if reallocation is needed
    if (finalSize > dest->size)
    {
        // if so, reallocate the data field
        char* newdata = realloc( dest->data, finalSize );
        if (newdata == NULL)
        {
            fputs("SLCL: Ran out of memory.", stderr);
            exit(1);
        }
        // At this point all of dest's fields need to be updated
        size_t currentPosition = dest->ptr - dest->data;

        // Could be new pointer
        dest->data = newdata;
        // Might need to be new offset of that pointer
        dest->ptr = dest->data + currentPosition;
        // New size
        dest->size = finalSize;
    }
}

void write_tcppack( struct tcppacket* dest, const char* data, size_t size )
{
    // Determine final size of packet
    size_t finalSize = (dest->ptr - dest->data) + size;

    // Ensure that enough size will be available
    realloc_tcppack( dest, finalSize );

    // Copy data into the packet
    memcpy( dest->ptr, data, size );
    // Update the position pointer
    dest->ptr += size;
}

void free_tcppacket( struct tcppacket* packet )
{
    if (packet->data) free(packet->data);
    free(packet);
}
size_t sizeof_tcppacket( struct tcppacket* packet )
{
    return packet->size;
}
char* read_tcppacket( struct tcppacket* packet )
{
    return packet->data;
}
char* disown_tcppacket( struct tcppacket* packet )
{
    char* out = packet->data;
    packet->data = NULL;
    return out;
}


static void memrcpy( const char* src, char* dest, size_t size )
{
    for(size_t i = 1; i <= size; i++)
    {
        dest[size-i] = src[i-1];
    }
}


#if defined(SLCL_TARGET_WIN)
// Windows declarations


#elif defined(SLCL_TARGET_UNIXBASED)
// Linux declarations
#   include <sys/socket.h>
#   include <arpa/inet.h>
#   include <string.h>
#   include <errno.h>

typedef int fd_t;

typedef struct tcpsock
{
    fd_t fd;
} tcpsock_t;

typedef struct tcpserver
{
    fd_t fd;
} tcpserver_t;

// Open a socket, and process an ip/port
fd_t createUnixTCPSockAddrPair( const char* ip, struct sockaddr_in* addr )
{
    slclerr_t err;
    // open a standard tcp socket
    fd_t temporary = socket( AF_INET, SOCK_STREAM, 0 );
    if (temporary == -1)
    {
        slcl_seterr( strerror( errno ) );
        return -1;
    }
    // Process an ip address as %d.%d.%d.%d into sockaddr_in
    if ((err = inet_aton( ip, &addr->sin_addr )) == 0)
    {
        slcl_seterr( "Invalid IP address." );
        return -1;
    }
    // return socket file descriptor
    return temporary;
}

struct tcpsock* open_tcpsock( const char* ip, short port )
{
    
    slclerr_t err;

    // setup address info
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons( port );

    // open socket, and process address
    fd_t temporary = createUnixTCPSockAddrPair( ip, &address );
    if (temporary == -1)
    {
        return SLCL_FAILED;
    }

    // Try to connect
    if ((err = connect( temporary, (struct sockaddr*) &address, sizeof(struct sockaddr_in) )) == -1)
    {
        slcl_seterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    
    // return new socket if all has gone well
    struct tcpsock* outsock = malloc( sizeof(struct tcpsock) );
    outsock->fd = temporary;
    return outsock;

}
slclerr_t send_tcpsock( struct tcpsock* sock, const char* data, size_t bytes )
{
    slclerr_t err;

    if ((err = send(sock->fd, data, bytes, 0)) == -1)
    {
        slcl_seterr( strerror( errno ) );
        return SLCL_ERROR;
    }

    return err;

}
slclerr_t send_tcpsockpack( struct tcpsock* sock, struct tcppacket* data )
{
    return send_tcpsock( sock, data->data, data->size );
}
struct tcppacket* recv_tcpsock( struct tcpsock* sock, size_t bytes )
{
    struct tcppacket* out = create_tcppacket();
    realloc_tcppack(out, bytes);
    slclerr_t recved;
    if ((recved = recv( sock->fd, out->data, bytes, 0)) == -1)
    {
        free_tcppacket(out);
        slcl_seterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    out->size = recved;
    return out;
}
struct tcppacket* recv_tcpsock_exact( struct tcpsock* sock, size_t bytes )
{
    struct tcppacket* out = create_tcppacket();
    realloc_tcppack( out, bytes );
    slclerr_t recved = 0;
    while (recved < bytes)
    {
        slclerr_t recviter = 0;
        if ((recviter = recv(sock->fd, out->ptr, bytes - recved, 0)) == -1)
        {
            free_tcppacket(out);
            slcl_seterr( strerror( errno ) );
            return SLCL_FAILED;
        }
        out->ptr += recviter;
        recved += recviter;
    }

    return out;
}


struct tcpserver* open_tcpserver( const char* ip, short port )
{

    slclerr_t err;
    // setup address info
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons( port );
    // open socket and process address info
    fd_t temporary = createUnixTCPSockAddrPair( ip, &address );
    if (temporary == -1)
    {
        return SLCL_FAILED;
    }
    // try to bind socket
    if ((err = bind( temporary, (struct sockaddr*) &address, sizeof(struct sockaddr) )) == -1)
    {
        slcl_seterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    // try to listen
    if ((err = listen( temporary, SOMAXCONN )) == -1)
    {
        slcl_seterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    // return new server if all has gone well
    struct tcpserver* outsock = malloc( sizeof(struct tcpserver) );
    outsock->fd = temporary;
    return outsock;

}

struct tcpsock* accept_tcpserver( struct tcpserver* server )
{

    slclerr_t err;
    struct sockaddr_storage addr;
    socklen_t size;

    // Attempt unix accept()
    // (blocking)
    if ((err = accept( server->fd, (struct sockaddr*) &addr, &size )) == -1)
    {
        slcl_seterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    // Once accept has found a client, it is returned here
    struct tcpsock* outsock = malloc( sizeof( struct tcpsock ) );
    outsock->fd = err;
    return outsock;

}



#ifdef __cpluscplus
SLCL_EXITCPP
#endif


#endif

