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

typedef struct s_ray
{
    t_vec2d origin;
    t_vec2d direction;
} t_ray;

typedef struct s_circle
{
    t_vec2d center;
    float radius;
} t_circle;

#endif