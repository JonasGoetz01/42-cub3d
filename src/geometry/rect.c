#include "../../inc/cub3d.h"

void    rect_2(t_point a, t_point b, uint32_t color, t_global *global)
{
    line(a, (t_point){b.x, a.y}, color, global);
    line((t_point){b.x, a.y}, b, color, global);
    line(b, (t_point){a.x, b.y}, color, global);
    line((t_point){a.x, b.y}, a, color, global);
}

void    rect_4(t_point a, t_point b, t_point c, t_point d, uint32_t color, t_global *global)
{
    line(b, c, color, global);
    line(b, d, color, global);
    line(a, d, color, global);
    line(c, a, color, global);
}
