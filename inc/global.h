#ifndef GLOBAL_H
# define GLOBAL_H

#include "../lib/MLX42/include/MLX42/MLX42.h"
#include "player.h"

typedef struct s_map t_map;

typedef struct s_global {
    mlx_t		*mlx;
	mlx_image_t	*img;
    t_map       *map;
    t_player    *player;
    int         xfactor;
    int         yfactor;
} t_global;

#endif