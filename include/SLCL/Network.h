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
 * A type representing an os-dependent socket type. 
 */
struct slclSock;
struct slclSockaddr_in;

typedef int slclAddressFamily;
typedef int slclSockProtocol;
typedef int slclSockType;
typedef int slclSockopt;
typedef int slclSockoptLevel;

// General decls

typedef enum slclShutdownMethod
{
    SLCL_SHUTDOWN_READ = 0,
    SLCL_SHUTDOWN_WRITE = 1,
    SLCL_SHUTDOWN_ALL = 2
} slclShutdownMethod_t;


extern const slclAddressFamily SLCL_AF_UNSPEC;
extern const slclAddressFamily SLCL_AF_LOCAL;
extern const slclAddressFamily SLCL_AF_UNIX;
extern const slclAddressFamily SLCL_AF_FILE;
extern const slclAddressFamily SLCL_AF_INET;
extern const slclAddressFamily SLCL_AF_AX25;
extern const slclAddressFamily SLCL_AF_IPX;
extern const slclAddressFamily SLCL_AF_APPLETALK;
extern const slclAddressFamily SLCL_AF_NETROM;
extern const slclAddressFamily SLCL_AF_BRIDGE;
extern const slclAddressFamily SLCL_AF_ATMPVC;
extern const slclAddressFamily SLCL_AF_X25;
extern const slclAddressFamily SLCL_AF_INET6;
extern const slclAddressFamily SLCL_AF_ROSE;
extern const slclAddressFamily SLCL_AF_DECnet;
extern const slclAddressFamily SLCL_AF_NETBEUI;
extern const slclAddressFamily SLCL_AF_SECURITY;
extern const slclAddressFamily SLCL_AF_KEY;
extern const slclAddressFamily SLCL_AF_NETLINK;
extern const slclAddressFamily SLCL_AF_ROUTE;
extern const slclAddressFamily SLCL_AF_PACKET;
extern const slclAddressFamily SLCL_AF_ASH;
extern const slclAddressFamily SLCL_AF_ECONET;
extern const slclAddressFamily SLCL_AF_ATMSVC;
extern const slclAddressFamily SLCL_AF_RDS;
extern const slclAddressFamily SLCL_AF_SNA;
extern const slclAddressFamily SLCL_AF_IRDA;
extern const slclAddressFamily SLCL_AF_PPPOX;
extern const slclAddressFamily SLCL_AF_WANPIPE;
extern const slclAddressFamily SLCL_AF_LLC;
extern const slclAddressFamily SLCL_AF_IB;
extern const slclAddressFamily SLCL_AF_MPLS;
extern const slclAddressFamily SLCL_AF_CAN;
extern const slclAddressFamily SLCL_AF_TIPC;
extern const slclAddressFamily SLCL_AF_BLUETOOTH;
extern const slclAddressFamily SLCL_AF_IUCV;
extern const slclAddressFamily SLCL_AF_RXRPC;
extern const slclAddressFamily SLCL_AF_ISDN;
extern const slclAddressFamily SLCL_AF_PHONET;
extern const slclAddressFamily SLCL_AF_IEEE802154;
extern const slclAddressFamily SLCL_AF_CAIF;
extern const slclAddressFamily SLCL_AF_ALG;
extern const slclAddressFamily SLCL_AF_NFC;
extern const slclAddressFamily SLCL_AF_VSOCK;
extern const slclAddressFamily SLCL_AF_KCM;
extern const slclAddressFamily SLCL_AF_QIPCRTR;
extern const slclAddressFamily SLCL_AF_SMC;
extern const slclAddressFamily SLCL_AF_XDP;
extern const slclAddressFamily SLCL_AF_MAX;

