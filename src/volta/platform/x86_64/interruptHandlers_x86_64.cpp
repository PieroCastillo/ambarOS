/*
Volta x86-64 Interruption Handling

Reference:
Intel Manual Vol3, 7.3.1 External Interrupts
*/

// standard headers
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// kernel headers
#include <interruptHandlers.hpp>

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

alignas(16) IDTEntry idt[256];

IDTR idtr = {
    .limit = sizeof(idt) - 1,
    .base = (uint64_t)idt
};

void setGate(int vector, void (*handler)(), uint16_t selector, uint8_t ist, uint8_t flags)
{
    uint64_t addr = (uint64_t)handler;

    idt[vector].offset_low = addr;
    idt[vector].selector = selector;
    idt[vector].ist = ist;
    idt[vector].type_attr = flags;
    idt[vector].offset_mid = addr >> 16;
    idt[vector].offset_high = addr >> 32;
}

void Volta::ConfigureExceptions()
{
    setGate(0, &handleExceptionDivideError, 0x08, 0, 0x8E);
    setGate(1, &handleExceptionDebug, 0x08, 0, 0x8E);
    setGate(2, &handleExceptionNMI, 0x08, 0, 0x8E);
    setGate(4, &handleExceptionOverflow, 0x08, 0, 0x8E);
    setGate(6, &handleExceptionInvalidOpcode, 0x08, 0, 0x8E);
    setGate(13, &handleExceptionGeneralProtection, 0x08, 0, 0x8E);
    setGate(14, &handleExceptionPageFault, 0x08, 0, 0x8E);

    asm volatile("lidt %0" :: "m"(idtr));

    /*
    to dispatch: 
	asm volatile("int 40");
    */
}