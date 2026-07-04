#ifndef VGA_TERMINAL_HPP

#include <cstddef>
#include <cstdint>

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

constexpr uint64_t VGA_WIDTH = 80;
constexpr uint64_t VGA_HEIGHT = 25;
constexpr uint64_t VGA_MEMORY = 0xB8000;


ambarOSfn void terminalInitialize();
ambarOSfn void terminalSetColor(uint8_t color);
ambarOSfn void terminalWriteOnBuffer(const char *data, size_t size);
ambarOSfn void terminalWrite(const char *data);
ambarOSfn void terminalWriteline(const char *data);
#endif