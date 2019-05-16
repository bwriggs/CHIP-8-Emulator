#ifndef __CHIP8_H__
#define __CHIP8_H__

#define MEMORY_SIZE 4096
#define NUM_REGISTERS 16;

typedef struct Chip8
{
  unsigned short opcode;
  unsigned char memory[MEMORY_SIZE];
  unsigned char V[NUM_REGISTERS];
} Chip8;

#endif
