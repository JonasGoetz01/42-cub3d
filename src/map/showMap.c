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

void add_line_segment(t_line **lines, int *count, t_vec2d a, t_vec2d b, t_alignment alignment) {
    *lines = realloc(*lines, (*count + 1) * sizeof(t_line));
    (*lines)[*count].a = a;
    (*lines)[*count].b = b;
    (*lines)[*count].alignment = alignment;
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
                    add_line_segment(lines, line_count, top_left, top_right, HORIZONTAL);
                }
                if (!isWallBelow(global, x, y)) {
                    add_line_segment(lines, line_count, bottom_left, bottom_right, HORIZONTAL);
                }
                if (!isWallLeft(global, x, y)) {
                    add_line_segment(lines, line_count, top_left, bottom_left, VERTICAL);
                }
                if (!isWallRight(global, x, y)) {
                    add_line_segment(lines, line_count, top_right, bottom_right, VERTICAL);
                }
            }
        }
    }
}

void draw_fov_lines(t_global *global)
{
    float angle;
    t_vec2d start, left_end, right_end;
    t_player *player;

    player = global->player;
    start = player->pos;
    angle = atan2(player->dir.y, player->dir.x) - (FOV / 2);
    left_end.x = start.x + cos(angle) * 10;
    left_end.y = start.y + sin(angle) * 10;
    angle = atan2(player->dir.y, player->dir.x) + (FOV / 2);
    right_end.x = start.x + cos(angle) * 10;
    right_end.y = start.y + sin(angle) * 10;
    draw_line(global, start, left_end, get_rgba(255, 255, 255, 255)); // white color
    draw_line(global, start, right_end, get_rgba(255, 255, 255, 255)); // white color
}


void showMap(t_global *global)
{
    int i;

    i = 0;
    while (i < global->line_count) {
        draw_line(global, global->lines[i].a, global->lines[i].b, get_rgba(255, 255, 255, 255));
        i++;
    }
    draw_circle(global, &(t_circle){global->player->pos, 5}, get_rgba(255, 0, 0, 255));
    for (i = 0; i < (int)global->img->width; i++) {
        if (global->player->rays[i].collisions)
        {
            free(global->player->rays[i].collisions);
        }
        global->player->rays[i].collisions = NULL;
        global->player->rays[i].collision_count = 0;
    }
    raycast(global);
    for (uint32_t i = 0; i < global->img->width; i++)
    {
        if(SHOW_RAYS)
            draw_ray(global, &global->player->rays[i]);
        for (int j = 0; j < global->player->rays[i].collision_count; j++)
        {
            if(SHOW_COLLISIONS)
                draw_circle(global, &(t_circle){global->player->rays[i].collisions[j].point, 3}, get_rgba(0, 255, 0, 255));
        }
        if(SHOW_COLLISIONS)
            draw_circle(global, &(t_circle){global->player->rays[i].closest_collision->point, 3}, get_rgba(0, 0, 255, 255));
    }
    for (int i = 0; i < global->opponent_count; i++)
    {
        draw_ray(global, &global->player->opponent_rays[i]);
        if(global->player->opponent_rays[i].closest_collision)
            draw_circle(global, &(t_circle){global->player->opponent_rays[i].closest_collision->point, 2}, get_rgba(255, 0, 0, 255));
        draw_circle(global, &(t_circle){global->opponent[i].pos, 5}, get_rgba(255, 255, 0, 255));
    }
    if(SHOW_FOV)
        draw_fov_lines(global);
}
