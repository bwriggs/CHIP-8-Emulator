#include "chip8.h"
#include "view.h"
#include "viewCycleData.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

//some function macros to abbreviate opcode argmuent extraction
#define nnn(instruction) (instruction & 0x0FFF)
#define x(instruction) ((instruction & 0x0F00) >> 8)
#define y(instruction) ((instruction & 0x00F0) >> 4)
#define kk(instruction) (instruction & 0x00FF)

#define KEY_SPRITE_START 0x0
#define PROGRAM_START 0x200

#define KEY_SPRITE_DATA 0xF0, \
0x90, \
0x90, \
0x90, \
0xF0, \
0x20, \
0x60, \
0x20, \
0x20, \
0x70, \
0xF0, \
0x10, \
0xF0, \
0x80, \
0xF0, \
0xF0, \
0x10, \
0xF0, \
0x10, \
0xF0, \
0x90, \
0x90, \
0xF0, \
0x10, \
0x10, \
0xF0, \
0x80, \
0xF0, \
0x10, \
0xF0, \
0xF0, \
0x80, \
0xF0, \
0x90, \
0xF0, \
0xF0, \
0x10, \
0x20, \
0x40, \
0x40, \
0xF0, \
0x90, \
0xF0, \
0x90, \
0xF0, \
0xF0, \
0x90, \
0xF0, \
0x10, \
0xF0, \
0xF0, \
0x90, \
0xF0, \
0x90, \
0x90, \
0xE0, \
0x90, \
0xE0, \
0x90, \
0xE0, \
0xF0, \
0x80, \
0x80, \
0x80, \
0xF0, \
0xE0, \
0x90, \
0x90, \
0x90, \
0xE0, \
0xF0, \
0x80, \
0xF0, \
0x80, \
0xF0, \
0xF0, \
0x80, \
0xF0, \
0x80, \
0x80

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

  free(chip8);
}

void printBeep()
{
  printf("BEEP\n");
}

bool chip8Initialize(Chip8* chip8)
{
  srand(time(0));

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

  chip8->onSoundTimer = &printBeep;

  chip8->I = 0;
  chip8->pc = PROGRAM_START;
  chip8->delayTimer = 0;
  chip8->soundTimer = 0;
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
    chip8->keys[i] = 0;
  }

  unsigned char keySpriteData[] = {KEY_SPRITE_DATA};
  for(int i = 0; i < 0x50; ++i)
  {
    chip8->memory[KEY_SPRITE_START + i] = keySpriteData[i];
  }

  return true;
}

void chip8LoadCodeFromFile(Chip8* chip8, char* fileName)
{
  FILE* file = fopen(fileName, "rb");
  while(!feof(file))
  {
    for(int i = PROGRAM_START; i < MEMORY_SIZE && fread(chip8->memory + i, 1, 1, file); ++i);
  }
  fclose(file);
}

