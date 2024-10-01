#include "scratch_buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char SCRATCH_BUFFER[SCRATCH_BUFFER_MAX_SIZE];
uint16_t __SCRATCH_BUFFER_WRITE_INDEX = 0;

void write_scratch_buffer(const char *str) {
  if (!str)
    return;

  size_t str_len = strlen(str);
  size_t space_left = SCRATCH_BUFFER_MAX_SIZE - __SCRATCH_BUFFER_WRITE_INDEX -
                      1; // -1 for null terminator
  size_t chars_to_write = (str_len < space_left) ? str_len : space_left;

  memcpy(SCRATCH_BUFFER + __SCRATCH_BUFFER_WRITE_INDEX, str, chars_to_write);
  __SCRATCH_BUFFER_WRITE_INDEX += chars_to_write;
  SCRATCH_BUFFER[__SCRATCH_BUFFER_WRITE_INDEX] = '\0';
}

void copy_buffer(char *copy_buf, int copy_buf_len) {
  size_t current_len = strlen(copy_buf);
  size_t space_left = copy_buf_len - current_len - 1; // -1 for null terminator
  size_t chars_to_copy = (__SCRATCH_BUFFER_WRITE_INDEX < space_left)
                             ? __SCRATCH_BUFFER_WRITE_INDEX
                             : space_left;
  strncpy(copy_buf + strlen(copy_buf), SCRATCH_BUFFER, chars_to_copy);
  copy_buf[strlen(copy_buf) + chars_to_copy] = '\0';
  __SCRATCH_BUFFER_WRITE_INDEX = 0;
}

void print_scratch_buffer(void) { printf("%s\n", SCRATCH_BUFFER); }

int scratch_buffer_len(void) { return __SCRATCH_BUFFER_WRITE_INDEX; }
