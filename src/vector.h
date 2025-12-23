#ifndef VECTOR_H
#define VECTOR_H

struct vec2 {
  double x;
  double y;
};

void multiply_vector(struct vec2 *vector, double multiplier);
void divide_vector(struct vec2 *vector, double divisor);
void normalise(struct vec2 *vector);

#endif