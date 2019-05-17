#include "inc.h"
#include <stdlib.h>

void setUpGrapics(Chip8* chip8)
{
  
}

void setUpInput(Chip8* chip8)
{

}

void initialize(Chip8* chip8)
{
  for(int i = 0; i < MEMORY_SIZE; ++i)
  {
    chip8->opcode = 0;
    chip8->memory = malloc(MEMORY_SIZE * sizeof(unsigned char));
    chip8->V = malloc(NUM_REGISTERS * sizeof(unsigned char));
    chip8->I = 0;
    chip8->pc = 200;
    chip8->gfx = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(unsigned char));
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->stack = malloc(STACK_SIZE * sizeof(unsigned short));
    chip8->sp = 0;
    chip8->key = malloc(KEYPAD_SIZE * sizeof(unsigned char));
  }
}

void close(Chip8* chip8)
{
  if(chip8->memory != NULL)
  {
    free(chip8->memory);
    chip8->memory = NULL;
  }

  if(chip8->V != NULL)
  {
    free(chip8->V);
    chip8->V = NULL;
  }

  if(chip8->gfx != NULL)
  {
    free(chip8->gfx);
    chip8->gfx  = NULL;
  }

  if(chip8->stack != NULL)
  {
    free(chip8->stack);
    chip8->stack  = NULL;
  }

  if(chip8->key != NULL)
  {
    free(chip8->key);
    chip8->key  = NULL;
  }
}

void loadCodeFromFile(Chip8* chip8, char* fileName, int fileNameLength)
{

}

void emulateCycle(Chip8* chip8)
{
  chip8->opcode = chip8->memory[chip8->pc] << 8 
    || memory[chip8->pc + 1];

  switch(opcode & 
}

void drawGraphics(Chip8* chip8)
{

}

void setKeys(Chip8* chip8)
{

}
