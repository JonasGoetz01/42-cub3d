/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 15:55:48 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 15:10:54 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft.h"
# include "geometry.h"
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define WIDTH 1080
# define HEIGHT 720
# define FOV 60.0f * (M_PI / 180.0f)
# define BPP sizeof(int32_t)
# define MOVE_SPEED 0.7f
# define MINIMAP_SCALE 0.3f
# define SHOW_MINIMAP 1
# define SHOW_RAYS 0
# define SHOW_COLLISIONS 0
# define SHOW_FOV 1
# define WALL_BUFFER_DISTANCE 0.2f
# define PLAYER_RADIUS 0.2f
# define INTERACT_MAX 2.0f // maybe change value
# define INTERACT_MIN 0.3f // maybe change value

# define NC "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAG "\033[35m"
# define CYAN "\033[36m"
# define BOLD "\033[1m"
# define LINE "\033[4m"

# define ERR_MALLOC RED BOLD "ERROR: Malloc failed\n" NC
# define ERR_OPEN RED BOLD "ERROR: Could not open file "
# define ERR_DIR RED BOLD "ERROR: File is a directory\n" NC
# define ERR_PLAYER RED BOLD "ERROR: Multiple players or no player in map\n" NC
# define ERR_CHAR RED BOLD "ERROR: Invalid character in map\n" NC
# define ERR_MAP RED BOLD "ERROR: Map is not closed correctly\n" NC
# define ERR_RANGE RED BOLD "ERROR: Color value out of range\n" NC
# define ERR RED BOLD "ERROR: Error\n" NC
# define ERR_NO_ID RED BOLD "ERROR: Unrecognized identifier\n" NC
# define ERR_D_ID RED BOLD "ERROR: Double identifier\n" NC
# define ERR_COMMA RED BOLD "ERROR: Invalid position of commas\n" NC
# define ERR_FORMAT RED BOLD "ERROR: Invalid color format\n" NC
# define ERR_EMPTY RED BOLD "ERROR: Empty line in map\n" NC
# define ERR_ARG RED BOLD "ERROR: Invalid number of arguments\n" NC
# define ERR_DOOR RED BOLD "ERROR: Invalid door position\n" NC
# define USAGE BLUE BOLD "USAGE: ./cub3D <[file].cub>\n" NC

typedef struct s_map
{
	char				**map;
	int					width;
	int					height;
	int					count;
}						t_map;

typedef struct s_texture
{
	char				*north;
	char				*south;
	char				*east;
	char				*west;
}						t_texture;

typedef struct s_color
{
	int					r;
	int					g;
	int					b;
}						t_color;

typedef struct s_identifier_flag
{
	bool				no;
	bool				so;
	bool				we;
	bool				ea;
	bool				f;
	bool				c;
}						t_identifier_flag;

typedef struct s_map_size
{
	char				*line;
	char				*tmp;
	int					fd;
	int					height;
	int					max_width;
	int					in_map;
	int					i;
	int					empty_line;
}						t_map_size;

typedef struct s_player
{
	t_vec2d				pos;
	t_vec2d				dir;
	t_ray				*rays;
}						t_player;

typedef struct s_segment
{
	t_vec2d				a;
	t_vec2d				b;
	t_alignment			alignment;
	t_type				type;
	t_door				*door;
	t_vec2d				open_end;
	t_vec2d				close_end;
}						t_segment;

typedef struct s_add_segments
{
	t_vec2d				top_left;
	t_vec2d				top_right;
	t_vec2d				bottom_left;
	t_vec2d				bottom_right;
	t_vec2d				left_middle;
	t_vec2d				right_middle;
	t_door				*door;
	float				scaled_x;
	float				scaled_y;
	int					i;
	int					x;
	int					y;
}						t_add_segments;

typedef struct s_render
{
	int					bar_width;
	int					bar_height;
	int					i;
	t_ray				*ray;
	t_collision			*closest_collision;
	float				distance;
	int					center_y;
	int					bottom_y;
	int					top_y;
	int					x;
	float				hit_percentage;
	int					color;
	mlx_texture_t		texture;
	uint8_t				*pixel;
	int					draw_y;
	int					texture_x;
	int					texture_y;
	float				perpendicular_distance;
	float				z_buffer[WIDTH];
	float				ray_angle;
	float				player_angle;
	float				angle_diff;
	int					r;
	int					g;
	int					b;
	int					j;
}						t_render;

typedef struct s_global
{
	double				time;
	mlx_t				*mlx;
	mlx_texture_t		*sprite_textures[4];
	int					sprite_index;
	mlx_image_t			*minimap;
	mlx_image_t			*img;
	t_map				*map;
	t_player			*player;
	int					line_count;
	float				scale_factor;
	t_line				*lines;
	float				minimap_scale;
	int					window_width;
	int					window_height;
	int					i;
	int					j;
	bool				free_mouse;
	t_texture			*texture;
	t_color				floor;
	t_color				ceiling;
	t_identifier_flag	flags;
	t_door				*doors;
	int					door_count;
	bool				open;
	bool				close;
	t_render			ren;
	t_line				**door_line;
}						t_global;

