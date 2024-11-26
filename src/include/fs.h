#ifndef KERNEL_FS_H
#define KERNEL_FS_H

#include "lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FILES 128

typedef struct {
    char *data;
    char *path;
    size_t size;
} File;

typedef struct {
    File files[MAX_FILES];
    size_t num_files;
} FileSystem;

// Filesystem functions
void init_filesystem(FileSystem *fs);
int create_file(FileSystem *fs, const char *path, const char *contents);
const char *read_file(FileSystem *fs, const char *path);
int delete_file(FileSystem *fs, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_FS_H */
