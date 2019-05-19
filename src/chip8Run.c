#include "chip8.h"
#include "view.h"
#include <stdio.h>

#define FRAME_RATE 60

int main(int argc, char* argv[])
{
  Chip8* chip8 = newChip8();
  if(!chip8)
  {
    return 1;
  }
  
  chip8LoadCodeFromFile(chip8, "pong.rom");
  
  for(;;)
  { 
    Uint32 ticksAtStart = SDL_GetTicks();

    if(!chip8EmulateCycle(chip8))
    {
      break;
    }

    if(chip8->drawFlag)
    {
      viewUpdate(chip8->view, chip8->gfx);
    }

    SDL_Delay(1000 / FRAME_RATE - (SDL_GetTicks() - ticksAtStart));
  }

  chip8Destroy(chip8);

  return 0;
}
