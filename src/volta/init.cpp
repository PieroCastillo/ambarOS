#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.hpp>
#include <vgaTerminal.hpp>
#include <interruptHandlers.hpp>

ambarOSfn void kernelMain(void)
{
	terminalInitialize();
	terminalWriteline("ambarOS alpha v0.0.1");
	terminalWriteline("volta kernel alpha v0.0.1");
	Volta::ConfigureExceptions();
	terminalWriteline("Hello, World!");
	// terminalWriteline("MENA OLA!");

	while (1)
	  	Volta::Platform::halt();
}