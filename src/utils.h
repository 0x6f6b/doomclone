#ifndef UTILS_H
#define UTILS_H

#include "vector.h"
#include <stdbool.h>

bool line_line(struct vec2 p1, struct vec2 p2, struct vec2 p3, struct vec2 p4,
               struct vec2 *intersection);

#endif