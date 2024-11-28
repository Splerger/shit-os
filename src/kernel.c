/*
 * kernel.c - Kernel entry point
 */

#include "include/kernel.h"
#include "include/lib.h"
#include "include/driver.h"
#include "include/utils.h"
#include "include/keyboard.h"
#include "include/fs.h"

// Declare a global FileSystem object
FileSystem fs;

bool restart() {
    asm volatile("cli"); // Disable all interrupts
    while (inb(0x64) & 0x02); // Wait for keyboard controller input buffer to clear
    outb(0x64, 0xFE); // Send the reset command
    return true; // Command issued
}

void shutdown() {
    uint16_t ACPI_PM1a_CNT_BLK = 0xB004; // Example address
    uint16_t SLP_TYPa = 0x2000;         // Example value
    uint16_t SLP_EN = 0x2000;           // Set bit 13 to 1 (sleep enable)
    outb(ACPI_PM1a_CNT_BLK, SLP_TYPa | SLP_EN); // Write shutdown command
}

void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();

    // Initialize the filesystem
    init_filesystem(&fs);

    // Test creating a file
    create_file(&fs, "/hello.txt", "Hello, World!");
    create_file(&fs, "/foo.txt", "This is a test file.");

    // Buffer for reading files
    char buffer[512];

    // Test reading files
    const char *hello_data = read_file(&fs, "/hello.txt", buffer);
    terminal_writestring("Read /hello.txt: ");
    terminal_writestring(hello_data);
    terminal_putchar('\n');
    

    const char *foo_data = read_file(&fs, "/foo.txt", buffer);
    terminal_writestring("Read /foo.txt: ");
    terminal_writestring(foo_data);
    terminal_putchar('\n');

    // Test deleting a file
    delete_file(&fs, "/hello.txt");
    const char *deleted_data = read_file(&fs, "/hello.txt", buffer);
    if (!deleted_data) {
        terminal_writestring("/hello.txt successfully deleted.\n");
    } else {
        terminal_writestring("Failed to delete /hello.txt\n");
    }

    return;
}
