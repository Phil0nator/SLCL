#pragma once



extern void slclSeterr( const char* msg );
extern char* slclGetErrorBuf();

#define SLCL_ERRORBUF_SIZE 1024
