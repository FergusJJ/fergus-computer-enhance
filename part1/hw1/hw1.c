#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_MAX_LENGTH 1000

enum { MOV = 0b100010 };

// W == 0
enum {
  AL = 0b000,
  CL = 0b001,
  DL = 0b010,
  BL = 0b011,
  AH = 0b100,
  CH = 0b101,
  DH = 0b110,
  BH = 0b111,

};

// W == 1
enum {
  AX = 0b000,
  CX = 0b001,
  DX = 0b010,
  BX = 0b011,
  SP = 0b100,
  BP = 0b101,
  SI = 0b110,
  DI = 0b111,
};

const char *get_op_code(uint8_t op_bits) {
  switch (op_bits) {
  case MOV:
    return "MOV";
  default:
    printf("Unknown opcode, got %d\n", op_bits);
    return "";
  }
}

const char *get_register(uint8_t register_bits, uint8_t w_bit) {
  switch (w_bit) {
  case 0b0:
    switch (register_bits) {
    case AL:
      return "AL";
    case CL:
      return "CL";
    case DL:
      return "DL";
    case BL:
      return "BL";
    case AH:
      return "AH";
    case CH:
      return "CH";
    case DH:
      return "DH";
    case BH:
      return "BH";
    default:
      return "";
    }
  case 0b1:
    switch (register_bits) {
    case AX:
      return "AX";
    case CX:
      return "CX";
    case DX:
      return "DX";
    case BX:
      return "BX";
    case SP:
      return "SP";
    case BP:
      return "BP";
    case SI:
      return "SI";
    case DI:
      return "DI";
    default:
      return "";
    }
  default:
    printf("Unknown w bit: %d\n", w_bit);
    return "";
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    return 1;
  }

  ssize_t parse_index = 0;
  ssize_t buffer_length = 0;
  uint8_t buf[BUFFER_MAX_LENGTH + 1];
  FILE *fp;
  fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    printf("Could not open fp\n");
    return 1;
  }

  int c;
  while ((c = getc(fp)) != EOF) {
    if (buffer_length >= BUFFER_MAX_LENGTH) {
      printf("Input file too large, max length is %d\n", BUFFER_MAX_LENGTH);
      fclose(fp);
      return 1;
    }
    buf[buffer_length] = (uint8_t)c;
    buffer_length++;
  }
  fclose(fp);

  char *asm_output = NULL;

  while (parse_index < buffer_length) {
    if (parse_index + 1 >= buffer_length) {
      printf("Bad number of bytes");
      return 1;
    }
    char *current_instruction = NULL;
    size_t current_instruction_size = 0;
    // byte 1
    // bits [1..6] = OPCODE
    // bits [7] = d bit
    // bits [8] = w bit
    uint8_t byte_1 = buf[parse_index];
    uint8_t op_code = (byte_1 >> 2) & 0x3F;
    uint8_t d_bit = (byte_1 >> 1) & 0x01;
    uint8_t w_bit = byte_1 & 0x01;
    // byte 2
    // bits [1..2] register mode/memory mode w displacement length
    // bits [3..5] register operand
    // bits [6..8] register operand
    uint8_t byte_2 = buf[parse_index + 1];
    // uint8_t mod = (byte_2 >> 6) & 0x03;
    uint8_t reg = (byte_2 >> 3) & 0x07;
    uint8_t rm = byte_2 & 0x07;
    parse_index += 2; // for 16 bit instructions

    const char *op_code_str = get_op_code(op_code);
    size_t op_code_len = strlen(op_code_str);
    current_instruction = malloc(op_code_len + 2);
    if (current_instruction == NULL) {
      printf("Memory allocation failed");
      return 1;
    }
    strcpy(current_instruction, op_code_str);
    strcat(current_instruction, " ");
    current_instruction_size = op_code_len + 2;

    const char *dest_reg;
    const char *source_reg;
    if (d_bit == 0) {
      dest_reg = get_register(rm, w_bit);
      source_reg = get_register(reg, w_bit);
    } else {
      dest_reg = get_register(reg, w_bit);
      source_reg = get_register(rm, w_bit);
    }

    size_t new_size =
        current_instruction_size + strlen(dest_reg) + strlen(source_reg) + 4;
    char *temp = realloc(current_instruction, new_size);
    if (temp == NULL) {
      printf("Memory allocation failed");
      free(current_instruction);
      return 1;
    }
    current_instruction = temp;
    current_instruction_size = new_size;

    strcat(current_instruction, dest_reg);
    strcat(current_instruction, ", ");
    strcat(current_instruction, source_reg);
    strcat(current_instruction, "\n");

    if (asm_output == NULL) {
      asm_output = strdup(current_instruction);
    } else {
      char *temp = realloc(asm_output, strlen(asm_output) +
                                           strlen(current_instruction) + 1);
      if (temp == NULL) {
        printf("Memory allocation failed");
        free(current_instruction);
        free(asm_output);
        return 1;
      }
      asm_output = temp;
      strcat(asm_output, current_instruction);
    }

    free(current_instruction);
  }
  FILE *wp;
  wp = fopen("hw1.asm", "w");
  if (wp == NULL) {
    printf("Could not open wp\n");
    return 1;
  }
  fprintf(wp, "%s\n", asm_output);
  fclose(wp);
  free(asm_output);
  return 0;
}
