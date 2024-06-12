#ifndef GEOMETRY_H
# define GEOMETRY_H

typedef struct s_vec2d
{
    float x;
    float y;
} t_vec2d;

typedef struct s_line
{
    t_vec2d a;
    t_vec2d b;
} t_line;

typedef struct s_collision
{
    t_vec2d point;
    t_line *line;
} t_collision;


typedef struct s_ray
{
    t_vec2d origin;
    t_vec2d direction;
    t_collision *collisions;
    int collision_count;
    t_collision *closest_collision;
    t_line *collision_line;
} t_ray;

typedef struct s_circle
{
    t_vec2d center;
    float radius;
} t_circle;

#endif