void					ft_exit_free(t_global *global);
void					loop(void *param);
void					keyHook(void *param);
void					initMap(t_global *global);
void					map_to_line_segments(t_global *global, t_line **lines,
							int *line_count);
void					draw_line(t_global *global, t_vec2d a, t_vec2d b,
							int color);
void					draw_line_crosshair(t_global *global, t_vec2d a,
							t_vec2d b, int color);
void					showMap(t_global *global);
float					calculate_scale_factor(int map_width, int map_height,
							int window_width, int window_height);
void					scale_line_segments(t_line *lines, int line_count,
							float scale_factor);
void					draw_circle(t_global *global, t_circle *circle,
							int color);
int						get_rgba(int r, int g, int b, int a);
void					draw_ray(t_global *global, t_ray *ray);
t_player				*new_player(t_global *global, t_vec2d pos, t_vec2d dir);
t_vec2d					ray_line_collision(t_ray *ray, t_line *line,
							t_face *face);
void					update_position(t_global *global, t_vec2d dir,
							float speed);
void					rotate_player(t_global *global, float angle);
void					raycast(t_global *global);
void					resize(int32_t width, int32_t height, void *param);
t_vec2d					get_player_position(t_global *global);
t_vec2d					get_player_direction(t_global *global);
void					cursor(double xpos, double ypos, void *param);
void					make_background_transparent(t_global *global);
void					render_3d(t_global *global);
void					draw_bar(t_global *global, int x, int y, int width,
							int height, int color);
double					get_current_millis(void);
void					key_hook(mlx_key_data_t keydata, void *param);

void					loop(void *param);
void					key_hook_movement(void *param);
void					initMap(t_global *global);
void					map_to_line_segments(t_global *global, t_line **lines,
							int *line_count);
void					draw_line(t_global *global, t_vec2d a, t_vec2d b,
							int color);
void					showMap(t_global *global);
float					calculate_scale_factor(int map_width, int map_height,
							int window_width, int window_height);
void					scale_line_segments(t_line *lines, int line_count,
							float scale_factor);
int						get_rgba(int r, int g, int b, int a);
void					draw_ray(t_global *global, t_ray *ray);
t_player				*new_player(t_global *global, t_vec2d pos, t_vec2d dir);
t_vec2d					ray_line_collision(t_ray *ray, t_line *line,
							t_face *face);
void					update_position(t_global *global, t_vec2d dir,
							float speed);
void					raycast(t_global *global);
void					resize(int32_t width, int32_t height, void *param);
t_vec2d					get_player_position(t_global *global);
t_vec2d					get_player_direction(t_global *global);
void					cursor(double xpos, double ypos, void *param);
void					show_sky(t_global *global);
void					show_floor(t_global *global);
void					make_background_transparent(t_global *global);
void					render_3d(t_global *global);
void					draw_bar(t_global *global, int x, int y, int width,
							int height, int color);
double					get_current_millis(void);
void					get_opponents(t_global *global);
int						parse_and_validate(char *file, t_global *global);
bool					valid_file(char *file, int flag);
bool					valid_map(char **map, int height);
bool					double_identifier(char *identifier, t_global *global);
int						ft_strcmp(const char *s1, const char *s2);
int						strlen_tab_to_space(char *str);
char					*strdup_tab_to_space(const char *s1, int len);
bool					check_arg_amount(char **split, int amount);
bool					valid_range(int *color);
bool					check_identifier(char *line, t_global *global);
bool					color_format(char **color);
bool					strlen_check(char **split);
int						map_size(char *file, t_global *global);
bool					parse_map(char *line, t_global *global);
bool					parse_line(char *line, t_global *global);
void					get_doors(t_global *global);
float					get_distance(t_vec2d a, t_vec2d b);
void					update_door_segments(t_global *global);
void					put_pixel_if_in_bounds(t_global *global, int x, int y,
							int color);
void					plot_circle_points(t_global *global, t_circle *circle,
							int color, t_vec2d xy);
void					draw_circle_points(t_global *global, t_circle *circle,
							int color, int radius);
t_vec2d					calculate_collision_point(float t, t_vec2d ray_origin,
							t_vec2d ray_direction);
void					determine_face(t_vec2d ray_origin,
							t_vec2d collision_point, t_line *line,
							t_face *face);
bool					is_collision(float t, float u);
t_vec2d					ray_line_intersection(t_ray *ray, t_line *line,
							float *t, float *u);
t_collision				*new_collision(t_collision *collisions,
							int *collision_count, t_collision n_collision);
t_collision				*process_intersections(t_ray *ray, t_line *lines,
							int line_count);
