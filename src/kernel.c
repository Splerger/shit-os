#include "include/kernel.h"
#include "include/lib.h"
#include "include/driver.h"
#include "include/utils.h"
#include "include/keyboard.h"
#include "include/fs.h"

bool restart() {
    // Disable all interrupts
    asm volatile ("cli");

    // Wait for the keyboard controller input buffer to be clear
    while (inb(0x64) & 0x02);

    // Send the reset command
    outb(0x64, 0xFE);

    // Return true to indicate the command was issued
    return true;
}

void shutdown() {
    // Define the ACPI PM1a Control Block I/O Port (This address may vary)
    // You need to obtain the correct address from the ACPI tables (FADT table).
    uint16_t ACPI_PM1a_CNT_BLK = 0xB004;  // Example address

    // Define the SLP_TYPa and SLP_EN values
    uint16_t SLP_TYPa = 0x2000;  // Example value
    uint16_t SLP_EN = 0x2000;    // Set bit 13 to 1 (sleep enable)

    // Write the shutdown command to the PM1a control register
    outb(ACPI_PM1a_CNT_BLK, SLP_TYPa | SLP_EN);
}

void kernel_main(void) {
    terminal_initialize();

    /*
    void* smash = malloc(696969);
    if (smash == NULL) {
        blue_screen("SEGFAULT", "Failed to allocate memory");
        return;
    }
    */

    // Initialize the filesystem
    //FileSystem fs = {0};
    //create_file(&fs, "/hello.txt", "Hello, world!");
    //create_file(&fs, "/test.txt", "This is a test file.");

    // Print the contents of the files
    //const char* hello = read_file(&fs, "/hello.txt");
    //const char* test = read_file(&fs, "/test.txt");
    //terminal_writestring(hello);
    //terminal_writestring(test);

    // Delete the test file
    //delete_file(&fs, "/test.txt");

    terminal_set_cursor(0, 1);

    print_cursor_position();

    return;
}
