#ifndef RECT_H
#define RECT_H

#include "point.h"
#include "global.h"

void    rect_2(t_point a, t_point b, uint32_t color, t_global *global);
void    rect_4(t_point a, t_point b, t_point c, t_point d, uint32_t color, t_global *global);

#endif