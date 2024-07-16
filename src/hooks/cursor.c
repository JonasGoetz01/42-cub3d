#include "../../inc/cub3d.h"

void	cursor(double xpos, double ypos, void *param)
{
	t_global	*global;
	double		xoffset;
	int			factor;

	(void)ypos;
	global = (t_global *)param;
	xoffset = xpos - global->window_width / 2;
	factor = fabs(xoffset) / 10;
	if (xoffset > 0)
		rotate_player(global, 0.01f * factor);
	else if (xoffset < 0)
		rotate_player(global, -0.01f * factor);
	mlx_set_mouse_pos(global->mlx, global->window_width / 2,
		global->window_height / 2);
}
