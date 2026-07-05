#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.hpp>
#include <vgaTerminal.hpp>

// x64
struct IDTEntry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
};

struct IDTR
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

alignas(16)
IDTEntry idt[256];

IDTR idtr = {
    .limit = sizeof(idt) - 1,
    .base  = (uint64_t)idt
};

void setGate(int vector, void (*handler)(), uint16_t selector, uint8_t ist, uint8_t flags)
{
    uint64_t addr = (uint64_t)handler;

    idt[vector].offset_low  = addr;
    idt[vector].selector    = selector;
    idt[vector].ist         = ist;
    idt[vector].type_attr   = flags;
    idt[vector].offset_mid  = addr >> 16;
    idt[vector].offset_high = addr >> 32;
}

void handleCustomInterruption()
{
	terminalWriteline("interrupted!");
}

ambarOSfn void kernelMain(void)
{
	terminalInitialize();
	terminalWriteline("ambarOS alpha v0.0.1");
	terminalWriteline("volta kernel alpha v0.0.1");
	setGate(40, &handleCustomInterruption, 0x08,0, 0x8E);
	asm volatile("lidt %0" :: "m"(idtr));
	terminalWriteline("Hello, World!");
	asm volatile("int 40");
	// terminalWriteline("MENA OLA!");

	while (1)
	  	Volta::Platform::halt();
}