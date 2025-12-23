#include "game.h"
#include "constants.h"
#include "vector.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>
#include <stdio.h>

void update_player(struct player player, struct vec2 direction_vector,
                   double turn_velocity) {

  player.body->x += direction_vector.x;
  player.body->y += direction_vector.y;
  if (player.body->x < 0)
    player.body->x = 0;
  if (player.body->x > WINDOW_WIDTH / 2.0 - player.body->w)
    player.body->x = WINDOW_WIDTH / 2.0 - player.body->w;
  if (player.body->y > WINDOW_HEIGHT - player.body->h)
    player.body->y = WINDOW_HEIGHT - player.body->h;
  if (player.body->y < 0)
    player.body->y = 0;

  *player.facing_rads += turn_velocity;
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
  multiply_vector(&direction, game->player_speed * deltatime);

  printf("direction: %.2f %.2f\n", direction.x, direction.y);

  update_player(game->player, direction,
                turn_velocity * deltatime * game->turning_speed);
}