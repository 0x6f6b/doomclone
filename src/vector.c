#include "vector.h"
#include <math.h>

void multiply_vector(struct vec2 *vector, double multiplier) {
  vector->x *= multiplier;
  vector->y *= multiplier;
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