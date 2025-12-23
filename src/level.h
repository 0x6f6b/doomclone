#ifndef LEVEL_H
#define LEVEL_H

#include "vector.h"
#include <SDL3/SDL.h>

struct wall {
  struct vec2 point1;
  struct vec2 point2;
  SDL_Color color;
};

struct level {
  struct wall *walls;
  int wall_count;
};

struct wall create_wall(double x1, double y1, double x2, double y2,
                        SDL_Color color);

struct level *create_level();

void draw_level(SDL_Renderer *renderer, struct level *level);

#endif