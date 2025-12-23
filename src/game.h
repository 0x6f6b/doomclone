#ifndef GAME_H
#define GAME_H

#include "vector.h"
#include <SDL3/SDL_rect.h>

struct player {
  SDL_FRect *body;
  double *facing_rads;
};

struct game {
  struct player player;
  double player_speed;
  double turning_speed;
  // SDL_FRect* walls;
};

void update_player(struct player player, struct vec2 direction_vector,
                   double turn_velocity);

void handle_game_update(struct game *game, bool *held_keys, double deltatime);

#endif