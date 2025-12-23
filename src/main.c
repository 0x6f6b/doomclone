#include "constants.h"
#include "events.h"
#include "game.h"
#include "level.h"
#include "player.h"
#include <SDL3/SDL.h>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <stdio.h>

void update_renderer(SDL_Renderer *renderer, struct game *game) {
  // Draw to screen
  SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
  SDL_RenderClear(renderer);

  draw_player(renderer, game->player.hitbox);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderLine(renderer, WINDOW_WIDTH / 2.0, 0, WINDOW_WIDTH / 2.0,
                 WINDOW_HEIGHT);

  // DRAW THE LEVEL
  draw_level(renderer, game->level);

  SDL_RenderPresent(renderer);
}

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL Didn't Initialise - Error:%s\n", SDL_GetError());
  }

  SDL_Window *window;
  SDL_Renderer *renderer;

  if (!SDL_CreateWindowAndRenderer(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                                   &window, &renderer)) {
    fprintf(stderr, "Window and Renderer Didn't Initialise - Error:%s\n",
            SDL_GetError());
  }

  // GAME
  struct player player =
      create_player(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, 50, 50, 500.0, 200.0);

  printf("Created player\n");

  struct level *level = create_level();
  printf("Created level\n");

  struct game game = {player, level};

  // KEYS HELD
  bool held_keys[SDL_SCANCODE_COUNT];
  memset(held_keys, false, SDL_SCANCODE_COUNT);
  // Some blocking code
  bool running = true;

  Uint64 frame1, frame2 = 0;
  double deltatime = 0;

  while (running) {
    handle_events(&running, held_keys);

    // LIMIT FRAME RATE
    frame2 = SDL_GetTicksNS();
    deltatime = (double)(frame2 - frame1) / NANOSECONDS_IN_SECOND;
    if (deltatime >= 1.0f / FPS) {
      // printf("FPS:%.2f\n", 1.0f / deltatime);

      frame1 = frame2;
      handle_game_update(&game, held_keys, deltatime);

      update_renderer(renderer, &game);
    }
  }

  SDL_Quit();

  return 0;
}