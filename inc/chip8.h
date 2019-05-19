#ifndef __CHIP8_H__
#define __CHIP8_H__

#define MEMORY_SIZE 4096
#define NUM_REGISTERS 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define STACK_SIZE 16
#define KEYPAD_SIZE 16

#include "view.h"
#include <stdbool.h>

typedef struct Chip8
{
  unsigned short instructionRegister;
  unsigned char* memory;
  unsigned char registers[NUM_REGISTERS];
  unsigned short I;
  unsigned short pc;
  unsigned char* gfx;
  unsigned char delay_timer;
  unsigned char sound_timer;
  unsigned short stack[STACK_SIZE];
  unsigned short sp;
  unsigned char key[KEYPAD_SIZE];
  View* view;

  bool drawFlag;
} Chip8;

Chip8* newChip8();
void chip8LoadCodeFromFile(Chip8* chip8, char* fileName);
bool chip8EmulateCycle(Chip8* chip8);
void chip8Destroy(Chip8* chip8);

#endif
