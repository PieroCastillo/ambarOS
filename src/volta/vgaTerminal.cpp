#include <vgaTerminal.hpp>

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