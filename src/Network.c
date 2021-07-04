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


#if defined(SLCL_TARGET_WIN)
// Windows declarations
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

typedef struct slclTcpSock
{
    SOCKET s;
} slclTcpSock_t;

typedef struct slclTcpServer
{
    SOCKET s;
} slclTcpServer_t;

slclerr_t slclInitWinsock()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return SLCL_ERROR;
    }
    return 0;
}

void slclSetWsaErrorMsg()
{
    memset(slclGetErrorBuf(), 0, SLCL_ERRORBUF_SIZE);
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        slclGetErrorBuf(),
        SLCL_ERRORBUF_SIZE,
        NULL);
}

slclerr_t slclConstructBaseWindowsSockaddr(struct sockaddr_in* addr, const char* ip, short port)
{
    slclerr_t err = inet_pton(AF_INET, ip, &addr->sin_addr.s_addr);
    if (!err)
    {
        slclSeterr("Invalid IP address.");
        return SLCL_ERROR;
    }
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    return 0;
}

SOCKET slclConstructBaseWindowsTcp()
{
    slclInitWinsock();
    SOCKET tmp;
    if ((tmp = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        slclSetWsaErrorMsg();
        return INVALID_SOCKET;
    }
    

    return tmp;
}


struct slclTcpSock* slclOpenTcpSock( const char* ip, short port )
{
    
    SOCKET tmp = slclConstructBaseWindowsTcp(ip, port);
    if (tmp == INVALID_SOCKET)
    {
        return SLCL_FAILED;
    }

    struct sockaddr_in addr;
    slclerr_t err = slclConstructBaseWindowsSockaddr(&addr, ip, port);

    if (err == SLCL_ERROR)
    {
        return SLCL_FAILED;
    }

    if ((connect(tmp, (struct sockaddr*)&addr, sizeof(addr)) < 0))
    {
        slclSetWsaErrorMsg();
        return INVALID_SOCKET;
    }

    struct slclTcpSock* out = malloc(sizeof(slclTcpSock_t));
    if (out == 0) {
        slclSeterr("Out of memory.");
        return SLCL_FAILED;
    }
    out->s = tmp;
    return out;

}

struct slclTcpServer* slclOpenTcpServer(const char* ip, short port)
{
    slclInitWinsock();
    SOCKET tmp = slclConstructBaseWindowsTcp();
    if (tmp == INVALID_SOCKET)
    {
        return SLCL_FAILED;
    }
    struct sockaddr_in addr;
    slclerr_t err = slclConstructBaseWindowsSockaddr(&addr, ip, port);
    if (err == SLCL_ERROR)
    {
        return SLCL_FAILED;
    }
    
    if (bind(tmp, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        slclSetWsaErrorMsg();
        return SLCL_FAILED;
    }

    if ((listen(tmp, 4096)) == SOCKET_ERROR)
    {
        slclSetWsaErrorMsg();
        return SLCL_FAILED;
    }

    struct slclTcpServer* out = malloc(sizeof(slclTcpServer_t));
    if (out == 0)
    {
        slclSeterr("Out of memory.");
        return SLCL_FAILED;
    }

    out->s = tmp;
    return out;

}

struct slclTcpSock* slclAcceptTcpServer(struct slclTcpServer* server)
{

    SOCKET newsock;
    struct sockaddr_in client;
    newsock = accept(server->s, NULL, NULL);
    if (newsock == INVALID_SOCKET)
    {
        slclSetWsaErrorMsg();
        return SLCL_FAILED;
    }

    struct slclTcpSock* out = malloc(sizeof(struct slclTcpSock));
    if (out == 0)
    {
        slclSeterr("Out of memory.");
        return SLCL_FAILED;
    }
    out->s = newsock;
    return out;

}

slclerr_t slclSendTcpSock(struct slclTcpSock* sock, const char* data, size_t bytes)
{
    slclerr_t out;
    if ((out = send(sock->s, data, bytes, 0))< 0)
    {
        slclSetWsaErrorMsg();
        return SLCL_ERROR;
    }
    return out;
}

slclerr_t slclRecvTcpSock(struct slclTcpSock* sock, char* buffer, size_t bytes)
{
    slclerr_t out;
    if ((out = recv(sock->s, buffer, bytes, 0)) == SOCKET_ERROR)
    {
        slclSetWsaErrorMsg();
        return SLCL_ERROR;
    }
    return out;
}

slclerr_t slclRecvTcpSockExact(struct slclTcpSock* sock, char* buffer, size_t bytes)
{

}

slclerr_t slclCloseTcpSock(struct slclTcpSock* socket, enum slclShutdownMethod how)
{
    if (shutdown(socket->s, how) == SOCKET_ERROR)
    {
        slclSetWsaErrorMsg();
        return SLCL_ERROR;
    }
    
    if (closesocket(socket->s) == SOCKET_ERROR)
    {
        slclSetWsaErrorMsg();
        return SLCL_ERROR;
    }
    return 0;
}

slclerr_t slclCloseTcpServer( struct slclTcpServer* server, enum slclShutdownMethod how )
{
    if (shutdown(server->s, how) == SOCKET_ERROR)
    {
        slclSetWsaErrorMsg();
        return SLCL_ERROR;
    }

    if (closesocket(server->s) == SOCKET_ERROR)
    {
        slclSetWsaErrorMsg();
        return SLCL_ERROR;
    }
    return 0;
}


#elif defined(SLCL_TARGET_UNIXBASED)
// Linux declarations
#   include <sys/socket.h>
#   include <arpa/inet.h>
#   include <string.h>
#   include <errno.h>

typedef int fd_t;

typedef struct slclTcpSock
{
    fd_t fd;
} tcpsock_t;

typedef struct slclTcpServer
{
    fd_t fd;
} tcpserver_t;



/**
 * slclCloseTcpSock will free all the resources being used by a tcp socket.
 * @param socket a slclTcpSock
 * @returns
 */

slclerr_t slclCloseTcpSock( struct slclTcpSock* socket, enum slclShutdownMethod how )
{
    slclerr_t error = shutdown(socket->fd, how);
    if (error == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    free(socket);
}

/**
 * slclCloseTcpSock will free all the resources being used by a tcp server. 
 * ( this does NOT include any clients that may have been created through
 *  calls to slclAcceptTcpServer )
 * @param socket a slclTcpSock
 * @returns
 */
slclerr_t slclCloseTcpServer( struct slclTcpServer* server, enum slclShutdownMethod how )
{
    slclerr_t error = shutdown(server->fd, how);
    if (error == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    free(socket);
}


// Open a socket, and process an ip/port
fd_t createUnixTCPSockAddrPair( const char* ip, struct sockaddr_in* addr )
{
    slclerr_t err;
    // open a standard tcp socket
    fd_t temporary = socket( AF_INET, SOCK_STREAM, 0 );
    if (temporary == -1)
    {
        slclSeterr( strerror( errno ) );
        return -1;
    }
    // Process an ip address as %d.%d.%d.%d into sockaddr_in
    if ((err = inet_aton( ip, &addr->sin_addr )) == 0)
    {
        slclSeterr( "Invalid IP address." );
        return -1;
    }
    // return socket file descriptor
    return temporary;
}

struct slclTcpSock* slclOpenTcpSock( const char* ip, short port )
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
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    
    // return new socket if all has gone well
    struct slclTcpSock* outsock = malloc( sizeof(struct slclTcpSock) );
    outsock->fd = temporary;
    return outsock;

}
slclerr_t slclSendTcpSock( struct slclTcpSock* sock, const char* data, size_t bytes )
{
    slclerr_t err;

    if ((err = send(sock->fd, data, bytes, 0)) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }

    return err;

}

slclerr_t slclRecvTcpSock( struct slclTcpSock* sock, char* buffer, size_t bytes )
{
    slclerr_t recved;
    if ((recved = recv( sock->fd, buffer, bytes, 0)) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return recved;
}
slclerr_t slclRecvTcpSockExact( struct slclTcpSock* sock, char* buffer, size_t bytes )
{
    slclerr_t recved = 0;
    while (recved < bytes)
    {
        slclerr_t recviter = 0;
        if ((recviter = recv(sock->fd, buffer, bytes - recved, 0)) == -1)
        {
            slclSeterr( strerror( errno ) );
            return SLCL_ERROR;
        }
        recved += recviter;
    }
    return bytes;
}


struct slclTcpServer* slclOpenTcpServer( const char* ip, short port )
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
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    // try to listen
    if ((err = listen( temporary, SOMAXCONN )) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    // return new server if all has gone well
    struct slclTcpServer* outsock = malloc( sizeof(struct slclTcpServer) );
    outsock->fd = temporary;
    return outsock;

}

struct slclTcpSock* slclAcceptTcpServer( struct slclTcpServer* server )
{

    slclerr_t err;
    struct sockaddr_storage addr;
    socklen_t size;

    // Attempt unix accept()
    // (blocking)
    if ((err = accept( server->fd, (struct sockaddr*) &addr, &size )) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    // Once accept has found a client, it is returned here
    struct slclTcpSock* outsock = malloc( sizeof( struct slclTcpSock ) );
    outsock->fd = err;
    return outsock;

}



#ifdef __cpluscplus
SLCL_EXITCPP
#endif


#endif

