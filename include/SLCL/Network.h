#ifndef SLCL_NET
#define SLCL_NET

#include "Config.h"

// Networking

#ifdef __cplusplus
SLCL_ENTERCPP
#endif

#define SLCL_LOCALHOST "127.0.0.1"

// Opaque types:
struct tcpsock;
struct tcppacket;
struct tcpserver;

struct udpsock;

// TCP Functions

void free_tcppacket( struct tcppacket* packet );
size_t sizeof_tcppacket( struct tcppacket* packet );
char* read_tcppacket( struct tcppacket* packet );
char* disown_tcppacket( struct tcppacket* packet );


struct tcpsock* open_tcpsock( const char* ip, short port );
struct tcpserver* open_tcpserver( const char* ip, short port );
struct tcpsock* accept_tcpserver( struct tcpserver* server );


slclerr_t send_tcpsock( struct tcpsock* sock, const char* data, size_t bytes );
slclerr_t send_tcpsockpack( struct tcpsock* sock, struct tcppacket* data );

struct tcppacket* recv_tcpsock( struct tcpsock* sock, size_t bytes );
struct tcppacket* recv_tcpsock_exact( struct tcpsock* sock, size_t bytes );


slclerr_t close_tcpsock( struct tcpsock* socket );
slclerr_t close_tcpserver( struct tcpserver* server );


#ifdef __cplusplus
SLCL_EXITCPP
#endif


#endif