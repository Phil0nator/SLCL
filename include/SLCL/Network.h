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
struct slclSock;
struct slclSockaddr_in;

typedef int slclAddressFamily;
typedef int slclSockProtocol;
typedef int slclSockType;

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


//  Functions
struct slclSockaddr_in* slclCreateAddress( slclAddressFamily domain, int address, short port );
struct slclSockaddr_in* slclCreateStrAddress( slclAddressFamily domain, const char* address, short port );
void slclFreeAddress(struct slclSockaddr_in* addr);

/**
 * slclOpenTcpSock creates a TCP socket and initiates a connection to the 
 * given ip address and port. (the port is expected in host order).
 * @param ip a string representing the IP as #.#.#.# 
 * @param port the host port (host order)
 * @returns a slclTcpSock struct which will represent the underlying OS implimentation.
 * The resources used by this slclTcpSock will be automatically freed after a call to slclCloseTcpSock.
 */
struct slclSock* slclOpenSock( slclAddressFamily domain, slclSockType type, slclSockProtocol protocol  );

slclerr_t slclConnect( struct slclSock* sock, struct slclSockaddr_in* addr );

/**
 * slclAcceptTcpServer is used to accept incoming connections over a slclTcpServer struct
 * as new slclTcpSock structs.
 * @see slclTcpSock
 * @see slclTcpServer
 * 
 * @param server the server through which to accept
 * @returns a new slclTcpSock structure after blocking until a new connection arrives.
 */
struct slclSock* slclAccept( struct slclSock* server );

/**
 * slclSendTcpSock is used to send data through a tcp socket.
 * @param sock the socket to send data through
 * @param data a pointer to some data to send
 * @param bytes the number of bytes to send
 * @returns the number of bytes sent, or SLCL_ERROR if an error occured.
 * @see slclStrerr
 */
slclerr_t slclSend( struct slclSock* sock, const char* data, size_t bytes );

/**
 * slclRecvTcpSock is used to receive data through a given socket.
 * @param sock the socket to receive through
 * @param buffer a buffer of size 'bytes' or greater
 * @param bytes the maximum number of bytes to receive
 * @returns the number of bytes received, or SLCL_ERROR if an error occured.
 * @see slclStrerr
 */
slclerr_t slclRecv( struct slclSock* sock, char* buffer, size_t bytes );


/**
 * slclCloseTcpSock will free all the resources being used by a tcp socket.
 * @param socket a slclTcpSock
 * @returns
 */
slclerr_t slclShutdown( struct slclSock* socket, enum slclShutdownMethod how );

slclerr_t slclBind( struct slclSock* socket, struct slclSockaddr_in* addr );

slclerr_t slclListen( struct slclSock* socket, int backlog );

slclerr_t slclCloseSock( struct slclSock* socket );


#ifdef __cplusplus
SLCL_EXITCPP
#endif


#endif