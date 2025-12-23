#include "player.h"
#include "game.h"
#include "level.h"
#include "vector.h"
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

void initialise_hitbox(struct hitbox *hitbox) {
  struct vec2 *top_left = malloc(sizeof(struct vec2));
  struct vec2 *top_right = malloc(sizeof(struct vec2));
  struct vec2 *bottom_left = malloc(sizeof(struct vec2));
  struct vec2 *bottom_right = malloc(sizeof(struct vec2));

  hitbox->bottom_left = bottom_left;
  hitbox->bottom_right = bottom_right;
  hitbox->top_left = top_left;
  hitbox->top_right = top_right;
}

struct player create_player(int x, int y, int w, int h, double movement_speed,
                            double turning_speed) {
  struct player *player = malloc(sizeof(struct player));

  player->facing_rads = 0;
  player->movement_speed = movement_speed;
  player->turning_speed = turning_speed;
  player->width = w;
  player->height = h;

  struct vec2 *spawn_point = create_vector(x, y);
  player->position = spawn_point;

  initialise_hitbox(&player->hitbox);

  update_hitbox(*player, *player->position);

  return *player;
}

void update_hitbox(struct player player, struct vec2 position) {
  double half_player_width = player.width / 2.0;
  double half_player_height = player.height / 2.0;

  player.hitbox.top_left->x = position.x - half_player_width;
  player.hitbox.top_left->y = position.y - half_player_height;

  player.hitbox.top_right->x = position.x + half_player_width;
  player.hitbox.top_right->y = position.y - half_player_height;

  player.hitbox.bottom_left->x = position.x - half_player_width;
  player.hitbox.bottom_left->y = position.y + half_player_height;

  player.hitbox.bottom_right->x = position.x + half_player_width;
  player.hitbox.bottom_right->y = position.y + half_player_height;
}

void update_player(struct player player, struct level level,
                   struct vec2 direction_vector, double turn_velocity,
                   double deltatime) {
  multiply_vector(&direction_vector, player.movement_speed * deltatime);

  player.facing_rads += player.turning_speed * deltatime * turn_velocity;

  struct vec2 new_position = add_vector(*player.position, direction_vector);

  update_hitbox(player, new_position);
  if (!out_of_bounds(new_position, player.width, player.height) &&
      !colliding_with_walls(player.hitbox, level)) {
    player.position->x = new_position.x;
    player.position->y = new_position.y;
  } else {
    update_hitbox(player, *player.position);
  }
}

void draw_player(SDL_Renderer *renderer, struct hitbox hitbox) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  SDL_RenderLine(renderer, hitbox.top_left->x, hitbox.top_left->y,
                 hitbox.top_right->x, hitbox.top_right->y);
  SDL_RenderLine(renderer, hitbox.top_right->x, hitbox.top_right->y,
                 hitbox.bottom_right->x, hitbox.bottom_right->y);
  SDL_RenderLine(renderer, hitbox.bottom_right->x, hitbox.bottom_right->y,
                 hitbox.bottom_left->x, hitbox.bottom_left->y);
  SDL_RenderLine(renderer, hitbox.bottom_left->x, hitbox.bottom_left->y,
                 hitbox.top_left->x, hitbox.top_left->y);
}