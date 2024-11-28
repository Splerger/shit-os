#ifndef KERNEL_FS_H
#define KERNEL_FS_H

#include "lib.h"
#include "ata.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define MAX_FILES 128
#define FS_METADATA_SECTOR 0
#define SECTOR_SIZE 512

typedef struct {
    char path[128];
    uint32_t size;
    uint32_t start_sector;
    uint32_t sector_count;
} File;

typedef struct {
    File files[MAX_FILES];
    uint32_t num_files;
} FileSystem;

void init_filesystem(FileSystem *fs);
void create_file(FileSystem *fs, const char *path, const char *contents);
const char* read_file(FileSystem *fs, const char *path, char *buffer);
void delete_file(FileSystem *fs, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_FS_H */
