#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>

const char *get_register(uint8_t register_bits, uint8_t wide_bit);

typedef enum Mode {
  MEM_NO_DISP,
  MEM_DIRECT,
  MEM_8BIT_DISP,
  MEM_16BIT_DISP,
  REG_NO_DISP,
  MODE_NULL,
} Mode;

typedef struct DecodedInstruction {
  int bytes_used;
  char *instruction;
} DecodedInstruction;

typedef enum Opcode {
  MOV_RM_R,
  MOV_I_RM,
  MOV_I_R,
  MOV_M_A,
  MOV_A_M,
  MOV_RM_SR,
  MOV_SR_RM,

  NO_OP,
} Opcode;

Opcode parse_opcode(uint8_t byt);
void show_opcode(Opcode opcode);

typedef DecodedInstruction *(*InstructionParser)(uint8_t *);
DecodedInstruction *mov_rm_r_parse(uint8_t *_);
DecodedInstruction *mov_i_rm_parse(uint8_t *_);
DecodedInstruction *mov_i_r_parse(uint8_t *_);
DecodedInstruction *mov_m_a_parse(uint8_t *_);
DecodedInstruction *mov_a_m_parse(uint8_t *_);
DecodedInstruction *mov_rm_sr_parse(uint8_t *_);
DecodedInstruction *mov_sr_rm_parse(uint8_t *_);

extern InstructionParser parse_table[7];

#endif
