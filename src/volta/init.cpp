#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.hpp>

#define ambarOSfn extern "C"


/* Hardware text mode color constants. */
enum vga_color
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

ambarOSfn inline size_t max(size_t a, size_t b)
{
	return (a > b) ? a : b;
}

ambarOSfn inline uint8_t vgaEntryColor(vga_color fg, vga_color bg)
{
	return fg | bg << 4;
}

ambarOSfn inline uint16_t vgaEntry(unsigned char uc, uint8_t color)
{
	return (uint16_t)uc | (uint16_t)color << 8;
}

ambarOSfn inline size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer = (uint16_t *)VGA_MEMORY;

ambarOSfn void terminalInitialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vgaEntry(' ', terminal_color);
		}
	}
}

ambarOSfn void terminalSetColor(uint8_t color)
{
	terminal_color = color;
}

ambarOSfn void terminalWriteOnBuffer(const char *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		const size_t index = (terminal_row * VGA_WIDTH) + terminal_column;
		terminal_buffer[index] = vgaEntry(data[i], terminal_color);

		if (++terminal_column == VGA_WIDTH)
		{
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
		}
	}
}

ambarOSfn void terminalWrite(const char *data)
{
	terminalWriteOnBuffer(data, strlen(data));
}

ambarOSfn void terminalWriteline(const char *data)
{
	terminal_column = 0;
	terminal_row++;
	terminalWriteOnBuffer(data, strlen(data));
}

ambarOSfn void kernelMain(void)
{
	terminalInitialize();
	terminalWriteline("ambarOS alpha v0.0.1");
	terminalWriteline("volta kernel alpha v0.0.1");
	terminalWriteline("Hello, World!");
	terminalWriteline("MENA OLA!");

	/* TODOS:
	- Dynamic Memory Allocation / Paging
	- Interrupts
	- Keyboard Input
	*/

	while (1)
	  	Volta::Platform::halt();
}