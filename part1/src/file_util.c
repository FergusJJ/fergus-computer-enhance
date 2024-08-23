#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_MAX_LENGTH 1000

typedef struct {
  uint8_t content[BUFFER_MAX_LENGTH];
  ssize_t length;
  int index;
} file_body;

file_body *read_file(file_body* file,const char *path) {
  if (file == NULL) {
    printf("Memory allocation failed\n");
    return NULL;
  }

  FILE *fp = fopen(path, "rb");
  if (fp == NULL) {
    printf("Could not open %s\n", path);
    free(file);
    return NULL;
  }

  size_t buffer_length = 0;
  int c;
  while ((c = getc(fp)) != EOF) {
    if (buffer_length >= BUFFER_MAX_LENGTH) {
      printf("Input file too large, max length is %d\n", BUFFER_MAX_LENGTH);
      fclose(fp);
      free(file);
      return NULL;
    }
    file->content[buffer_length] = (uint8_t)c;
    buffer_length++;
  }

  fclose(fp);
  file->length = buffer_length;
  return file;
}

uint8_t write_file(const char* file_content, const char* path) {
  FILE *wp;
  wp = fopen(path, "w");
  if (wp == NULL) {
    printf("Could not open wp\n");
    return 0;
  }
  fprintf(wp, "%s\n", file_content);
  fclose(wp);
  return 1;
}
