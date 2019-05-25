#include "keys.h"
#include <SDL2/SDL.h>

char getASCII(Key key)
{
  switch(key)
  {
    case KEY0: return '0';
    case KEY1: return '1';
    case KEY2: return '2';
    case KEY3: return '3';
    case KEY4: return '4';
    case KEY5: return '5';
    case KEY6: return '6';
    case KEY7: return '7';
    case KEY8: return '8';
    case KEY9: return '9';
    case KEYA: return 'A';
    case KEYB: return 'B';
    case KEYC: return 'C';
    case KEYD: return 'D';
    case KEYE: return 'E';
    case KEYF: return 'F';
    default:   return '\0';
  }
}

int getInt(SDL_Keycode sdlKeycode)
{
  switch(sdlKeycode)
  {
    case SDLK_0: return 0;
    case SDLK_1: return 1;
    case SDLK_2: return 2;
    case SDLK_3: return 3;
    case SDLK_4: return 4;
    case SDLK_5: return 5;
    case SDLK_6: return 6;
    case SDLK_7: return 7;
    case SDLK_8: return 8;
    case SDLK_9: return 9;
    case SDLK_a: return 10;
    case SDLK_b: return 11;
    case SDLK_c: return 12;
    case SDLK_d: return 13;
    case SDLK_e: return 14;
    case SDLK_f: return 15;
    default:     return -1;
  }
}
