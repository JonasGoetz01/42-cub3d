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
#define BPP sizeof(int32_t)
#define MOVE_SPEED 0.7f
#define MINIMAP_SCALE 0.3f
#define SHOW_MINIMAP 0
#define SHOW_RAYS 0
#define SHOW_COLLISIONS 0
#define SHOW_FOV 1

#define NORTH_COLOR get_rgba(255, 255, 200, 255) // creme
#define SOUTH_COLOR get_rgba(100, 255, 100, 255) // green
#define EAST_COLOR get_rgba(100, 10, 10, 255) // brown
#define WEST_COLOR get_rgba(140, 20, 100, 255) // purple

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
    t_ray *opponent_rays;
    t_ray *weapon_ray;
} t_player;

typedef struct s_opponent
{
    t_vec2d pos;
    bool visible;
    bool dead;
} t_opponent;

typedef struct s_global
{
    double time;
    mlx_t *mlx;
    mlx_image_t *minimap;
    mlx_image_t *img;
    t_map *map;
    t_player *player;
    int line_count;
    float scale_factor;
    t_line *lines;
    float minimap_scale;
    int window_width;
    int window_height;
    t_opponent *opponent;
    int opponent_count;
} t_global;

void    loop(void *param);
void    keyHook(void* param);
void    initMap(t_global *global);
void    map_to_line_segments(t_global *global, t_line **lines, int *line_count);
void    draw_line(t_global *global, t_vec2d a, t_vec2d b, int color);
void    showMap(t_global *global);
float   calculate_scale_factor(int map_width, int map_height, int window_width, int window_height);
void    scale_line_segments(t_line *lines, int line_count, float scale_factor);
void    draw_circle(t_global *global, t_circle *circle, int color);
int     get_rgba(int r, int g, int b, int a);
void    draw_ray(t_global *global, t_ray *ray);
t_player *new_player(t_global *global, t_vec2d pos, t_vec2d dir);
t_vec2d ray_line_collision(t_ray *ray, t_line *line, t_face *face);
void    update_position(t_global *global, t_vec2d dir, float speed);
void    rotate_player(t_global *global, float angle);
void    raycast(t_global *global);
void    resize(int32_t width, int32_t height, void* param);
t_vec2d get_player_position(t_global *global);
t_vec2d get_player_direction(t_global *global);
void    cursor(double xpos, double ypos, void* param);
void    show_sky_and_floor(t_global *global);
void    make_background_transparent(t_global *global);
void    render_3d(t_global *global);
void    draw_bar(t_global *global, int x, int y, int width, int height, int color);
double  get_current_millis(void);

#endif