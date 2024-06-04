#include "../../inc/cub3d.h"

void loop(void *param)
{
	t_global *global;

    global = NULL;
    global = (t_global *)param;
    mlx_delete_image(global->mlx, global->img);
    global->img = mlx_new_image(global->mlx, WIDTH, HEIGHT);
    t_point p1;
    t_point p2;
    t_point p3;
    t_point p4;

    p1.x = 10;
    p1.y = 10;
    p2.x = 500;
    p2.y = 500;
    p3.x = 500;
    p3.y = 10;
    p4.x = 10;
    p4.y = 500;

    // line(p1, p2, get_rgba(255, 255, 255, 255), global);
    rect_2(p1, p2, get_rgba(255, 255, 255, 255), global);
    // rect_4(p1, p2, p3, p4, get_rgba(255, 255, 255, 255), global);
    mlx_image_to_window(global->mlx, global->img, 0, 0);
}
