#ifndef __VIEW_H__
#define __VIEW_H__

#include "SDL2/SDL.h"
#include <stdbool.h>
typedef struct
{
  SDL_Window* window;
  SDL_Renderer* renderer;
}View;

View* newView();
void viewUpdate(View* view, unsigned char* gfx);
void viewSetKeys(View* view, unsigned char keys[]);
void viewDestroy(View* view);
void viewHandleEvents(bool* quit);

#endif