extern const slclSockProtocol SLCL_PF_UNSPEC;
extern const slclSockProtocol SLCL_PF_LOCAL;
extern const slclSockProtocol SLCL_PF_UNIX;
extern const slclSockProtocol SLCL_PF_FILE;
extern const slclSockProtocol SLCL_PF_INET;
extern const slclSockProtocol SLCL_PF_AX25;
extern const slclSockProtocol SLCL_PF_IPX;
extern const slclSockProtocol SLCL_PF_APPLETALK;
extern const slclSockProtocol SLCL_PF_NETROM;
extern const slclSockProtocol SLCL_PF_BRIDGE;
extern const slclSockProtocol SLCL_PF_ATMPVC;
extern const slclSockProtocol SLCL_PF_X25;
extern const slclSockProtocol SLCL_PF_INET6;
extern const slclSockProtocol SLCL_PF_ROSE;
extern const slclSockProtocol SLCL_PF_DECnet;
extern const slclSockProtocol SLCL_PF_NETBEUI;
extern const slclSockProtocol SLCL_PF_SECURITY;
extern const slclSockProtocol SLCL_PF_KEY;
extern const slclSockProtocol SLCL_PF_NETLINK;
extern const slclSockProtocol SLCL_PF_ROUTE;
extern const slclSockProtocol SLCL_PF_PACKET;
extern const slclSockProtocol SLCL_PF_ASH;
extern const slclSockProtocol SLCL_PF_ECONET;
extern const slclSockProtocol SLCL_PF_ATMSVC;
extern const slclSockProtocol SLCL_PF_RDS;
extern const slclSockProtocol SLCL_PF_SNA;
extern const slclSockProtocol SLCL_PF_IRDA;
extern const slclSockProtocol SLCL_PF_PPPOX;
extern const slclSockProtocol SLCL_PF_WANPIPE;
extern const slclSockProtocol SLCL_PF_LLC;
extern const slclSockProtocol SLCL_PF_IB;
extern const slclSockProtocol SLCL_PF_MPLS;
extern const slclSockProtocol SLCL_PF_CAN;
extern const slclSockProtocol SLCL_PF_TIPC;
extern const slclSockProtocol SLCL_PF_BLUETOOTH;
extern const slclSockProtocol SLCL_PF_IUCV;
extern const slclSockProtocol SLCL_PF_RXRPC;
extern const slclSockProtocol SLCL_PF_ISDN;
extern const slclSockProtocol SLCL_PF_PHONET;
extern const slclSockProtocol SLCL_PF_IEEE802154;
extern const slclSockProtocol SLCL_PF_CAIF;
extern const slclSockProtocol SLCL_PF_ALG;
extern const slclSockProtocol SLCL_PF_NFC;
extern const slclSockProtocol SLCL_PF_VSOCK;
extern const slclSockProtocol SLCL_PF_KCM;
extern const slclSockProtocol SLCL_PF_QIPCRTR;
extern const slclSockProtocol SLCL_PF_SMC;
extern const slclSockProtocol SLCL_PF_XDP;
extern const slclSockProtocol SLCL_PF_MAX;

extern const slclSockType SLCL_SOCK_STREAM;
extern const slclSockType SLCL_SOCK_DGRAM;
extern const slclSockType SLCL_SOCK_RAW;
extern const slclSockType SLCL_SOCK_RDM;
extern const slclSockType SLCL_SOCK_SEQPACKET;
extern const slclSockType SLCL_SOCK_DCCP;
extern const slclSockType SLCL_SOCK_PACKET;
extern const slclSockType SLCL_SOCK_CLOEXEC;
extern const slclSockType SLCL_SOCK_NONBLOCK;

extern const slclSockoptLevel SLCL_SOL_IPPROTO_IP;
extern const slclSockoptLevel SLCL_SOL_IPPROTO_IPV6;
extern const slclSockoptLevel SLCL_SOL_IPPROTO_ICMP;
extern const slclSockoptLevel SLCL_SOL_IPPROTO_RAW;
extern const slclSockoptLevel SLCL_SOL_IPPROTO_TCP;
extern const slclSockoptLevel SLCL_SOL_IPPROTO_UDP;
extern const slclSockoptLevel SLCL_SOL_SOCKET;

extern const slclSockopt SLCL_SO_DEBUG;
extern const slclSockopt SLCL_SO_BROADCAST;
extern const slclSockopt SLCL_SO_REUSEADDR;
extern const slclSockopt SLCL_SO_KEEPALIVE;
extern const slclSockopt SLCL_SO_LINGER;
extern const slclSockopt SLCL_SO_OOBINLINE;
extern const slclSockopt SLCL_SO_SNDBUF;
extern const slclSockopt SLCL_SO_RCVBUF;
extern const slclSockopt SLCL_SO_DONTROUTE;
extern const slclSockopt SLCL_SO_RCVLOWAT;
extern const slclSockopt SLCL_SO_RCVTIMEO;
extern const slclSockopt SLCL_SO_SNDLOWAT;
extern const slclSockopt SLCL_SO_SNDTIMEO;


//  Functions


/**
 * slclCreateAddress will create a new sockaddr type based on a domain, raw address, and port;
 * ( If you are looking to make an address from a string-represented ip and port, you
 * should use slclCreateStrAddress() ).
 * 
 * @param domain an address family for the address
 * @param address a raw address
 * @param port the port number ( in network order )
 * @returns the abstracted slclSockaddr_in pointer, which can be freed with slclFreeAddress
 */
