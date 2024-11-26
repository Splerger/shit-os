#include "include/fs.h"

// Utility function to calculate the length of a string
static size_t my_strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Utility function to copy a string
static void my_strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

// Utility function to compare strings
static int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Utility function to allocate memory for a string and copy its contents
static char *my_strdup(const char *str) {
    size_t len = my_strlen(str) + 1; // Include null terminator
    char *dup = (char *)my_malloc(len);
    if (!dup) {
        return NULL; // Memory allocation failed
    }
    my_strcpy(dup, str);
    return dup;
}

// A simple memory allocation function
// You can replace this with your OS's memory management system
static void *my_malloc(size_t size) {
    static char memory_pool[1024 * 1024]; // 1 MB of static memory pool
    static size_t offset = 0;

    if (offset + size > sizeof(memory_pool)) {
        return NULL; // Out of memory
    }

    void *ptr = &memory_pool[offset];
    offset += size;
    return ptr;
}

// A simple memory deallocation function (no-op for static allocation)
static void my_free(void *ptr) {
    // Do nothing for now; static memory can't be freed
}

void init_filesystem(FileSystem *fs) {
    fs->num_files = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        fs->files[i].data = NULL;
        fs->files[i].path = NULL;
        fs->files[i].size = 0;
    }
}

int create_file(FileSystem *fs, const char *path, const char *contents) {
    if (fs->num_files >= MAX_FILES) {
        return -1; // Filesystem full
    }
    for (int i = 0; i < fs->num_files; i++) {
        if (my_strcmp(fs->files[i].path, path) == 0) {
            return -2; // File already exists
        }
    }
    File *file = &fs->files[fs->num_files++];
    file->data = my_strdup(contents);
    file->size = my_strlen(contents);
    file->path = my_strdup(path);
    return 0; // Success
}

const char *read_file(FileSystem *fs, const char *path) {
    for (int i = 0; i < fs->num_files; i++) {
        if (my_strcmp(fs->files[i].path, path) == 0) {
            return fs->files[i].data;
        }
    }
    return NULL; // File not found
}

int delete_file(FileSystem *fs, const char *path) {
    for (int i = 0; i < fs->num_files; i++) {
        if (my_strcmp(fs->files[i].path, path) == 0) {
            my_free(fs->files[i].data);
            my_free(fs->files[i].path);

            // Shift remaining files to close the gap
            for (int j = i; j < fs->num_files - 1; j++) {
                fs->files[j] = fs->files[j + 1];
            }
            fs->num_files--;
            return 0; // Success
        }
    }
    return -1; // File not found
}
