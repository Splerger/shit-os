#include "include/vga.h"
#include "include/lib.h"
#include "include/utils.h"

void terminal_initialize(void) {
    terminal_buffer = (unsigned short *) VGA_ADDRESS;
    vga_index = 0;
    terminal_color = VGA_COLOR_WHITE;
    terminal_clear();
}

void terminal_setcolor(unsigned char color) {
    terminal_color = color;
}

void terminal_putentryat(char c, unsigned char color, unsigned int x, unsigned int y) {
    // VGA buffer starts at some base address. For simplicity, we assume a direct address.
    unsigned short* vga_buffer = (unsigned short*)0xB8000;
    
    // VGA text mode stores the character as a single byte, and the color is stored as a separate byte.
    unsigned short entry = ((unsigned short)color << 8) | (unsigned char)c;
    
    // The offset in the VGA buffer is calculated by the row and column (x, y).
    unsigned int index = y * VGA_WIDTH + x;
    
    vga_buffer[index] = entry;
}


void terminal_putchar(char c) {
    static unsigned int vga_i = 0;
    
    if (c == '\r') {
        vga_i = vga_i - (vga_i % VGA_WIDTH);
    } else if (c == '\b') {
        if (vga_i > 0) {
            vga_i--;
            terminal_putentryat(' ', terminal_color, vga_i % VGA_WIDTH, vga_i / VGA_WIDTH);
        }
    } else if (c == '\t') {
        vga_i += 4;
    } else if (c == '\n') {
        vga_i += VGA_WIDTH - (vga_i % VGA_WIDTH);
    } else {
        terminal_putentryat(c, terminal_color, vga_i % VGA_WIDTH, vga_i / VGA_WIDTH);
        vga_i++;
    }

    vga_index = vga_i;
}

void terminal_write(const char *data, unsigned int size, uint32_t newline) {
    for (unsigned int i = 0; i < size; i++)
        terminal_putchar(data[i]);
    if (newline == 0)
        return;
    if (size == 0) {
        for (unsigned int i = 0; i < VGA_WIDTH; i++)
            terminal_putchar(' ');
        return;
    }
    for (unsigned int i = 0; i < VGA_WIDTH - size; i++)
        terminal_putchar(' ');
}

void terminal_writestring(const char *data) {
    terminal_write(data, strlen(data), 1);
}

void terminal_writestring_without_newline(const char *data) {
    terminal_write(data, strlen(data), 0);
}

void terminal_clear(void) {
    unsigned int i = 0;
    while (i < VGA_WIDTH * VGA_HEIGHT) {
        terminal_buffer[i++] = 0;
    }
}

void terminal_set_cursor(unsigned int x, unsigned int y) {
    
    /*
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
        return;
    }
    */

    const unsigned int index = y * VGA_WIDTH + x;
    vga_index = index;
}

void terminal_get_cursor(unsigned int *x, unsigned int *y) {
    *x = vga_index % VGA_WIDTH;
    *y = vga_index / VGA_WIDTH;
}

void print_cursor_position() {
    unsigned int x, y;
    char x_str[10];
    char y_str[10];
    char buffer[50]; // Buffer to hold the formatted string

    // Get the cursor position
    terminal_get_cursor(&x, &y);

    // Convert integers to strings
    int_to_str(x, x_str);
    int_to_str(y, y_str);

    // Construct the output string manually
    char *msg = "Cursor Position: X=";
    char *msg_end = ", Y=";
    char *msg_end_end = "\n";
    int idx = 0;

    // Copy "Cursor Position: X="
    while (*msg) {
        buffer[idx++] = *msg++;
    }

    // Copy X value
    char *ptr = x_str;
    while (*ptr) {
        buffer[idx++] = *ptr++;
    }

    // Copy ", Y="
    ptr = msg_end;
    while (*ptr) {
        buffer[idx++] = *ptr++;
    }

    // Copy Y value
    ptr = y_str;
    while (*ptr) {
        buffer[idx++] = *ptr++;
    }

    // Copy "\n"
    ptr = msg_end_end;
    while (*ptr) {
        buffer[idx++] = *ptr++;
    }

    // Null-terminate the string
    buffer[idx] = '\0';

    // Print the buffer to the terminal
    terminal_writestring(buffer);
}

void vga_set_pixel(unsigned int x, unsigned int y, unsigned char color) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
        return;
    }

    const unsigned int index = y * VGA_WIDTH + x;
    terminal_buffer[index] = (unsigned short) terminal_buffer[index] | (unsigned short) color << 8;
}