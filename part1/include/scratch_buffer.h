#ifndef SCRATCH_BUFFER_H
#define SCRATCH_BUFFER_H

#define SCRATCH_BUFFER_MAX_SIZE 500

extern char SCRATCH_BUFFER[SCRATCH_BUFFER_MAX_SIZE];

void write_scratch_buffer(const char *str);

void copy_buffer(char *copy_buf, int copy_buf_len);

int scratch_buffer_len(void);

#endif
