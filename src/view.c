#include "view.h"
#include "SDL2/SDL.h"
#include "chip8.h"
#include "events.h"
#include "viewCycleData.h"
#include "keys.h"
#include <stdbool.h>
#include <stdlib.h>

#define SCALE 20

int count = 0;

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

  free(view);

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
  
  rect.w = scale * SCREEN_WIDTH;
  rect.h = scale * SCREEN_HEIGHT;
  rect.x = 0;
  rect.y = 0;
  SDL_SetRenderDrawColor(view->renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(view->renderer, &rect);
  SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, 255);
  rect.w = scale;
  rect.h = scale;

  for(int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    for(int x = 0; x < SCREEN_WIDTH / 8; ++x)
    {
      for(int bitIndex = 7; bitIndex >= 0; --bitIndex)
      {
        if((1 << bitIndex) & gfx[y * SCREEN_WIDTH / 8 + x])
        {
          rect.x = (x * 8 + 7 - bitIndex) * scale;
          rect.y = y * scale;
          SDL_RenderFillRect(view->renderer, &rect);
        }
      }
    }
  }

  SDL_RenderPresent(view->renderer);
}

void viewUpdate(View* view, unsigned char* gfx)
{
  _viewUpdate(view, gfx, SCALE);
}


void isQuitEvent(SDL_Event* event, void* obj)
{
//  printf("(%d, %d) ", event->type, SDL_WINDOWEVENT_CLOSE);
  *(bool*)obj |= event->type == SDL_QUIT;
}

void getPressedKey(SDL_Event* event, void* obj)
{
  unsigned char* keys = (unsigned char*)obj;
  if(event->type != SDL_KEYDOWN)
  {
    return;
  }

  int index = getInt(event->key.keysym.sym);
  if(index >= 0)
  {
    *(unsigned char*)(keys + index) = 1;
  }
}

void viewEmulateCycle(ViewCycleData* viewCycleData)
{
  int n = 2;

  EventHandler eventHandler[] = {&isQuitEvent, &getPressedKey};
  
  ObjHolder quitObjHolder = {viewCycleData->isQuit};
  ObjHolder keyObjHolder = {viewCycleData->keys};
  ObjHolder* objHolders[] = {&quitObjHolder, &keyObjHolder};
  
  customPollEvent(eventHandler, objHolders, 2);
}

void viewAwaitKeyPress(unsigned char* keys)
{
  int n = 1;

  EventHandler eventHandlers[] = {&getPressedKey};
  ObjHolder keyObjHolder = {keys};
  ObjHolder* objHolders[] = {&keyObjHolder}; 

  for(;;)
  {
    if(customPollEvent(eventHandlers, objHolders, 1))
    {
      for(int i = 0; i < KEYPAD_SIZE; ++i)
      {
        if(keys[i])
        {
          break;
        }
      }
    }
  }
}
