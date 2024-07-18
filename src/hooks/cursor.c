#include "../../inc/cub3d.h"

void	cursor(double xpos, double ypos, void *param)
{
	t_global	*global;
	double		x_offset;
	int			factor;

	(void)ypos;
	global = (t_global *)param;
	if (global->free_mouse)
	{
		mlx_set_cursor_mode(global->mlx, MLX_MOUSE_NORMAL);
		return ;
	}
	else
		mlx_set_cursor_mode(global->mlx, MLX_MOUSE_HIDDEN);
	x_offset = xpos - (double)global->window_width / 2;
	factor = (int)fabs(x_offset) / 10;
	if (x_offset > 0)
		rotate_player(global, 0.01f * (float)factor);
	else if (x_offset < 0)
		rotate_player(global, -0.01f * (float)factor);
	mlx_set_mouse_pos(global->mlx, global->window_width / 2,
		global->window_height / 2);
}
