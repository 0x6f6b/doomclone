#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>

void handle_events(bool *running, bool *held_keys) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_EVENT_QUIT:
    *running = false;
    break;
  case SDL_EVENT_KEY_DOWN:
    held_keys[event.key.scancode] = true;
    break;
  case SDL_EVENT_KEY_UP:
    held_keys[event.key.scancode] = false;
    break;
  }
}