#include "utils.h"
#include "vector.h"
#include <stdbool.h>
#include <wchar.h>

bool line_line(struct vec2 p1, struct vec2 p2, struct vec2 p3, struct vec2 p4,
               struct vec2 *intersection) {
  float uA = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) /
             ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

  float uB = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) /
             ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

  if ((uA >= 0 && uA <= 1) && (uB >= 0 && uB <= 1)) {
    if (intersection != NULL) {
      intersection->x = p1.x + uA * (p2.x - p1.x);
      intersection->y = p1.y + uA * (p2.y - p1.y);
    }

    return true;
  }
  return false;
}