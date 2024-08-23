#include "scratch_buffer.h"
#include <stdint.h>
#include <string.h>

char SCRATCH_BUFFER[SCRATCH_BUFFER_MAX_SIZE];
uint16_t __SCRATCH_BUFFER_WRITE_INDEX = 0;

void write_scratch_buffer(const char *str) {
  while (str && __SCRATCH_BUFFER_WRITE_INDEX < (SCRATCH_BUFFER_MAX_SIZE - 1)) {
    SCRATCH_BUFFER[__SCRATCH_BUFFER_WRITE_INDEX++] = *str++;
  }
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

int scratch_buffer_len(void) { return __SCRATCH_BUFFER_WRITE_INDEX; }
