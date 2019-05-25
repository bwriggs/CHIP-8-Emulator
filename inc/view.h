#ifndef __VIEW_H__
#define __VIEW_H__

#include "SDL2/SDL.h"
#include "viewCycleData.h"
#include <stdbool.h>

typedef struct
{
  SDL_Window* window;
  SDL_Renderer* renderer;
}View;

View* newView();
void viewUpdate(View* view, unsigned char* gfx);
void viewEmulateCycle(ViewCycleData* viewCycleData);
void viewDestroy(View* view);
void viewAwaitKeyPress(unsigned char* keys);

#endif
