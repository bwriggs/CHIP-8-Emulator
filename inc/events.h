#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "objHolder.h"

typedef void (*EventHandler)(SDL_Event*, void*);

bool customPollEvent(EventHandler* eventHandlers, ObjHolder** objHolders, int n);

#endif
