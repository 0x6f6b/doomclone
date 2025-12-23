#include "vector.h"
#include <math.h>
#include <stdlib.h>

void multiply_vector(struct vec2 *vector, double multiplier) {
  vector->x *= multiplier;
  vector->y *= multiplier;
}

struct vec2 add_vector(struct vec2 vector1, struct vec2 vector2) {
  struct vec2 *result_vector = malloc(sizeof(struct vec2));
  result_vector->x = vector1.x + vector2.x;
  result_vector->y = vector1.y + vector2.y;

  return *result_vector;
}

void divide_vector(struct vec2 *vector, double divisor) {
  if (divisor == 0)
    return;

  vector->x /= divisor;
  vector->y /= divisor;
}

void normalise(struct vec2 *vector) {
  double magnitude = sqrt(pow(vector->x, 2) + pow(vector->y, 2));

  if (magnitude == 0)
    return;

  divide_vector(vector, magnitude);
}

struct vec2 *create_vector(int x, int y) {
  struct vec2 *vector = malloc(sizeof(struct vec2));
  vector->x = x;
  vector->y = y;

  return vector;
}