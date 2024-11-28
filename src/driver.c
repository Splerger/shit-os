#include "include/driver.h"
#include "include/utils.h"
#include "include/vga.h"

void init_graphics() {
    // Set the framebuffer address
    outb(0x03D4, 0x0C);
    outb(0x03D5, FRAMEBUFFER_ADDRESS & 0xFF);
    outb(0x03D4, 0x0D);
    outb(0x03D5, (FRAMEBUFFER_ADDRESS >> 8) & 0xFF);

    // Set the graphics mode
    outb(0x03D4, 0x00);
    outb(0x03D5, 0x3);
    outb(0x03D4, 0x01);
    outb(0x03D5, 0x0);
    outb(0x03D4, 0x02);
    outb(0x03D5, 0xF);
    outb(0x03D4, 0x03);
    outb(0x03D5, 0x0);
    outb(0x03D4, 0x04);
    outb(0x03D5, 0x0);

    // Set the cursor position
    outb(0x03D4, 0x0E);
    outb(0x03D5, 0x0);
    outb(0x03D4, 0x0F);
    outb(0x03D5, 0x0);
}

void render_framebuffer() {
    outb(0x03D4, 0x0C);
    outb(0x03D5, FRAMEBUFFER_ADDRESS & 0xFF);
    outb(0x03D4, 0x0D);
    outb(0x03D5, (FRAMEBUFFER_ADDRESS >> 8) & 0xFF);
    outb(0x03D4, 0x01);
    outb(0x03D5, 0x0);
    outb(0x03D4, 0x02);
    outb(0x03D5, 0xF);

    outb(0x03D4, 0x09);
    outb(0x03D5, 0x0);
}

void clear_screen() {

}

void draw_pixel(int x, int y, unsigned char color) {
    int offset = (y * VGA_WIDTH + x) * sizeof(screen_char);
    screen_char *pixel = (screen_char *) (FRAMEBUFFER_ADDRESS + offset);
    pixel->character = ' ';
    pixel->attributes = color;
}

// Updated functions with unused parameter suppression and default return values
void putc(char c) {
    (void)c; // Suppress unused parameter warning
}

void puts(char *s) {
    (void)s; // Suppress unused parameter warning
}

void draw_rect(int x, int y, int width, int height, unsigned char color) {
    (void)x; (void)y; (void)width; (void)height; (void)color; // Suppress warnings
}

unsigned char get_pixel(int x, int y) {
    (void)x; (void)y; // Suppress unused parameter warning
    return 0;         // Return a default value
}