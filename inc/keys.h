#ifndef _KEYS_H__
#define _KEYS_H__

#include <SDL2/SDL.h>

typedef enum _key
{
  KEY0,
  KEY1,
  KEY2,
  KEY3,
  KEY4,
  KEY5,
  KEY6,
  KEY7,
  KEY8,
  KEY9,
  KEYA,
  KEYB,
  KEYC,
  KEYD,
  KEYE,
  KEYF
}Key;

char getASCII(Key key);
int getInt(SDL_Keycode);

#endif
