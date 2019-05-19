#include "view.h"
#include "SDL2/SDL.h"
#include "chip8.h"
#include <stdbool.h>
#include <stdlib.h>

#define SCALE 20

bool sdlInited = false;

bool viewInitialize(View* view);

View* newView()
{
  View* newView = (View*)malloc(sizeof(View));
  return newView && viewInitialize(newView) ? newView : NULL;
}

void viewDestroy(View* view)
{
  if(view->window)
  {
    SDL_DestroyWindow(view->window);
    view->window = NULL;
  }

  if(view->renderer)
  {
    SDL_DestroyRenderer(view->renderer);
    view->renderer = NULL;
  }

  SDL_Quit();
}

bool viewInitialize(View* view)
{
  Uint32 flagsToUse = SDL_INIT_VIDEO;

  if(!sdlInited)
  {
    SDL_Init(flagsToUse);
    sdlInited = true;
  }
  else
  {
    Uint32 initedFlags = SDL_WasInit(SDL_INIT_EVERYTHING);
    Uint32 flagsToInit = flagsToUse & ~initedFlags;
    if(flagsToInit != 0)
    {
      SDL_InitSubSystem(flagsToInit);
    }
  }

  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  SDL_CreateWindowAndRenderer(SCALE * SCREEN_WIDTH, SCALE * SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
  if(!(window && renderer))
  {
    printf("Could not create window or renderer");
    return false;
  }

  view->window = window;
  view->renderer = renderer;

  return true;
}

void _viewUpdate(View* view, unsigned char* gfx, int scale)
{
  SDL_Rect rect;
  rect.x = scale;
  rect.y = scale;
  rect.w = scale;
  rect.h = scale;

  for(int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    for(int x = 0; x < SCREEN_WIDTH / 8; ++x)
    {
      for(int mask = 1 << 7; mask > 0; mask >>= 1)
      {
        if((mask & gfx[y * SCREEN_WIDTH * 8]) == 1)
        {
          SDL_SetRenderDrawColor(view->renderer, 0, 0, 0, 255);
        }
        else
        {
          SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, 255);
        }
        SDL_RenderDrawRect(view->renderer, &rect);
      }
    }
  }
}

void viewUpdate(View* view, unsigned char* gfx)
{
  _viewUpdate(view, gfx, SCALE);
}

void viewHandleEvents(bool* quit)
{
  *quit = false;

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_QUIT:
        *quit = true;
        return;
      default:
        break;
    }
  }
}
