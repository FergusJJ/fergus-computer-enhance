#include "decode.h"
#include "scratch_buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Mode get_mode(const uint8_t mod, const uint8_t reg) {
  switch (mod) {
  case 0x0:
    if (reg == 0x6) {
      return MEM_DIRECT;
      // Example: MOV AX, [1234h]
    }
    return MEM_NO_DISP;
    // MOV AX, [BX+SI]   (when R/M = 000)

  case 0x1:
    return MEM_8BIT_DISP;
    // MOV AX, [BX+12h]      (8-bit displacement)

  case 0x2:
    return MEM_16BIT_DISP;
    // MOV AX, [BP+SI+5678h] (16-bit displacement)

  case 0x3:
    return REG_NO_DISP;
    // MOV AH, CH    (when wide = 0)

  default:
    return NULL;
    // This case should never occur in valid 8086 instructions
  }
}

const char *get_register(uint8_t register_bits, uint8_t wide_bit) {
  switch (register_bits) {
  case 0x0:
    if (wide_bit == 1) {
      return "AX";
    }
    return "AL";
  case 0x1:
    if (wide_bit == 1) {
      return "CX";
    }
    return "CL";
  case 0x2:
    if (wide_bit == 1) {
      return "DX";
    }
    return "DL";
  case 0x3:
    if (wide_bit == 1) {
      return "BX";
    }
    return "BL";
  case 0x4:
    if (wide_bit == 1) {
      return "SP";
    }
    return "AH";
  case 0x5:
    if (wide_bit == 1) {
      return "BP";
    }
    return "CH";
  case 0x6:
    if (wide_bit == 1) {
      return "SI";
    }
    return "DH";
  case 0x7:
    if (wide_bit == 1) {
      return "DI";
    }
    return "BH";
  default:
    printf("Unknown register_bits: %d", register_bits);
    break;
  }
  return NULL;
}

Opcode parse_opcode(uint8_t byt) {
  switch (byt & 0xfc) {
  case 0x88:
    return MOV_RM_R;
  case 0xc4:
    return MOV_I_RM;
  case 0xb0:
  case 0xb4:
  case 0xb8:
  case 0xbc:
    return MOV_I_R;
  case 0xa0:
    if (((byt >> 1) & 0x1) == 0x1) {
      return MOV_A_M;
    }
    return MOV_M_A;
  case 0x8c:
    if (((byt >> 1) & 0x1) == 0x1) {
      return MOV_RM_SR;
    }
    return MOV_SR_RM;
  }
  return NO_OP;
}

char *effective_addr_reg(uint8_t rm) {
  switch (rm) {
  case 0x0:
    return "BX+SI";
  case 0x1:
    return "BX+DI";
  case 0x2:
    return "BP+SI";
  case 0x3:
    return "BP+DI";
  case 0x4:
    return "SI";
  case 0x5:
    return "DI";
  case 0x6:
    return "BP";
  case 0x7:
    return "BX";
  default:
    return NULL;
  }
  return NULL;
}

// this can probably be a function that takes mod bits
// and then either calcs effective address or decides the register?
// next bytes should be from teh 2nd byte onwards
// This is just for rm
int calc_rm(const Mode mode, const uint8_t rm_bits, const uint8_t wide_bit,
            const uint8_t *bytes, char *buf, const int buf_len) {
  switch (mode) {
  case MEM_DIRECT: {
    uint8_t disp_lo = bytes[2];
    uint8_t disp_hi = bytes[3];
    uint16_t address = (disp_hi << 8) | disp_lo;
    snprintf(buf, buf_len, "[%Xh]", address);
    return 4;
  }
  case MEM_NO_DISP:
    snprintf(buf, buf_len, "[%s]", effective_addr_reg(rm_bits));
    return 2;
  case MEM_8BIT_DISP:
    snprintf(buf, buf_len, "[%s + %Xh]", effective_addr_reg(rm_bits), bytes[2]);
    return 3;
  case MEM_16BIT_DISP: {
    uint16_t disp = (bytes[3] << 8) | bytes[2];
    snprintf(buf, buf_len, "[%s+ %Xh]", effective_addr_reg(rm_bits), disp);
    return 4;
  }
  case REG_NO_DISP:
    snprintf(buf, buf_len, "[%s]", get_register(rm_bits, wide_bit));
    return 2;
  default:
    return -1;
  }
}

typedef DecodedInstruction *(*InstructionParser)(uint8_t *);

DecodedInstruction *mov_rm_r_parse(uint8_t *bytes) {
  DecodedInstruction *decodedInstruction =
      (DecodedInstruction *)malloc(sizeof(DecodedInstruction));
  write_scratch_buffer("MOV ");

  uint8_t byte_one = bytes[0];
  uint8_t direction_bit = (byte_one >> 1) & 0x1;
  uint8_t wide_bit = byte_one & 0x1;
  uint8_t mode_bits = bytes[1] & 0xc0;
  uint8_t reg_bits = bytes[1] & 0x38;
  uint8_t reg_mem_bits = bytes[1] & 0x7;

  const char *reg = get_register(reg_bits, wide_bit);
  if (reg == NULL) {
    free(decodedInstruction);
    return NULL;
  }
  Mode mode = get_mode(mode_bits, reg_bits);
  char rm_buf[50];
  int bytes_used = calc_rm(mode, reg_mem_bits, wide_bit, bytes, rm_buf, 50);
  if (bytes_used == -1) {
    free(decodedInstruction);
    return NULL;
  }
    // + 2 for ", ", any + or "[" "]" should be in the r/m or reg instruction
    // strings
    switch (direction_bit) {
    case 0x0:
      write_scratch_buffer(rm_buf);
      write_scratch_buffer(", ");
      write_scratch_buffer(reg);
      break;
    case 0x1:
      write_scratch_buffer(reg);
      write_scratch_buffer(", ");
      write_scratch_buffer(rm_buf);
      break;
    default:
      return NULL;
    }

  // might have to fill in the string for instruction

  decodedInstruction->instruction = (char*)malloc(sizeof(char) * scratch_buffer_len());
  //memset();
  decodedInstruction->bytes_used = bytes_used;
  copy_buffer(decodedInstruction->instruction, 0); // CHANGE ME
  return decodedInstruction;
}

DecodedInstruction *mov_i_rm_parse(uint8_t *_) { return NULL; }
DecodedInstruction *mov_i_r_parse(uint8_t *_) { return NULL; }
DecodedInstruction *mov_m_a_parse(uint8_t *_) { return NULL; }
DecodedInstruction *mov_a_m_parse(uint8_t *_) { return NULL; }
DecodedInstruction *mov_rm_sr_parse(uint8_t *_) { return NULL; }
DecodedInstruction *mov_sr_rm_parse(uint8_t *_) { return NULL; }

InstructionParser parse_table[7] = {
    mov_rm_r_parse, mov_i_rm_parse,  mov_i_r_parse,   mov_m_a_parse,
    mov_a_m_parse,  mov_rm_sr_parse, mov_sr_rm_parse,
};
