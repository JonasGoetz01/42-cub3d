#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft.h"
#include "geometry.h"

#define WIDTH 1080
#define HEIGHT 720
#define NUM_RAYS 30
#define FOV 60
#define BPP sizeof(int32_t)

typedef struct s_map
{
    char **map;
    int width;
    int height;
} t_map;

typedef struct s_player
{
    t_vec2d pos;
    t_vec2d dir;
    t_ray *rays;
} t_player;

typedef struct s_global
{
    mlx_t *mlx;
    mlx_image_t *img;
    t_map *map;
    t_player *player;
} t_global;

void    loop(void *param);
void    keyHook(mlx_key_data_t keydata, void* param);
void    initMap(t_global *global);
void map_to_line_segments(t_global *global, t_line **lines, int *line_count);

#endif