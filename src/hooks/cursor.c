#include "../../inc/cub3d.h"

void cursor(double xpos, double ypos, void* param)
{
    (void)ypos;
    t_global *global = (t_global*)param;
    double xoffset = xpos - global->window_width / 2;
    int factor = fabs(xoffset) / 10;
    if (xoffset > 0)
        rotate_player(global, 0.01f * factor);
    else if (xoffset < 0)
        rotate_player(global, -0.01f * factor);

    mlx_set_mouse_pos(global->mlx, global->window_width / 2, global->window_height / 2);
}
