#include "include/ata.h"

// ATA Ports
#define ATA_PRIMARY_IO 0x1F0
#define ATA_PRIMARY_CONTROL 0x3F6

// ATA Commands
#define ATA_CMD_READ_SECTORS 0x20
#define ATA_CMD_WRITE_SECTORS 0x30

// ATA Registers
#define ATA_REG_DATA 0
#define ATA_REG_ERROR 1
#define ATA_REG_SECT_COUNT 2
#define ATA_REG_LBA_LOW 3
#define ATA_REG_LBA_MID 4
#define ATA_REG_LBA_HIGH 5
#define ATA_REG_DEVICE 6
#define ATA_REG_STATUS 7
#define ATA_REG_COMMAND 7

// ATA Status Flags
#define ATA_STATUS_BSY 0x80
#define ATA_STATUS_DRQ 0x08

#define SECTOR_SIZE 512

// Wait for the drive to be ready (BSY clear)
void ata_wait_bsy() {
    while(inb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_STATUS_BSY);
}

void ata_wait_drq() {
    while(!(inb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_STATUS_DRQ));
}

// Write a single sector to the disk
void ata_write_sector(uint32_t lba, uint8_t *data) {
    // Wait until the drive is not busy
    ata_wait_bsy();

    // Select drive and LBA mode (Master, CHS/LBA mode bits)
    outb(ATA_PRIMARY_IO + ATA_REG_DEVICE, 0xE0 | ((lba >> 24) & 0x0F));

    // Set sector count to 1
    outb(ATA_PRIMARY_IO + ATA_REG_SECT_COUNT, 1);

    // Set LBA address
    outb(ATA_PRIMARY_IO + ATA_REG_LBA_LOW, lba & 0xFF);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA_HIGH, (lba >> 16) & 0xFF);

    // Send the write command
    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_WRITE_SECTORS);

    // Wait for the drive to request data
    ata_wait_bsy();
    ata_wait_drq();

    // Write 512 bytes (one sector)
    for (int i = 0; i < SECTOR_SIZE / 2; i++) {
        uint16_t word = ((uint16_t *)data)[i];
        outw(ATA_PRIMARY_IO + ATA_REG_DATA, word);
    }

    // Wait for the drive to finish writing
    ata_wait_bsy();
}

// Read a single sector from the disk
void ata_read_sector(uint32_t lba, uint8_t *buffer) {
    // Wait until the drive is not busy
    ata_wait_bsy();

    // Select drive and LBA mode (Master, CHS/LBA mode bits)
    outb(ATA_PRIMARY_IO + ATA_REG_DEVICE, 0xE0 | ((lba >> 24) & 0x0F));

    // Set sector count to 1
    outb(ATA_PRIMARY_IO + ATA_REG_SECT_COUNT, 1);

    // Set LBA address
    outb(ATA_PRIMARY_IO + ATA_REG_LBA_LOW, lba & 0xFF);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA_HIGH, (lba >> 16) & 0xFF);

    // Send the read command
    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_SECTORS);

    // Wait for the drive to request data
    ata_wait_bsy();
    ata_wait_drq();

    // Read 512 bytes (one sector)
    for (int i = 0; i < SECTOR_SIZE / 2; i++) {
        ((uint16_t *)buffer)[i] = inw(ATA_PRIMARY_IO + ATA_REG_DATA);
    }

    // Wait for the drive to finish reading
    ata_wait_bsy();
}