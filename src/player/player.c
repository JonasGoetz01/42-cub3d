#include "../../inc/cub3d.h"

t_player *new_player(t_global *global, t_vec2d pos, t_vec2d dir) {
    t_player *player;
    float angle_increment;
    float ray_angle;
    int i;

    player = malloc(sizeof(t_player));
    if (!player)
        return (NULL);

    player->pos = pos;
    player->dir = dir;
    
    player->rays = malloc(sizeof(t_ray) * global->img->width);
    if (!player->rays) {
        free(player);
        return (NULL);
    }

    angle_increment = FOV / (global->img->width - 1);
    for (i = 0; i < (int)global->img->width; i++) {
        ray_angle = atan2f(player->dir.y, player->dir.x) - (FOV / 2.0f) + (i * angle_increment);
        player->rays[i].origin = player->pos;
        player->rays[i].direction = (t_vec2d){cosf(ray_angle), sinf(ray_angle)};
        player->rays[i].collisions = NULL;
        player->rays[i].collision_count = 0;
    }
    
    return (player);
}

void update_position(t_global *global, t_vec2d pos) {
    global->player->pos = pos;
    for (int i = 0; i < (int)global->img->width; i++) {
        global->player->rays[i].origin = pos;
    }
}

void rotate_player(t_global *global, float angle) {
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);
    t_vec2d old_dir = global->player->dir;
    t_vec2d new_dir;
    float new_x = old_dir.x * cos_angle - old_dir.y * sin_angle;
    float new_y = old_dir.x * sin_angle + old_dir.y * cos_angle;
    new_dir = (t_vec2d){new_x, new_y};
    global->player->dir = new_dir;

    float angle_increment = FOV / (global->img->width - 1);
    float base_angle = atan2f(new_dir.y, new_dir.x) - (FOV / 2.0f);

    for (int i = 0; i < (int)global->img->width; i++) {
        float ray_angle = base_angle + (i * angle_increment);
        global->player->rays[i].direction = (t_vec2d){cosf(ray_angle), sinf(ray_angle)};
    }
}
