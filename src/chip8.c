#include "chip8.h"
#include "view.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void chip8Destroy(Chip8* chip8)
{
  if(chip8->memory)
  {
    free(chip8->memory);
    chip8->memory = NULL;
  }

  if(chip8->gfx)
  {
    free(chip8->gfx);
    chip8->gfx = NULL;
  }

  if(chip8->view)
  {
    viewDestroy(chip8->view);
    chip8->view = NULL;
  }
}

bool chip8Initialize(Chip8* chip8)
{
  chip8->memory = (unsigned char*)malloc(MEMORY_SIZE * sizeof(unsigned char));
  if(!chip8->memory)
  {
    printf("Bad Alloc");
    return false;
  }
  chip8->gfx = (unsigned char*)malloc(SCREEN_WIDTH / 8 * SCREEN_HEIGHT * sizeof(unsigned char));
  if(!chip8->gfx)
  {
    printf("Bad Alloc");
    return false;
  }

  chip8->view = newView();
  if(!chip8->gfx)
  {
    printf("Bad Alloc");
    return false;
  }

  chip8->instructionRegister = 0;
  chip8->I = 0;
  chip8->pc = 200;
  chip8->delay_timer = 0;
  chip8->sound_timer = 0;
  chip8->sp = 0;
  chip8->drawFlag = false;

  for(int i = 0; i < NUM_REGISTERS; ++i)
  {
    chip8->registers[i] = 0;
  }

  for(int i = 0; i < STACK_SIZE; ++i)
  {
    chip8->stack[i] = 0;
  }

  for(int i = 0; i < KEYPAD_SIZE; ++i)
  {
    chip8->key[i] = 0;
  }

  return true;
}

void chip8LoadCodeFromFile(Chip8* chip8, char* fileName)
{
  
}

bool chip8EmulateCycle(Chip8* chip8)
{
  chip8->instructionRegister = chip8->memory[chip8->pc] << 8 
    ||chip8->memory[chip8->pc + 1];
  bool quit;
  viewHandleEvents(&quit);
  if(quit)
    return false;

//  printf("%#x", chip8->instructionRegister);
//  switch(opcode &
    
  return true;
}

Chip8* newChip8()
{
  Chip8* newChip8 = malloc(sizeof(Chip8));
  return newChip8 && chip8Initialize(newChip8) ? newChip8 : NULL;
}
