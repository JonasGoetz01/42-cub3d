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
#define FOV 60.0f * (M_PI / 180.0f) // Convert degrees to radians
#define NUM_RAYS 60 // Example number of rays
#define BPP sizeof(int32_t)
#define MOVE_SPEED 3.0f

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
    int line_count;
    float scale_factor;
    t_line *lines;
    int window_width;
    int window_height;
} t_global;

void    loop(void *param);
void    keyHook(mlx_key_data_t keydata, void* param);
void    initMap(t_global *global);
void    map_to_line_segments(t_global *global, t_line **lines, int *line_count);
void    draw_line(t_global *global, t_vec2d a, t_vec2d b);
void    showMap(t_global *global);
float   calculate_scale_factor(int map_width, int map_height, int window_width, int window_height);
void    scale_line_segments(t_line *lines, int line_count, float scale_factor);
void    draw_circle(t_global *global, t_circle *circle, int color);
int     get_rgba(int r, int g, int b, int a);
void    draw_ray(t_global *global, t_ray *ray);
t_player *new_player(t_vec2d pos, t_vec2d dir);
t_vec2d ray_line_collision(t_ray *ray, t_line *line);
void    update_position(t_global *global, t_vec2d pos);
void    rotate_player(t_global *global, float angle);
void    raycast(t_global *global);
void    resize(int32_t width, int32_t height, void* param);

#endif