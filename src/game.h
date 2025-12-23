#ifndef GAME_H
#define GAME_H

#include "level.h"
#include "player.h"
#include "vector.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

struct game {
  struct player player;
  struct level *level;
  // SDL_FRect* walls;
};

bool out_of_bounds(struct vec2 position, int w, int h);

void handle_game_update(struct game *game, bool *held_keys, double deltatime);

bool colliding_with_wall(struct hitbox hitbox, struct wall wall);

bool colliding_with_walls(struct hitbox hitbox, struct level level);

#endif