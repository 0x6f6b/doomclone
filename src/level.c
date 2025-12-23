#include "level.h"
#include "vector.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <stdlib.h>

struct wall create_wall(double x1, double y1, double x2, double y2,
                        SDL_Color color) {
  struct wall wall;

  struct vec2 point1 = (struct vec2){x1, y1};
  struct vec2 point2 = (struct vec2){x2, y2};

  wall.point1 = point1;
  wall.point2 = point2;
  wall.color = color;

  return wall;
}

struct level *create_level() {
  struct wall *walls = calloc(10000, sizeof(struct wall));

  for (int i = 0; i < 10000; i++) {

    walls[i] = create_wall(i * (800.0 / 10000), 100, (i + 1) * (800.0 / 10000),
                           300, (SDL_Color){0, 255, 0, 255});
  }

  struct level *level = calloc(1, sizeof(struct level));
  level->walls = walls;
  level->wall_count = 10000;

  return level;
}

void draw_level(SDL_Renderer *renderer, struct level *level) {
  for (int i = 0; i < level->wall_count; i++) {
    struct wall wall = level->walls[i];
    SDL_SetRenderDrawColor(renderer, wall.color.r, wall.color.g, wall.color.b,
                           wall.color.a);
    SDL_RenderLine(renderer, wall.point1.x, wall.point1.y, wall.point2.x,
                   wall.point2.y);
  }
}