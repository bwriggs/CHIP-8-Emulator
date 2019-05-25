#include "keys.h"
#include "events.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

//handles any SDL_Events since last poll and runs an array
//returns true if any events were handled, false otherwise
bool customPollEvent(EventHandler* eventHandlers, ObjHolder** objHolders, int n){
  SDL_Event event;
  if(!SDL_PollEvent(&event))
  {
    return false;
  }
  do
  {
    for(int i = 0; i < n; ++i)
    {
      (*(*(eventHandlers + i)))(&event, (*(objHolders + i))->obj);
    }
  }while(SDL_PollEvent(&event));

  return true;
}
