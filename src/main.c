#include "constants.h"
#include "events.h"
#include "game.h"
#include <SDL3/SDL.h>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <stdio.h>

void update_renderer(SDL_Renderer *renderer, struct game *game,
                     SDL_Texture *player_texture) {
  // Draw to screen
  SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
  SDL_RenderClear(renderer);

  SDL_RenderTextureRotated(renderer, player_texture, NULL, game->player.body,
                           *(game->player.facing_rads) * (180 / 3.14), NULL,
                           SDL_FLIP_NONE);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderLine(renderer, WINDOW_WIDTH / 2.0, 0, WINDOW_WIDTH / 2.0,
                 WINDOW_HEIGHT);

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

  // Texture for player

  // GAME
  double facing = 0;
  struct player player = {&(SDL_FRect){0, 0, 50, 50}, &facing};
  struct game game = {player, 500.0, 5.0};

  SDL_Surface *player_surface =
      SDL_CreateSurface(player.body->w, player.body->h, SDL_PIXELFORMAT_RGBA32);

  SDL_FillSurfaceRect(player_surface, &(SDL_Rect){0, 0, 50, 50}, 0xff0000ff);

  SDL_Texture *player_texture =
      SDL_CreateTextureFromSurface(renderer, player_surface);

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

      update_renderer(renderer, &game, player_texture);
    }
  }

  SDL_Quit();

  return 0;
}