#ifndef GLOBAL_H
# define GLOBAL_H

#include "../lib/MLX42/include/MLX42/MLX42.h"

typedef struct s_global {
    mlx_t		*mlx;
	mlx_image_t	*img;
} t_global;

#endif