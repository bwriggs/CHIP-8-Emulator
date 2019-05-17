#ifndef __CHIP8_H__
#define __CHIP8_H__

#include <stdbool.h>

#define MEMORY_SIZE 4096
#define NUM_REGISTERS 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define STACK_SIZE 16
#define KEYPAD_SIZE 16

typedef struct Chip8
{
  unsigned short opcode;
  unsigned char memory[MEMORY_SIZE];
  unsigned char V[NUM_REGISTERS];
  unsigned short I;
  unsigned short pc;
  unsigned char gfx[SCREEN_WIDTH * SCREEN_HEIGHT];
  unsigned char delay_timer;
  unsigned char sound_timer;
  unsigned short stack[STACK_SIZE];
  unsigned short sp;
  unsigned char key[KEYPAD_SIZE];
} Chip8;


void setUpGrapics(Chip8* chip8);
void setUpInput(Chip8* chip8);
void initialize(Chip8* chip8);
void loadCodeFromFile(Chip8* chip8, char* fileName, int fileNameLength);
void emulateCycle(Chip8* chip8);
void drawGraphics(Chip8* chip8);
void setKeys(Chip8* chip*);

#endif