bool chip8EmulateCycle(Chip8* chip8)
{
  if(chip8->pc >= MEMORY_SIZE)
    return false;
  unsigned short instruction = chip8->memory[chip8->pc] << 8 
    | chip8->memory[chip8->pc + 1];
  chip8->pc += 2;

#if __DEBUG__
  //printf("MEM[%d] = %X\n", chip8->pc, instruction);
#endif

  bool isQuit = false;
  ViewCycleData viewCycleData = {&isQuit, chip8->keys};

  viewEmulateCycle(&viewCycleData);
  if(*viewCycleData.isQuit)
    return false;

  bool invalidOpcode = false;
  switch(instruction & 0xF000)
  {
    case 0x0:
      switch(instruction & 0x000F)
      {
        case 0x0:
          for(int i = 0; i < SCREEN_WIDTH / 8 * SCREEN_HEIGHT; ++i)
          {
            chip8->gfx[i] = 0;
          }
          chip8->drawFlag = true;
          break;
        case 0xE:
          chip8->pc = chip8->stack[--chip8->sp];
          break;
        default:
          invalidOpcode = true;
          break;
      }
      break;
    case 0x1 << 0xC:
      chip8->pc = nnn(instruction);
      break;
    case 0x2 << 0xC:
      chip8->stack[chip8->sp++] = chip8->pc;
      chip8->pc = nnn(instruction);
      break;
    case 0x3 << 0xC:
    {
      int x = x(instruction);
      int kk = kk(instruction);
      if(chip8->registers[x] == kk)
      {
        chip8->pc += 2;
      }
      break;
    }
    case 0x4 << 0xC:
    { 
      int x = x(instruction);
      int kk = kk(instruction);
      if(chip8->registers[x] != kk)
      {
        chip8->pc += 2;
      }
      break;
    }
    case 0x5 << 0xC:
    {
      int x = x(instruction);
      int y = y(instruction);
      if(chip8->registers[x] == chip8->registers[y])
      {
        chip8->pc += 2;
      }
      break;
    }
    case 0x6 << 0xC:
    {
      int x = x(instruction);
      unsigned char kk = kk(instruction);
      chip8->registers[x] = kk;
      break;
    }
    case 0x7 << 0xC:
    {
      int x = x(instruction);
      unsigned char kk = kk(instruction);
      chip8->registers[x] += kk;
      break;
    }
    case 0x8 << 0xC:
    {
      int x = x(instruction);
      int y = y(instruction);
      switch(instruction & 0x000F)
      {
        case 0x0:    
          chip8->registers[x] = chip8->registers[y];
          break;
        case 0x1:
          chip8->registers[x] |= chip8->registers[y];
          break;
        case 0x2:
          chip8->registers[x] &= chip8->registers[y];
          break;
        case 0x3:
          chip8->registers[x] ^= chip8->registers[y];
          break;
        case 0x4:
        {
          int sum = (int)(chip8->registers[x]) + chip8->registers[y];
          chip8->registers[0xF] = sum > 255 ? 1 : 0;

          chip8->registers[x] += chip8->registers[y];
          break;
        }
        case 0x5:
          chip8->registers[0xF] = chip8->registers[x] > chip8->registers[y];
          chip8->registers[x] -= chip8->registers[y];
          break;
        case 0x6:
          chip8->registers[0xF] = chip8->registers[x] & 1;
          chip8->registers[x] >>= 1;
          break;
        case 0x7:
          chip8->registers[0xF] = chip8->registers[y] > chip8->registers[x];
          chip8->registers[x] = chip8->registers[y] - chip8->registers[x];
          break;
        case 0xE:
          chip8->registers[0xF] = chip8->registers[x] & 0x80;
          chip8->registers[x] <<= 1;
          break;
        default:
          invalidOpcode = true;
          break;
      }
      break;
    }
    case 0x9 << 0xC:
    {
      int x = x(instruction);
      int y = y(instruction);
      if(chip8->registers[x] != chip8->registers[y])
      {
        chip8->pc += 2;
      }
      break;
    }
    case 0xA << 0xC:
      chip8->I = nnn(instruction);
      break;
    case 0xB << 0xC:
      chip8->pc = (chip8->registers[0] + nnn(instruction)) & 0xFFF;
      break;
    case 0xC << 0xC:
    {
      int x = x(instruction);
      int kk = kk(instruction);
      chip8->registers[x] = (rand() % 256) & kk;
      break;
    }
    case 0xD << 0xC:
    {
      int n = instruction & 0xF;
      int x = chip8->registers[x(instruction)];
      int y = chip8->registers[y(instruction)];
#if __DEBUG__
      printf("Drawing sprite of size %d at (%d,%d)", n, x, y);
#endif
      chip8->registers[0xF] = 0;
      for(int i = 0; i < n; ++i)
      {
        int yline = (y + i) % SCREEN_HEIGHT;
        unsigned char line = chip8->memory[chip8->I + i];
        for(int j = 7, xLine = x / 8, xBit = 7 - (x % 8); j >= 0; --j)
        {
          if(line & (1 << j))
          {
            if(chip8->gfx[yline * SCREEN_WIDTH / 8 + xLine] & (1 << xBit))
            {
              chip8->registers[0xF] = 1;
            }
            chip8->gfx[yline * SCREEN_WIDTH / 8 + xLine] ^= (1 << xBit);
          }
          if(--xBit < 0){
            xBit = 7;
            if(++xLine >= SCREEN_WIDTH / 8)
              xLine = 0;
          }
        }
      }
      chip8->drawFlag = true;
      break;
    }
    case 0xE << 0xC:
    {
      int x = x(instruction);
      switch(instruction & 0xFF)
      {
        case 0x9E:
          if(chip8->keys[chip8->registers[x]])
          {
            chip8->pc += 2;
          }
          break;
        case 0xA1:
          if(!chip8->keys[chip8->registers[x]])
          {
            chip8->pc += 2;
          }
          break;
        default:
          invalidOpcode = true;
          break;
      }
      break;
    }
    case 0xF << 0xC:
    {
      int x = x(instruction);
      switch(instruction & 0xFF)
      {
        case 0x07:
          chip8->registers[x] = chip8->delayTimer;
          break;
        case 0x0A:
          viewAwaitKeyPress(chip8->keys);
          for(int i = 0; i < KEYPAD_SIZE; ++i)
          {
            if(chip8->keys[i])
            {
              chip8->registers[x] = i;
            }
          }
          break;
        case 0x15:
          chip8->delayTimer = chip8->registers[x];
          break;
        case 0x18:
          chip8->soundTimer = chip8->registers[x];
          break;
        case 0x1E:
          chip8->I += chip8->registers[x];
          chip8->registers[0xF] = chip8->I > 0xFFF;
          chip8->I &= 0xFFF;
          break;
        case 0x29:
          chip8->I = KEY_SPRITE_START + 5 * chip8->registers[x];
          break;
        case 0x33:
        {
          int bcd = chip8->registers[x];
          chip8->memory[chip8->I] = (x / 100) % 10;
          chip8->memory[chip8->I + 1] = (x / 10) % 10;
          chip8->memory[chip8->I + 2] = x % 10;        
          break;
        }
        case 0x55:
          for(int i = 0; i < NUM_REGISTERS; ++i)
          {
            chip8->memory[chip8->I + i] = chip8->registers[i];
          }
          break;
        case 0x65:
          for(int i = 0; i < NUM_REGISTERS; ++i)
          {
            chip8->registers[i] = chip8->memory[chip8->I + i];
          }
          break;
        default:
          invalidOpcode = true;
          break;
      }
      break;
    }
  }


  if(chip8->delayTimer > 0)
  {
    chip8->delayTimer--;
  }

  if(chip8->soundTimer != 0)
  {
    if(chip8->soundTimer > 0 && chip8->onSoundTimer)
    {
      (*chip8->onSoundTimer)();
    }
    chip8->soundTimer--;
  }

  return !invalidOpcode;
}

Chip8* newChip8()
{
  Chip8* newChip8 = malloc(sizeof(Chip8));
  return newChip8 && chip8Initialize(newChip8) ? newChip8 : NULL;
}
