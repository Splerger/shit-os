#ifndef ATA_H
#define ATA_H

#include "lib.h"

void ata_write_sector(uint32_t lba, uint8_t *data);
void ata_read_sector(uint32_t lba, uint8_t *buffer);

#endif