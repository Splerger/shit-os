#include "include/utils.h"

uint8_t inb(uint16_t port) {
    uint8_t data;
    __asm__ volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

uint16_t inw(uint16_t port) {
    uint16_t data;
    __asm__ volatile("inw %1, %0" : "=a"(data) : "Nd"(port));
}

void outw(uint16_t port, uint16_t data) {
    __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}

void int_to_str(unsigned int num, char *str) {
    char temp[10]; // Buffer to store the reversed digits
    int i = 0, j = 0;

    if (num == 0) {
        str[j++] = '0';
    } else {
        while (num > 0) {
            temp[i++] = (num % 10) + '0'; // Get the last digit
            num /= 10; // Remove the last digit
        }
        while (i > 0) {
            str[j++] = temp[--i]; // Reverse the digits
        }
    }
    str[j] = '\0'; // Null-terminate the string
}