struct slclSockaddr_in* slclCreateAddress( slclAddressFamily domain, int address, short port );

/**
 * slclCreateStrAddress creates a slclSockaddr_in type from an ip and port;
 * 
 * @param domain an address family ( I.E AF_INET )
 * @param address the ip address as: #.#.#.#
 * @param port the port number ( in network order )
 * @returns the abstracted slclSockaddr_in pointer, which can be freed with slclFreeAddress
 */
struct slclSockaddr_in* slclCreateStrAddress( slclAddressFamily domain, const char* address, short port );
void slclFreeAddress(struct slclSockaddr_in* addr);

/**
 * slclOpenSock creates a socket object with the given parameters.
 * @param domain the address family of the socket. E.g: AF_INET
 * @param type socket type. E.g: SOCK_STREAM
 * @param protocol an optional socket protocol
 * @returns a new slclSock object, or SLCL_FAILED if an error occured. 
 * The resources used by the returned slclSock can be freed using slclCloseSock()
 */
struct slclSock* slclOpenSock( slclAddressFamily domain, slclSockType type, slclSockProtocol protocol  );

/**
 * Begin a connection between an slclSock and a given slcl address.
 * @param sock a socket to connect
 * @param addr an address to use
 * @returns either SLCL_SUCCESS, or SLCL_ERROR
 */
slclerr_t slclConnect( struct slclSock* sock, struct slclSockaddr_in* addr );

/**
 * slclAccept is used to accept incoming connections over a slclSock struct.
 * @see slclBind
 * @see slclListen
 * 
 * @param server a slclSock that is bound and listening
 * @returns either a new slclSock connection, or SLCL_FAILED if an error occured.
 * slclAccept will block unitl a new connection is ready.
 */
struct slclSock* slclAccept( struct slclSock* server );

/**
 * slclSend is used to send data through a socket.
 * @param sock the socket to send data through
 * @param data a pointer to some data to send
 * @param bytes the number of bytes to send
 * @returns the number of bytes sent, or SLCL_ERROR if an error occured.
 * @see slclStrerr
 */
slclerr_t slclSend( struct slclSock* sock, const char* data, size_t bytes );

/**
 * slclRecv is used to receive data through a given socket.
 * @param sock the socket to receive through
 * @param buffer a buffer of size 'bytes' or greater
 * @param bytes the maximum number of bytes to receive
 * @returns the number of bytes received, or SLCL_ERROR if an error occured.
 * @see slclStrerr
 */
slclerr_t slclRecv( struct slclSock* sock, char* buffer, size_t bytes );


/**
 * slclShutdown will call the os-dependent version of shutdown(2),
 * which will base its behavior on the how argument.
 * 
 * slclShutdown does NOT free resources used by an slclSocket,
 * for that @see slclCloseSock
 * 
 * @param socket a slclSock to shutdown
 * @param how a slclShutdownMethod to use to shutdown the socket
 * @returns either SLCL_SUCCESS or SLCL_ERROR
 * 
 */
slclerr_t slclShutdown( struct slclSock* socket, enum slclShutdownMethod how );

/**
 * slclBind will bind an slclSock to an address as a server.
 * @param socket a socket to bind
 * @param addr the address to bind to
 * @returns either SLCL_SUCCESS or SLCL_ERROR
 */
slclerr_t slclBind( struct slclSock* socket, struct slclSockaddr_in* addr );

/**
 * slclListen will begin listening on a socket for incoming connetions.
 * @param socket the socket to listen on
 * @param backlog the number of incoming connections to accept
 * @returns either SLCL_SUCCESS or SLCL_ERROR
 */
slclerr_t slclListen( struct slclSock* socket, int backlog );

/**
 * slclCloseSock will close any connection that is active on socket,
 * and free all resources used by the socket.
 * @param socket the socket to close
 * @warning (after a call to slclCloseSock, the given pointer becomes invalid and further
 * dereferences are undefined behavior.)
 */
slclerr_t slclCloseSock( struct slclSock* socket );

/**
 * slclSetSockOpt is an abstraction for the standard setsockopt() function.
 * @param socket the socket to apply the new information to
 * @param level level of application ( e.g SLCL_SOL_SOCKET )
 * @param option_name the sockopt
 * @param option_value the value for the requested option, if applicable
 * @param option_len the size of the memory given in option_value
 * @return either SLCL_SUCCESS or SLCL_ERROR
 */
slclerr_t slclSetSockopt( 
    struct slclSock* socket, 
    slclSockoptLevel level, 
    slclSockopt option_name, 
    const void* option_value, 
    size_t option_len );

#ifdef __cplusplus
SLCL_EXITCPP
#endif


#endif