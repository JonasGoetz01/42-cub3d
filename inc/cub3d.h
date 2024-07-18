#ifndef CUB3D_H
# define CUB3D_H

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft.h"
# include "geometry.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

# define WIDTH 1080
# define HEIGHT 720
# define FOV 60.0f * (M_PI / 180.0f)
# define BPP sizeof(int32_t)
# define MOVE_SPEED 0.7f
# define MINIMAP_SCALE 0.3f
# define SHOW_MINIMAP 0
# define SHOW_RAYS 0
# define SHOW_COLLISIONS 0
# define SHOW_FOV 1
# define WALL_BUFFER_DISTANCE 0.2f
# define PLAYER_RADIUS 0.2f
# define INTERACT_DISTANCE 15.0f // maybe change value

# define NC      "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAG     "\033[35m"
# define CYAN    "\033[36m"
# define BOLD    "\033[1m"
# define LINE    "\033[4m"

# define ERR_MALLOC  RED BOLD "ERROR: Malloc failed\n" NC
# define ERR_OPEN    RED BOLD "ERROR: Could not open file "
# define ERR_DIR	 RED BOLD "ERROR: File is a directory\n" NC
# define ERR_PLAYER  RED BOLD "ERROR: Multiple players or no player in map\n" NC
# define ERR_CHAR    RED BOLD "ERROR: Invalid character in map\n" NC
# define ERR_MAP     RED BOLD "ERROR: Map is not closed correctly\n" NC
# define ERR_RANGE   RED BOLD "ERROR: Color value out of range\n" NC
# define ERR         RED BOLD "ERROR: Error\n" NC
# define ERR_NO_ID   RED BOLD "ERROR: Unrecognized identifier\n" NC
# define ERR_D_ID    RED BOLD "ERROR: Double identifier\n" NC
# define ERR_COMMA   RED BOLD "ERROR: Invalid position of commas\n" NC
# define ERR_FORMAT  RED BOLD "ERROR: Invalid color format\n" NC
# define ERR_EMPTY   RED BOLD "ERROR: Empty line in map\n" NC
# define ERR_ARG     RED BOLD "ERROR: Invalid number of arguments\n" NC
# define ERR_DOOR    RED BOLD "ERROR: Invalid door position\n" NC
# define USAGE       BLUE BOLD "USAGE: ./cub3D <[file].cub>\n" NC
typedef struct s_map
{
	char **map;
	int width;
	int height;
	int count;
} t_map;

typedef struct s_texture
{
	char *north;
	char *south;
	char *east;
	char *west;
} t_texture;

typedef struct s_color
{
	int r;
	int g;
	int b;
} t_color;

typedef struct s_identifier_flag
{
	bool no;
	bool so;
	bool we;
	bool ea;
	bool f;
	bool c;
} t_identifier_flag;

typedef struct s_map_size
{
	char	*line;
	char	*tmp;
	int		fd;
	int		height;
	int		max_width;
	int		in_map;
	int		i;
	int		empty_line;
} t_map_size;

typedef struct s_player
{
	t_vec2d		pos;
	t_vec2d		dir;
	t_ray		*rays;
	t_ray		*opponent_rays;
	t_ray		*weapon_ray;
}				t_player;

typedef struct s_opponent
{
	t_vec2d		pos;
	bool		visible;
	bool		dead;
}				t_opponent;

typedef struct s_global
{
	double time;
	mlx_t *mlx;
	mlx_image_t *minimap;
	mlx_image_t *img;
	t_map *map;
	t_texture *texture;
	t_color floor;
	t_color ceiling;
	t_identifier_flag flags;
	t_player *player;
	int line_count;
	float scale_factor;
	t_line *lines;
	float minimap_scale;
	int window_width;
	int window_height;
	t_opponent *opponent;
	int opponent_count;
	t_door *doors;
	int door_count;
	bool open;
	bool close;
	t_line **door_line;
} t_global;

void			loop(void *param);
void			keyHook(void *param);
void			initMap(t_global *global);
void			map_to_line_segments(t_global *global, t_line **lines,
					int *line_count);
void			draw_line(t_global *global, t_vec2d a, t_vec2d b, int color);
void			showMap(t_global *global);
float			calculate_scale_factor(int map_width, int map_height,
					int window_width, int window_height);
void			scale_line_segments(t_line *lines, int line_count,
					float scale_factor);
void			draw_circle(t_global *global, t_circle *circle, int color);
int				get_rgba(int r, int g, int b, int a);
void			draw_ray(t_global *global, t_ray *ray);
t_player		*new_player(t_global *global, t_vec2d pos, t_vec2d dir);
t_vec2d			ray_line_collision(t_ray *ray, t_line *line, t_face *face);
void			update_position(t_global *global, t_vec2d dir, float speed);
void			rotate_player(t_global *global, float angle);
void			raycast(t_global *global);
void			resize(int32_t width, int32_t height, void *param);
t_vec2d			get_player_position(t_global *global);
t_vec2d			get_player_direction(t_global *global);
void			cursor(double xpos, double ypos, void *param);
void			show_sky_and_floor(t_global *global);
void			make_background_transparent(t_global *global);
void			render_3d(t_global *global);
void			draw_bar(t_global *global, int x, int y, int width, int height,
					int color);
double			get_current_millis(void);
void			get_opponents(t_global *global);

int				parse_and_validate(char *file, t_global *global);
bool			valid_file(char *file, int flag);
bool			valid_map(char **map, int height);
bool 			double_identifier(char *identifier, t_global *global);

int				ft_strcmp(const char *s1, const char *s2);
int				strlen_tab_to_space(char *str);
char			*strdup_tab_to_space(const char *s1, int len);
bool			check_arg_amount(char **split, int amount);
bool			valid_range(int *color);
bool			check_identifier(char *line, t_global *global);
bool			color_format(char **color);
bool 			strlen_check(char **split);
int				map_size(char *file, t_global *global);
bool			parse_map(char *line, t_global *global);
bool			parse_line(char *line, t_global *global);

void	get_doors(t_global *global);
float	get_distance(t_vec2d a, t_vec2d b);
void	update_door_segments(t_global *global);
#endif