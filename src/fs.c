/*
 * fs.c - Filesystem implementation with ATA disk support
 */

#include "include/fs.h"

// Local function prototypes
static void write_metadata(FileSystem *fs);
static void read_metadata(FileSystem *fs);
static uint32_t allocate_sectors(FileSystem *fs, uint32_t size);

// Initialize filesystem and load metadata from disk
void init_filesystem(FileSystem *fs) {
    read_metadata(fs);
    if (fs->num_files > MAX_FILES) {
        fs->num_files = 0; // Corrupt metadata; reset
    }
}

// Create a new file and persist it to disk
void create_file(FileSystem *fs, const char *path, const char *contents) {
    if (fs->num_files >= MAX_FILES) {
        return; // Max file limit reached
    }

    uint32_t size = strlen(contents);
    uint32_t sector_count = (size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    uint32_t start_sector = allocate_sectors(fs, sector_count);

    if (start_sector == 0) {
        return; // No space available
    }

    // Write file content to disk
    for (uint32_t i = 0; i < sector_count; i++) {
        ata_write_sector(start_sector + i, (uint8_t *)(contents + (i * SECTOR_SIZE)));
    }

    // Update metadata
    File *file = &fs->files[fs->num_files++];
    strcpy(file->path, path);
    file->size = size;
    file->start_sector = start_sector;
    file->sector_count = sector_count;

    write_metadata(fs);
}

// Read file contents from disk
const char* read_file(FileSystem *fs, const char *path, char *buffer) {
    for (uint32_t i = 0; i < fs->num_files; i++) {
        if (strcmp(fs->files[i].path, path) == 0) {
            File *file = &fs->files[i];
            for (uint32_t j = 0; j < file->sector_count; j++) {
                ata_read_sector(file->start_sector + j, (uint8_t *)(buffer + (j * SECTOR_SIZE)));
            }
            return buffer; // Return the buffer
        }
    }
    
    blue_screen("FILE_NOT_FOUND", "Failed to read file");
}

// Delete a file and free its sectors
void delete_file(FileSystem *fs, const char *path) {
    for (uint32_t i = 0; i < fs->num_files; i++) {
        if (strcmp(fs->files[i].path, path) == 0) {
            // Shift metadata
            for (uint32_t j = i; j < fs->num_files - 1; j++) {
                fs->files[j] = fs->files[j + 1];
            }
            fs->num_files--;
            write_metadata(fs);
            return;
        }
    }
}

// Write filesystem metadata to disk
static void write_metadata(FileSystem *fs) {
    ata_write_sector(FS_METADATA_SECTOR, (uint8_t *)fs);
}

// Read filesystem metadata from disk
static void read_metadata(FileSystem *fs) {
    ata_read_sector(FS_METADATA_SECTOR, (uint8_t *)fs);
}

// Allocate sectors for a file
static uint32_t allocate_sectors(FileSystem *fs, uint32_t sector_count) {
    uint32_t start_sector = 1; // Start after metadata sector
    // Simple allocation logic (for demo purposes; refine for a real allocator)
    for (uint32_t i = start_sector; i < 1000; i++) { // Arbitrary disk limit
        if (i, i+sector_count-1) {
            return i;
        }
    }
    return 0; // No free space
}
