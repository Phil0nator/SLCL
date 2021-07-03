#ifndef SLCL_NET
#define SLCL_NET

#include "Config.h"

// Networking

#ifdef __cplusplus
SLCL_ENTERCPP
#endif

#define SLCL_LOCALHOST "127.0.0.1"

// Opaque types:

/**
 * A type representing an os-dependent tcp socket type. 
 */
struct slclTcpSock;
/**
 * A type representing an os-dependent tcp socket bound to an address,
 * and listening for connections
 */
struct slclTcpServer;

/**
 * A type representing an os-dependent udp socket type
 */
struct slclUdpSock;

// General decls

typedef enum slclShutdownMethod
{
    SLCL_SHUTDOWN_READ = 0,
    SLCL_SHUTDOWN_WRITE = 1,
    SLCL_SHUTDOWN_ALL = 2
} slclShutdownMethod_t;


// TCP Functions

/**
 * slclOpenTcpSock creates a TCP socket and initiates a connection to the 
 * given ip address and port. (the port is expected in host order).
 * @param ip a string representing the IP as #.#.#.# 
 * @param port the host port (host order)
 * @returns a slclTcpSock struct which will represent the underlying OS implimentation.
 * The resources used by this slclTcpSock will be automatically freed after a call to slclCloseTcpSock.
 */
struct slclTcpSock* slclOpenTcpSock( const char* ip, short port );

/**
 * slclOpenTcpServer creates a TCP socket and begins listening for incoming connections
 * on the given ip address and port. (the port is expected in host order).
 * @param ip a string representing the IP as #.#.#.# 
 * @param port the host port (host order)
 * @returns a slclTcpServer struct which will represent the underlying OS implimentation.
 * The resources used by this slclTcpServer will be automatically freed after a call to slclCloseTcpSock.
 */
struct slclTcpServer* slclOpenTcpServer( const char* ip, short port );

/**
 * slclAcceptTcpServer is used to accept incoming connections over a slclTcpServer struct
 * as new slclTcpSock structs.
 * @see slclTcpSock
 * @see slclTcpServer
 * 
 * @param server the server through which to accept
 * @returns a new slclTcpSock structure after blocking until a new connection arrives.
 */
struct slclTcpSock* slclAcceptTcpServer( struct slclTcpServer* server );

/**
 * slclSendTcpSock is used to send data through a tcp socket.
 * @param sock the socket to send data through
 * @param data a pointer to some data to send
 * @param bytes the number of bytes to send
 * @returns the number of bytes sent, or SLCL_ERROR if an error occured.
 * @see slclStrerr
 */
slclerr_t slclSendTcpSock( struct slclTcpSock* sock, const char* data, size_t bytes );

/**
 * slclRecvTcpSock is used to receive data through a given socket.
 * @param sock the socket to receive through
 * @param buffer a buffer of size 'bytes' or greater
 * @param bytes the maximum number of bytes to receive
 * @returns the number of bytes received, or SLCL_ERROR if an error occured.
 * @see slclStrerr
 */
slclerr_t slclRecvTcpSock( struct slclTcpSock* sock, char* buffer, size_t bytes );

/**
 * slclRecvTcpSock, similar to slclRecvTcpSock, is used to receive a precise
 * number of bytes through a socket.
 * slclRecvTcpSockExact will block until the given number of bytes has been received,
 * or an error occurs.
 * @param sock the socket to receive through
 * @param buffer the destination buffer of size 'bytes' or greater
 * @param bytes the precise number of bytes to wait for
 * @returns either 'bytes', or SLCL_ERROR if an error occured.
 * @see slclStrerr
 */
slclerr_t slclRecvTcpSockExact( struct slclTcpSock* sock, char* buffer, size_t bytes );

/**
 * slclCloseTcpSock will free all the resources being used by a tcp socket.
 * @param socket a slclTcpSock
 * @returns
 */
slclerr_t slclCloseTcpSock( struct slclTcpSock* socket, enum slclShutdownMethod how );

/**
 * slclCloseTcpSock will free all the resources being used by a tcp server. 
 * ( this does NOT include any clients that may have been created through
 *  calls to slclAcceptTcpServer )
 * @param socket a slclTcpSock
 * @returns
 */
slclerr_t slclCloseTcpServer( struct slclTcpServer* server, enum slclShutdownMethod how );


#ifdef __cplusplus
SLCL_EXITCPP
#endif


#endif