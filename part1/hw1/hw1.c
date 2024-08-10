#include <stdint.h>
#include <stdio.h>
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
  while (buffer_length < BUFFER_MAX_LENGTH && (c = getc(fp)) != EOF) {
    buf[buffer_length] = (uint8_t)c;
    buffer_length++;
  }
  fclose(fp);

  // char *asm_output;

  while (parse_index < buffer_length) {

    if (parse_index + 1 >= buffer_length) {
      printf("Bad number of bytes");
      return 1;
    }
    char current_instruction[20] = "";
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
    strncpy(current_instruction, get_op_code(op_code), 3);
    strncat(current_instruction, " ", 1);
    if (d_bit == 0) {
      // REG is source, R/M is destination
      strncat(current_instruction, get_register(rm, w_bit), 2);
      strncat(current_instruction, ", ", 2);
      strncat(current_instruction, get_register(reg, w_bit), 2);
    } else {
      // REG is destination, R/M is source
      strncat(current_instruction, get_register(reg, w_bit), 2);
      strncat(current_instruction, ", ", 2);
      strncat(current_instruction, get_register(rm, w_bit), 2);
    }
    printf("%s\n", current_instruction);
  }
  return 0;
}
