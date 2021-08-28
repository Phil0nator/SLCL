#include "SLCL/Signals.h"
#include "SLCL/Errors.h"
#include <stdio.h>
#include <stdlib.h>

// define a signal handler
int handler(int x)
{
	puts("Interrupt.");
	return 0;
}

int main(int argc, char** argv)
{

	// This causes a silly popup in debug mode on windows, but the interrupt
	// will still work after you press "ignore"
	if (slclSigaction(SLCL_SIGABRT, handler) == SLCL_FAILED)
	{
		slclPerror("signal");
		exit(1);
	}
	// cause SIGABRT
	abort();

}