t_collision				*find_closest_collision(t_ray *ray, t_vec2d player_pos);
double					point_line_distance(t_vec2d point, t_line *line);
void					draw_crosshair(t_global *global);
void					ft_texture_to_image(t_global *global,
							mlx_texture_t *texture);
void					my_usleep(unsigned int microseconds);
void					cap_fps(double *elapsed_time, double frame_duration,
							t_global *global);
void					display_fps(int *fps_timer, double elapsed_time);
void					update_images(t_global *global);
void					init_to_null(t_global *global);
void					check_inactive_lines(t_global *global);
float					map_distance_to_height(float distance,
							t_global *global);
void					check_active_lines(t_global *global);
void					detect_collisions(t_global *global, t_ray *tmp_ray);
void					process_ray(t_global *global, int i, float player_angle,
							float *z_buffer);
mlx_texture_t			*load_texture(t_global *global,
							t_collision *closest_collision);
float					calculate_hit_percentage(t_collision *closest_collision);
void					draw_texture_column(t_global *global, t_vec2d position,
							mlx_texture_t *texture, t_vec2d texture_coords);
void					check_close_door(t_global *global);
void					update_door_lines(t_global *global);
void					find_closest_collision_door(t_ray *tmp_ray);
bool					circle_line_collision(t_vec2d circle_center,
							float radius, t_line line);
void					ft_exit_free(t_global *global);
void					loop(void *param);
void					keyHook(void *param);
void					map_to_line_segments(t_global *global, t_line **lines,
							int *line_count);
void					draw_line(t_global *global, t_vec2d a, t_vec2d b,
							int color);
void					draw_line_crosshair(t_global *global, t_vec2d a,
							t_vec2d b, int color);
void					show_map(t_global *global);
float					calculate_scale_factor(int map_width, int map_height,
							int window_width, int window_height);
void					scale_line_segments(t_line *lines, int line_count,
							float scale_factor);
void					draw_circle(t_global *global, t_circle *circle,
							int color);
int						get_rgba(int r, int g, int b, int a);
void					draw_ray(t_global *global, t_ray *ray);
t_player				*new_player(t_global *global, t_vec2d pos, t_vec2d dir);
t_vec2d					ray_line_collision(t_ray *ray, t_line *line,
							t_face *face);
void					update_position(t_global *global, t_vec2d dir,
							float speed);
void					rotate_player(t_global *global, float angle);
void					raycast(t_global *global);
void					resize(int32_t width, int32_t height, void *param);
t_vec2d					get_player_position(t_global *global);
t_vec2d					get_player_direction(t_global *global);
void					cursor(double xpos, double ypos, void *param);
void					show_sky_and_floor(t_global *global);
void					make_background_transparent(t_global *global);
void					render_3d(t_global *global);
double					get_current_millis(void);
void					key_hook(mlx_key_data_t keydata, void *param);
int						parse_and_validate(char *file, t_global *global);
bool					valid_file(char *file, int flag);
bool					valid_map(char **map, int height);
bool					double_identifier(char *identifier, t_global *global);
int						ft_strcmp(const char *s1, const char *s2);
int						strlen_tab_to_space(char *str);
char					*strdup_tab_to_space(const char *s1, int len);
bool					check_arg_amount(char **split, int amount);
bool					valid_range(int *color);
bool					check_identifier(char *line, t_global *global);
bool					color_format(char **color);
bool					strlen_check(char **split);
int						map_size(char *file, t_global *global);
bool					parse_map(char *line, t_global *global);
bool					parse_line(char *line, t_global *global);
void					get_doors(t_global *global);
float					get_distance(t_vec2d a, t_vec2d b);
double					point_line_distance(t_vec2d point, t_line *line);
void					check_open_door(t_global *global);
void					detect_collisions(t_global *global, t_ray *tmp_ray);
void					find_closest_collision_door(t_ray *tmp_ray);
void					map_to_line_segments(t_global *global, t_line **lines,
							int *line_count);
bool					is_wall_above(t_global *global, int x, int y);
bool					is_wall_below(t_global *global, int x, int y);
bool					is_wall_left(t_global *global, int x, int y);
bool					is_wall_right(t_global *global, int x, int y);
void					add_line_segment(t_line **lines, int *count,
							t_segment segment);
bool					is_door_left(t_global *global, int x, int y);
bool					is_door_right(t_global *global, int x, int y);
bool					is_door_above(t_global *global, int x, int y);
bool					is_door_below(t_global *global, int x, int y);
t_segment				set_wall_segment(t_vec2d a, t_vec2d b,
							t_alignment alignment);
t_segment				set_door_side_segment(t_vec2d a, t_vec2d b,
							t_alignment alignment, t_door *door);
t_segment				set_door_segment(t_vec2d a, t_vec2d b,
							t_alignment alignment, t_door *door);
#endif