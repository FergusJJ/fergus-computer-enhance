#include "decode.h"
#include "file_util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// just want to decode move instructions, apart from bottom 2
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Specify one file");
    return 1;
  }

  file_body *file = (file_body *)malloc(sizeof(file_body));
  file = read_file(file, argv[1]);
  if (file->length < 1) {
    printf("Invalid file");
    free(file);
    return 1;
  }

  // this while loop should decode a single instruction
  while (file->index < file->length) {
    uint8_t byt = file->content[file->index];
    printf("> reading instruction\n");
    Opcode op = parse_opcode(byt);
    show_opcode(op);
    DecodedInstruction *decodedInstruction = parse_table[op](file->content + file->index);
    printf("Decoded Instruction: %s\n", decodedInstruction->instruction);
    file->index = file->index + decodedInstruction->bytes_used;
    free(decodedInstruction->instruction);
    free(decodedInstruction);
  }

  free(file);
}
