#include "../../inc/cub3d.h"

bool isWallAbove(t_global *global, int x, int y)
{
    if (y == 0)
        return (false);
    if (global->map->map[y - 1][x] == '1')
        return (true);
    return (false);
}

bool isWallBelow(t_global *global, int x, int y)
{
    if (y == global->map->height - 1)
        return (false);
    if (global->map->map[y + 1][x] == '1')
        return (true);
    return (false);
}

bool isWallLeft(t_global *global, int x, int y)
{
    if (x == 0)
        return (false);
    if (global->map->map[y][x - 1] == '1')
        return (true);
    return (false);
}

bool isWallRight(t_global *global, int x, int y)
{
    if (x == global->map->width - 1)
        return (false);
    if (global->map->map[y][x + 1] == '1')
        return (true);
    return (false);
}

void add_line_segment(t_line **lines, int *count, t_vec2d a, t_vec2d b) {
    *lines = realloc(*lines, (*count + 1) * sizeof(t_line));
    (*lines)[*count].a = a;
    (*lines)[*count].b = b;
    (*count)++;
}

void map_to_line_segments(t_global *global, t_line **lines, int *line_count) {
    int x, y;
    *lines = NULL;
    *line_count = 0;

    for (y = 0; y < global->map->height; y++) {
        for (x = 0; x < global->map->width; x++) {
            if (global->map->map[y][x] == '1') {
                t_vec2d top_left = {x, y};
                t_vec2d top_right = {x + 1, y};
                t_vec2d bottom_left = {x, y + 1};
                t_vec2d bottom_right = {x + 1, y + 1};

                if (!isWallAbove(global, x, y)) {
                    add_line_segment(lines, line_count, top_left, top_right);
                }
                if (!isWallBelow(global, x, y)) {
                    add_line_segment(lines, line_count, bottom_left, bottom_right);
                }
                if (!isWallLeft(global, x, y)) {
                    add_line_segment(lines, line_count, top_left, bottom_left);
                }
                if (!isWallRight(global, x, y)) {
                    add_line_segment(lines, line_count, top_right, bottom_right);
                }
            }
        }
    }
}

void showMap(t_global *global)
{
    int i;

    i = 0;
    while (i < global->line_count) {
        draw_line(global, global->lines[i].a, global->lines[i].b);
        i++;
    }
    draw_circle(global, &(t_circle){global->player->pos, 5}, get_rgba(255, 0, 0, 255));
    for (i = 0; i < NUM_RAYS; i++) {
        if (global->player->rays[i].collisions)
            free(global->player->rays[i].collisions);
        global->player->rays[i].collisions = NULL;
        global->player->rays[i].collision_count = 0;
    }
    raycast(global);
    for (int i = 0; i < NUM_RAYS; i++)
    {
        if(SHOW_RAYS)
            draw_ray(global, &global->player->rays[i]);
        for (int j = 0; j < global->player->rays[i].collision_count; j++)
        {
            if(SHOW_COLLISIONS)
                draw_circle(global, &(t_circle){global->player->rays[i].collisions[j], 3}, get_rgba(0, 255, 0, 255));
        }
        if(SHOW_COLLISIONS)
            draw_circle(global, &(t_circle){*(global->player->rays[i].closest_collision), 3}, get_rgba(0, 0, 255, 255));
    }   
}
