/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/06/11 23:18:00 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

float calculate_scale_factor(int map_width, int map_height, int window_width, int window_height) {
    float scale_x = ((float)window_width - 10) / (float)map_width;
    float scale_y = ((float)window_height - 10) / (float)map_height;

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
        if (x >= 0 && x < global->minimap->width && y >= 0 && y < global->minimap->height) {
            mlx_put_pixel(global->minimap, (int)x, (int)y, get_rgba(255, 255, 255, 255));
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
    int img_width = (int)global->window_width;
    int img_height = (int)global->window_height;

    while (x >= y) {
        if ((int)circle->center.x + x >= 0 && (int)circle->center.x + x < img_width &&
            (int)circle->center.y + y >= 0 && (int)circle->center.y + y < img_height)
            mlx_put_pixel(global->minimap, (int)circle->center.x + x, (int)circle->center.y + y, color);

        if ((int)circle->center.x + y >= 0 && (int)circle->center.x + y < img_width &&
            (int)circle->center.y + x >= 0 && (int)circle->center.y + x < img_height)
            mlx_put_pixel(global->minimap, (int)circle->center.x + y, (int)circle->center.y + x, color);

        if ((int)circle->center.x - y >= 0 && (int)circle->center.x - y < img_width &&
            (int)circle->center.y + x >= 0 && (int)circle->center.y + x < img_height)
            mlx_put_pixel(global->minimap, (int)circle->center.x - y, (int)circle->center.y + x, color);

        if ((int)circle->center.x - x >= 0 && (int)circle->center.x - x < img_width &&
            (int)circle->center.y + y >= 0 && (int)circle->center.y + y < img_height)
            mlx_put_pixel(global->minimap, (int)circle->center.x - x, (int)circle->center.y + y, color);

        if ((int)circle->center.x - x >= 0 && (int)circle->center.x - x < img_width &&
            (int)circle->center.y - y >= 0 && (int)circle->center.y - y < img_height)
            mlx_put_pixel(global->minimap, (int)circle->center.x - x, (int)circle->center.y - y, color);

        if ((int)circle->center.x - y >= 0 && (int)circle->center.x - y < img_width &&
            (int)circle->center.y - x >= 0 && (int)circle->center.y - x < img_height)
            mlx_put_pixel(global->minimap, (int)circle->center.x - y, (int)circle->center.y - x, color);

        if ((int)circle->center.x + y >= 0 && (int)circle->center.x + y < img_width &&
            (int)circle->center.y - x >= 0 && (int)circle->center.y - x < img_height)
            mlx_put_pixel(global->minimap, (int)circle->center.x + y, (int)circle->center.y - x, color);

        if ((int)circle->center.x + x >= 0 && (int)circle->center.x + x < img_width &&
            (int)circle->center.y - y >= 0 && (int)circle->center.y - y < img_height)
            mlx_put_pixel(global->minimap, (int)circle->center.x + x, (int)circle->center.y - y, color);

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

void draw_ray(t_global *global, t_ray *ray) {
    t_vec2d end = {
        ray->origin.x + ray->direction.x * 1000,
        ray->origin.y + ray->direction.y * 1000
    };
    draw_line(global, ray->origin, end);
}

t_vec2d ray_line_collision(t_ray *ray, t_line *line)
{
    float x1 = ray->origin.x;
    float y1 = ray->origin.y;
    float x2 = ray->origin.x + ray->direction.x;
    float y2 = ray->origin.y + ray->direction.y;
    float x3 = line->a.x;
    float y3 = line->a.y;
    float x4 = line->b.x;
    float y4 = line->b.y;

    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denominator == 0)
        return ((t_vec2d){-1, -1});

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
    float u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denominator;
    
    if (t >= 0 && u >= 0 && u <= 1)
        return ((t_vec2d){x1 + t * (x2 - x1), y1 + t * (y2 - y1)});
    return ((t_vec2d){-1, -1});
}

t_vec2d* new_collision(t_vec2d *collisions, int *collision_count, t_vec2d collision) {
    t_vec2d *new_collisions;

    new_collisions = realloc(collisions, (*collision_count + 1) * sizeof(t_vec2d));
    if (!new_collisions)
        return NULL;
    new_collisions[*collision_count] = collision;
    (*collision_count)++;
    return new_collisions;
}


void raycast(t_global *global)
{
    for (int i = 0; i < NUM_RAYS; i++)
    {
        for (int j = 0; j < global->line_count; j++)
        {
            t_vec2d intersection;
            intersection = ray_line_collision(&global->player->rays[i], &global->lines[j]);
            if (intersection.x != -1)
            {
                t_vec2d *new_collisions = new_collision(global->player->rays[i].collisions, &global->player->rays[i].collision_count, intersection);
                if (!new_collisions)
                    return;
                global->player->rays[i].collisions = new_collisions;
            }
        }
        float min_distance = 1000000;
        for (int j = 0; j < global->player->rays[i].collision_count; j++)
        {
            float distance = sqrtf(powf(global->player->pos.x - global->player->rays[i].collisions[j].x, 2) + powf(global->player->pos.y - global->player->rays[i].collisions[j].y, 2));
            if (distance < min_distance)
            {
                min_distance = distance;
                global->player->rays[i].closest_collision = &global->player->rays[i].collisions[j];
            }
        }
    }
}

//draw a bar from the center of the screen "width" pixels wide and "height" pixels tall
void draw_bar(t_global *global, int x, int y, int width, int height, int color)
{
    for (int i = 0; i < width; i++)
    {
        if (x + i < 0 || (uint32_t)x + (uint32_t)i >= global->img->width)
            continue;
        for (int j = 0; j < height; j++)
        {
            int draw_y = y + j;
            if (draw_y >= 0 && (uint32_t)draw_y < global->img->height)
            {
                mlx_put_pixel(global->img, x + i, draw_y, color);
            }
        }
    }
}
