#ifndef KERNEL_H
#define KERNEL_H

#include "vga.h"
#include "keyboard.h"

#ifdef __cplusplus
extern "C" { /* Use C linkage for kernel_main. */
#endif

bool restart();
void shutdown();
void kernel_main(void);

void blue_screen(const char* errorcode, const char* errormessage) {
    // Clear screen and set color to blue background with white text
    vga_index = 0;
    terminal_clear();
    terminal_setcolor(VGA_COLOR_BACKGROUND(VGA_COLOR_BLUE) | VGA_COLOR_FOREGROUND(VGA_COLOR_WHITE));

    // Print the static part of the message
    terminal_writestring("A problem has been detected and the system has been shut down to prevent");
    terminal_writestring("damage to your computer.");
    terminal_writestring("");

    // Print the error code and message with blue background
    uint32_t i = 0;
    terminal_setcolor(VGA_COLOR_BACKGROUND(VGA_COLOR_BLUE) | VGA_COLOR_FOREGROUND(VGA_COLOR_WHITE));

    terminal_writestring_without_newline(errorcode);
    i += strlen(errorcode);
    terminal_writestring_without_newline(": ");
    i += 2;
    terminal_writestring_without_newline(errormessage);
    i += strlen(errormessage);

    // Fill the rest of the line with spaces to align properly
    for (; i < VGA_WIDTH; i++) {
        terminal_putchar(' ');
    }
    
    terminal_writestring("");
    terminal_writestring("If this is the first time you've seen this Stop error screen, restart");
    terminal_writestring("your computer. If this screen appears again, please email the");
    terminal_writestring("developers of shit os <jane@mingnint.xyz>.");
    terminal_writestring("");
    terminal_writestring("Press any key to restart.");
    terminal_writestring("Press ESC to shutdown.");

    // Fill remaining space on screen with blank lines to keep blue background
    for (unsigned int i = 0; i < VGA_WIDTH * VGA_HEIGHT - vga_index; i++) {
        terminal_putchar(' ');
    }

    // Wait for user input to either restart or shutdown
    while (1) {
        while (!key_pressed()) {}
        uint8_t key = read_key();
        if (key == 0x01) {
            shutdown();
        } else {
            restart();
        }
    }
}



#ifdef __cplusplus
}
#endif

#endif /* KERNEL_H */