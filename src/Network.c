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
#   include <unistd.h>



const slclAddressFamily SLCL_AF_UNSPEC = AF_UNSPEC;
const slclAddressFamily SLCL_AF_LOCAL = AF_LOCAL;
const slclAddressFamily SLCL_AF_UNIX = AF_UNIX;
const slclAddressFamily SLCL_AF_FILE = AF_FILE;
const slclAddressFamily SLCL_AF_INET = AF_INET;
const slclAddressFamily SLCL_AF_AX25 = AF_AX25;
const slclAddressFamily SLCL_AF_IPX = AF_IPX;
const slclAddressFamily SLCL_AF_APPLETALK = AF_APPLETALK;
const slclAddressFamily SLCL_AF_NETROM = AF_NETROM;
const slclAddressFamily SLCL_AF_BRIDGE = AF_BRIDGE;
const slclAddressFamily SLCL_AF_ATMPVC = AF_ATMPVC;
const slclAddressFamily SLCL_AF_X25 = AF_X25;
const slclAddressFamily SLCL_AF_INET6 = AF_INET6;
const slclAddressFamily SLCL_AF_ROSE = AF_ROSE;
const slclAddressFamily SLCL_AF_DECnet = AF_DECnet;
const slclAddressFamily SLCL_AF_NETBEUI = AF_NETBEUI;
const slclAddressFamily SLCL_AF_SECURITY = AF_SECURITY;
const slclAddressFamily SLCL_AF_KEY = AF_KEY;
const slclAddressFamily SLCL_AF_NETLINK = AF_NETLINK;
const slclAddressFamily SLCL_AF_ROUTE = AF_ROUTE;
const slclAddressFamily SLCL_AF_PACKET = AF_PACKET;
const slclAddressFamily SLCL_AF_ASH = AF_ASH;
const slclAddressFamily SLCL_AF_ECONET = AF_ECONET;
const slclAddressFamily SLCL_AF_ATMSVC = AF_ATMSVC;
const slclAddressFamily SLCL_AF_RDS = AF_RDS;
const slclAddressFamily SLCL_AF_SNA = AF_SNA;
const slclAddressFamily SLCL_AF_IRDA = AF_IRDA;
const slclAddressFamily SLCL_AF_PPPOX = AF_PPPOX;
const slclAddressFamily SLCL_AF_WANPIPE = AF_WANPIPE;
const slclAddressFamily SLCL_AF_LLC = AF_LLC;
const slclAddressFamily SLCL_AF_IB = AF_IB;
const slclAddressFamily SLCL_AF_MPLS = AF_MPLS;
const slclAddressFamily SLCL_AF_CAN = AF_CAN;
const slclAddressFamily SLCL_AF_TIPC = AF_TIPC;
const slclAddressFamily SLCL_AF_BLUETOOTH = AF_BLUETOOTH;
const slclAddressFamily SLCL_AF_IUCV = AF_IUCV;
const slclAddressFamily SLCL_AF_RXRPC = AF_RXRPC;
const slclAddressFamily SLCL_AF_ISDN = AF_ISDN;
const slclAddressFamily SLCL_AF_PHONET = AF_PHONET;
const slclAddressFamily SLCL_AF_IEEE802154 = AF_IEEE802154;
const slclAddressFamily SLCL_AF_CAIF = AF_CAIF;
const slclAddressFamily SLCL_AF_ALG = AF_ALG;
const slclAddressFamily SLCL_AF_NFC = AF_NFC;
const slclAddressFamily SLCL_AF_VSOCK = AF_VSOCK;
const slclAddressFamily SLCL_AF_KCM = AF_KCM;
const slclAddressFamily SLCL_AF_QIPCRTR = AF_QIPCRTR;
const slclAddressFamily SLCL_AF_SMC = AF_SMC;
const slclAddressFamily SLCL_AF_XDP = AF_XDP;
const slclAddressFamily SLCL_AF_MAX = AF_MAX;

