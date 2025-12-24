#include "game.h"
#include "constants.h"
#include "utils.h"
#include "vector.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>
#include <stdio.h>

bool out_of_bounds(struct vec2 position, int w, int h) {
  if (position.x < w / 2.0)
    return true;
  if (position.x > (WINDOW_WIDTH - w) / 2.0)
    return true;
  if (position.y > WINDOW_HEIGHT - h / 2.0)
    return true;
  if (position.y < h / 2.0)
    return true;

  return false;
}

bool colliding_with_wall(struct hitbox hitbox, struct wall wall) {
  bool top = line_line(wall.point1, wall.point2, *hitbox.top_left,
                       *hitbox.top_right, NULL);
  bool right = line_line(wall.point1, wall.point2, *hitbox.top_right,
                         *hitbox.bottom_right, NULL);
  bool bottom = line_line(wall.point1, wall.point2, *hitbox.bottom_right,
                          *hitbox.bottom_left, NULL);
  bool left = line_line(wall.point1, wall.point2, *hitbox.bottom_left,
                        *hitbox.top_left, NULL);

  return (top || right || bottom || left);
}

bool colliding_with_walls(struct hitbox hitbox, struct level level) {
  for (int i = 0; i < level.wall_count; i++) {
    if (colliding_with_wall(hitbox, level.walls[i])) {
      return true;
    }
  }
  return false;
}

void handle_game_update(struct game *game, bool *held_keys, double deltatime) {
  struct vec2 direction = (struct vec2){0, 0};
  int turn_velocity = 0;

  if (held_keys[SDL_SCANCODE_W]) {
    direction.y--;
  }
  if (held_keys[SDL_SCANCODE_A]) {
    direction.x--;
  }
  if (held_keys[SDL_SCANCODE_S]) {
    direction.y++;
  }
  if (held_keys[SDL_SCANCODE_D]) {
    direction.x++;
  }
  if (held_keys[SDL_SCANCODE_RIGHT]) {
    turn_velocity++;
  }
  if (held_keys[SDL_SCANCODE_LEFT]) {
    turn_velocity--;
  }

  normalise(&direction);

  update_player(&game->player, *game->level, direction, turn_velocity,
                deltatime);
}