#include "player.h"
#include "constants.h"
#include "game.h"
#include "level.h"
#include "utils.h"
#include "vector.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
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

  player->rays = calloc(NUMBER_OF_RAYS, sizeof(struct vec2));

  struct vec2 *spawn_point = create_vector(x, y);
  player->position = spawn_point;

  initialise_hitbox(&player->hitbox);

  update_hitbox(*player, *player->position);

  return *player;
}

void update_hitbox(struct player player, struct vec2 position) {
  double half_player_width = player.width / 2.0;
  double half_player_height = player.height / 2.0;

  // MAG
  double magnitude =
      vector_magnitude((struct vec2){half_player_width, half_player_height});

  player.hitbox.top_left->x =
      position.x - magnitude * SDL_cos(SDL_PI_D / 4 + player.facing_rads);
  player.hitbox.top_left->y =
      position.y - magnitude * SDL_sin(SDL_PI_D / 4 + player.facing_rads);

  player.hitbox.top_right->x =
      position.x + magnitude * SDL_cos(SDL_PI_D / 4 - player.facing_rads);
  player.hitbox.top_right->y =
      position.y - magnitude * SDL_sin(SDL_PI_D / 4 - player.facing_rads);

  player.hitbox.bottom_left->x =
      position.x - magnitude * SDL_cos(SDL_PI_D / 4 - player.facing_rads);
  player.hitbox.bottom_left->y =
      position.y + magnitude * SDL_sin(SDL_PI_D / 4 - player.facing_rads);

  player.hitbox.bottom_right->x =
      position.x + magnitude * SDL_cos(SDL_PI_D / 4 + player.facing_rads);
  player.hitbox.bottom_right->y =
      position.y + magnitude * SDL_sin(SDL_PI_D / 4 + player.facing_rads);
}

void update_player(struct player *player, struct level level,
                   struct vec2 direction_vector, double turn_velocity,
                   double deltatime) {
  multiply_vector(&direction_vector, player->movement_speed * deltatime);

  player->facing_rads += player->turning_speed * deltatime * turn_velocity;

  struct vec2 new_position = add_vector(*player->position, direction_vector);

  update_hitbox(*player, new_position);
  if (!out_of_bounds(new_position, player->width, player->height) &&
      !colliding_with_walls(player->hitbox, level)) {
    player->position->x = new_position.x;
    player->position->y = new_position.y;
  } else {
    update_hitbox(*player, *player->position);
  }

  raycast(*player, level);
}

void draw_player(SDL_Renderer *renderer, struct player player) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  SDL_RenderLine(renderer, player.hitbox.top_left->x, player.hitbox.top_left->y,
                 player.hitbox.top_right->x, player.hitbox.top_right->y);
  SDL_RenderLine(renderer, player.hitbox.top_right->x,
                 player.hitbox.top_right->y, player.hitbox.bottom_right->x,
                 player.hitbox.bottom_right->y);
  SDL_RenderLine(renderer, player.hitbox.bottom_right->x,
                 player.hitbox.bottom_right->y, player.hitbox.bottom_left->x,
                 player.hitbox.bottom_left->y);
  SDL_RenderLine(renderer, player.hitbox.bottom_left->x,
                 player.hitbox.bottom_left->y, player.hitbox.top_left->x,
                 player.hitbox.top_left->y);

  // draw the rays
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  for (int i = 0; i < NUMBER_OF_RAYS; i++) {
    SDL_RenderLine(renderer, player.position->x, player.position->y,
                   player.rays[i].x, player.rays[i].y);
  }

  // draw 2.5d view
  SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
  SDL_RenderFillRect(renderer, &(SDL_FRect){800, 0, 800, 800});

  // render the walls
  for (int i = 0; i < NUMBER_OF_RAYS; i++) {
    double ray_length = vector_magnitude(player.rays[i]);

    // 0 == 800, 800 = 0
    double depth_factor = (1 - ray_length / MAX_RAY_LENGTH);
    double line_height = (WINDOW_HEIGHT / 2.0) * depth_factor;

    SDL_SetRenderDrawColor(renderer, 180 * depth_factor, 180 * depth_factor,
                           255 * depth_factor, 255);
    SDL_RenderLine(renderer, WINDOW_WIDTH / 2.0 + i,
                   WINDOW_HEIGHT / 2.0 - line_height, WINDOW_WIDTH / 2.0 + i,
                   WINDOW_HEIGHT / 2.0 + line_height);
  }
}

void raycast(struct player player, struct level level) {
  double bearing = player.facing_rads - FOV / 2;

  double step = FOV / (NUMBER_OF_RAYS - 1);

  for (int i = 0; i < NUMBER_OF_RAYS; i++) {
    player.rays[i].x = player.position->x + SDL_sin(bearing) * MAX_RAY_LENGTH;
    player.rays[i].y = player.position->y - SDL_cos(bearing) * MAX_RAY_LENGTH;
    for (int j = 0; j < level.wall_count; j++) {
      line_line(*player.position, player.rays[i], level.walls[j].point1,
                level.walls[j].point2, &player.rays[i]);
    }
    bearing += step;
  }
}