const slclSockProtocol SLCL_PF_UNSPEC = PF_UNSPEC;
const slclSockProtocol SLCL_PF_LOCAL = PF_LOCAL;
const slclSockProtocol SLCL_PF_UNIX = PF_UNIX;
const slclSockProtocol SLCL_PF_FILE = PF_FILE;
const slclSockProtocol SLCL_PF_INET = PF_INET;
const slclSockProtocol SLCL_PF_AX25 = PF_AX25;
const slclSockProtocol SLCL_PF_IPX = PF_IPX;
const slclSockProtocol SLCL_PF_APPLETALK = PF_APPLETALK;
const slclSockProtocol SLCL_PF_NETROM = PF_NETROM;
const slclSockProtocol SLCL_PF_BRIDGE = PF_BRIDGE;
const slclSockProtocol SLCL_PF_ATMPVC = PF_ATMPVC;
const slclSockProtocol SLCL_PF_X25 = PF_X25;
const slclSockProtocol SLCL_PF_INET6 = PF_INET6;
const slclSockProtocol SLCL_PF_ROSE = PF_ROSE;
const slclSockProtocol SLCL_PF_DECnet = PF_DECnet;
const slclSockProtocol SLCL_PF_NETBEUI = PF_NETBEUI;
const slclSockProtocol SLCL_PF_SECURITY = PF_SECURITY;
const slclSockProtocol SLCL_PF_KEY = PF_KEY;
const slclSockProtocol SLCL_PF_NETLINK = PF_NETLINK;
const slclSockProtocol SLCL_PF_ROUTE = PF_ROUTE;
const slclSockProtocol SLCL_PF_PACKET = PF_PACKET;
const slclSockProtocol SLCL_PF_ASH = PF_ASH;
const slclSockProtocol SLCL_PF_ECONET = PF_ECONET;
const slclSockProtocol SLCL_PF_ATMSVC = PF_ATMSVC;
const slclSockProtocol SLCL_PF_RDS = PF_RDS;
const slclSockProtocol SLCL_PF_SNA = PF_SNA;
const slclSockProtocol SLCL_PF_IRDA = PF_IRDA;
const slclSockProtocol SLCL_PF_PPPOX = PF_PPPOX;
const slclSockProtocol SLCL_PF_WANPIPE = PF_WANPIPE;
const slclSockProtocol SLCL_PF_LLC = PF_LLC;
const slclSockProtocol SLCL_PF_IB = PF_IB;
const slclSockProtocol SLCL_PF_MPLS = PF_MPLS;
const slclSockProtocol SLCL_PF_CAN = PF_CAN;
const slclSockProtocol SLCL_PF_TIPC = PF_TIPC;
const slclSockProtocol SLCL_PF_BLUETOOTH = PF_BLUETOOTH;
const slclSockProtocol SLCL_PF_IUCV = PF_IUCV;
const slclSockProtocol SLCL_PF_RXRPC = PF_RXRPC;
const slclSockProtocol SLCL_PF_ISDN = PF_ISDN;
const slclSockProtocol SLCL_PF_PHONET = PF_PHONET;
const slclSockProtocol SLCL_PF_IEEE802154 = PF_IEEE802154;
const slclSockProtocol SLCL_PF_CAIF = PF_CAIF;
const slclSockProtocol SLCL_PF_ALG = PF_ALG;
const slclSockProtocol SLCL_PF_NFC = PF_NFC;
const slclSockProtocol SLCL_PF_VSOCK = PF_VSOCK;
const slclSockProtocol SLCL_PF_KCM = PF_KCM;
const slclSockProtocol SLCL_PF_QIPCRTR = PF_QIPCRTR;
const slclSockProtocol SLCL_PF_SMC = PF_SMC;
const slclSockProtocol SLCL_PF_XDP = PF_XDP;
const slclSockProtocol SLCL_PF_MAX = PF_MAX;

