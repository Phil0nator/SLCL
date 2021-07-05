# Simple Lightweight Cross-Platform Layer

The Simple Lightweight Cross-Platform Layer library or 'SLCL' provides a minimal interface for os-dependent tasks in C/C++. The goal of SLCL is to make common, and simple tasks that are os-dependent a little easier in C/C++. For example, a basic AF_INET stream socket tcp setup already written for either *nix based machines or windows could be make cross platform using SLCL, largely by prepending the 'slcl' prefix to function calls, and ammending some details. 

e.g:
### Linux only C
```C
#include <sys/socket.h>
int sock = socket(AF_INET, SOCK_STREAM, 0);
```

### Cross platform SLCL
```C
#include "SLCL/Network.h"
struct slclSock* sock slclOpenSock( SLCL_AF_INET, SLCL_SOCK_STREAM, 0 );
```

## Features

Currently, SLCL has abstractions for the following tasks
- Sockets
- Signals
- Dynamic Libraries
- Os-dependent error messages
- Memory and file mapping

maybe coming soon
- ioctl

## Compiling

CmakeLists are included.

### unix-based:

```bash
mkdir build && cd build
cmake ..
make
```

## Linking

The CmakeLists are by default setup to produce a static library called libSLCL.a