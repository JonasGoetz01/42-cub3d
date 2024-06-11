/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/06/11 14:14:48 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

float calculate_scale_factor(int map_width, int map_height, int window_width, int window_height) {
    float scale_x = (float)window_width / (float)map_width;
    float scale_y = (float)window_height / (float)map_height;

    return (scale_x < scale_y) ? scale_x : scale_y;
}

void scale_line_segments(t_line *lines, int line_count, float scale_factor) {
    for (int i = 0; i < line_count; i++) {
        lines[i].a.x *= scale_factor;
        lines[i].a.y *= scale_factor;
        lines[i].b.x *= scale_factor;
        lines[i].b.y *= scale_factor;
    }
}

void draw_line(t_global *global, t_vec2d a, t_vec2d b) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;
    float x = a.x;
    float y = a.y;
    int i;

    for (i = 0; i <= steps; i++) {
        if (x >= 0 && x < global->img->width && y >= 0 && y < global->img->height) {
            mlx_put_pixel(global->img, (int)x, (int)y, get_rgba(255, 255, 255, 255));
        }
        x += x_inc;
        y += y_inc;
    }
}

void draw_circle(t_global *global, t_circle *circle, int color) {
    int radius = (int)circle->radius;
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        mlx_put_pixel(global->img, (int)circle->center.x + x, (int)circle->center.y + y, color);
        mlx_put_pixel(global->img, (int)circle->center.x + y, (int)circle->center.y + x, color);
        mlx_put_pixel(global->img, (int)circle->center.x - y, (int)circle->center.y + x, color);
        mlx_put_pixel(global->img, (int)circle->center.x - x, (int)circle->center.y + y, color);
        mlx_put_pixel(global->img, (int)circle->center.x - x, (int)circle->center.y - y, color);
        mlx_put_pixel(global->img, (int)circle->center.x - y, (int)circle->center.y - x, color);
        mlx_put_pixel(global->img, (int)circle->center.x + y, (int)circle->center.y - x, color);
        mlx_put_pixel(global->img, (int)circle->center.x + x, (int)circle->center.y - y, color);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

//draw a line from the origin of the ray to the border of the image
void draw_ray(t_global *global, t_ray *ray) {
    // Scale the direction vector to a sufficient length (e.g., 1000 units)
    t_vec2d end = {
        ray->origin.x + ray->direction.x * 1000,
        ray->origin.y + ray->direction.y * 1000
    };
    draw_line(global, ray->origin, end);
}