const slclSockType SLCL_SOCK_STREAM = SOCK_STREAM;
const slclSockType SLCL_SOCK_DGRAM = SOCK_DGRAM;
const slclSockType SLCL_SOCK_RAW = SOCK_RAW;
const slclSockType SLCL_SOCK_RDM = SOCK_RDM;
const slclSockType SLCL_SOCK_SEQPACKET = SOCK_SEQPACKET;
const slclSockType SLCL_SOCK_DCCP = SOCK_DCCP;
const slclSockType SLCL_SOCK_PACKET = SOCK_PACKET;
const slclSockType SLCL_SOCK_CLOEXEC = SOCK_CLOEXEC;
const slclSockType SLCL_SOCK_NONBLOCK = SOCK_NONBLOCK;


typedef int fd_t;

typedef struct slclSock
{
    fd_t fd;
} slclSock_t;

typedef struct slclSockaddr_in
{
    struct sockaddr_in addr;
} slclSockaddr_in_t;


struct slclSockaddr_in* slclCreateAddress( slclAddressFamily domain, int address, short port )
{
    slclSockaddr_in_t* out = malloc(sizeof(slclSockaddr_in_t));
    out->addr.sin_addr.s_addr = address;
    out->addr.sin_family = domain;
    out->addr.sin_port = port;
    return out;
}
struct slclSockaddr_in* slclCreateStrAddress( slclAddressFamily domain, const char* address, short port )
{
    slclSockaddr_in_t* out = slclCreateAddress(domain, 0, port);
    slclerr_t err;
    if ((err = inet_aton( address, &out->addr.sin_addr )) == 0)
    {
        slclSeterr( "Invalid IP address." );
        return SLCL_FAILED;
    }
    return out;
}

slclerr_t slclShutdown( struct slclSock* socket, enum slclShutdownMethod how )
{
    slclerr_t error = shutdown(socket->fd, how);
    if (error == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}

slclerr_t slclCloseSock( struct slclSock* socket )
{
    if (close( socket->fd ) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    free(socket);
    return SLCL_SUCCESS;
}


struct slclSock* slclOpenSock( slclAddressFamily domain, slclSockType type, slclSockProtocol protocol  )
{
    slclerr_t err;
    // open a standard tcp socket
    fd_t temporary = socket( domain, type, protocol );
    if (temporary == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_FAILED;
    }
    struct slclSock* outsock = malloc( sizeof(struct slclSock) );
    outsock->fd = temporary;
    return outsock;
}

slclerr_t slclConnect( struct slclSock* sock, struct slclSockaddr_in* addr )
{
    slclerr_t err;

    // Try to connect
    if ((err = connect( sock->fd, (struct sockaddr*) &addr->addr, sizeof(struct sockaddr_in) )) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }


    return SLCL_SUCCESS;
}

slclerr_t slclSend( struct slclSock* sock, const char* data, size_t bytes )
{
    slclerr_t err;

    if ((err = send(sock->fd, data, bytes, 0)) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }

    return err;

}

slclerr_t slclRecv( struct slclSock* sock, char* buffer, size_t bytes )
{
    slclerr_t recved;
    if ((recved = recv( sock->fd, buffer, bytes, 0)) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return recved;
}

slclerr_t slclBind( struct slclSock* socket, struct slclSockaddr_in* addr )
{
    // try to bind socket
    slclerr_t err;
    if ((err = bind( socket->fd, (struct sockaddr*) &addr->addr, sizeof(struct sockaddr_in) )) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}

slclerr_t slclListen( struct slclSock* socket, int backlog )
{
    // try to listen
    slclerr_t err;
    if ((err = listen( socket->fd, backlog )) == -1)
    {
        slclSeterr( strerror( errno ) );
        return SLCL_ERROR;
    }
    return SLCL_SUCCESS;
}


struct slclSock* slclAccept( struct slclSock* server )
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
    struct slclSock* outsock = malloc( sizeof( struct slclSock ) );
    outsock->fd = err;
    return outsock;

}



#ifdef __cpluscplus
SLCL_EXITCPP
#endif


#endif

