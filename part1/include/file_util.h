#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_MAX_LENGTH 1000

typedef struct {
    uint8_t content[BUFFER_MAX_LENGTH];
    ssize_t length;
    int index;
} file_body;

/**
 * Reads a file from the given path into a file_body structure.
 *
 * @param path The path to the file to be read.
 * @return A pointer to a file_body structure containing the file content,
 *         or NULL if an error occurred.
 */
file_body *read_file(file_body *file,const char *path);

/**
 * Writes content to a file at the given path.
 *
 * @param file_content The content to be written to the file.
 * @param path The path where the file should be written.
 * @return 1 if the write was successful, 0 otherwise.
 */
uint8_t write_file(const char* file_content, const char* path);

#endif /* FILE_UTIL_H */
