#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"
#include "vector.h"
#include <SDL3/SDL.h>

struct hitbox {
  struct vec2 *top_left;
  struct vec2 *top_right;
  struct vec2 *bottom_left;
  struct vec2 *bottom_right;
};

struct player {
  struct vec2 *position;
  struct hitbox hitbox;
  int width;
  int height;
  double facing_rads;
  double movement_speed;
  double turning_speed;
};

struct player create_player(int x, int y, int w, int h, double movement_speed,
                            double turning_speed);

void update_hitbox(struct player player, struct vec2 position);

void update_player(struct player player, struct level level,
                   struct vec2 direction_vector, double turn_velocity,
                   double deltatime);

void draw_player(SDL_Renderer *renderer, struct hitbox hitbox);

#endif