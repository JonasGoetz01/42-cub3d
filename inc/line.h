#ifndef LINE_H
# define LINE_H

#include "point.h"
#include "global.h"

void    line(t_point a, t_point b, uint32_t color, t_global *global);
void    line_angle(t_point a, double angle, uint32_t color, t_global *global);

#endif