#include "../../inc/cub3d.h"

t_player *new_player(t_vec2d pos, t_vec2d dir) {
    t_player *player;
    float angle_increment;
    float ray_angle;
    int i;

    player = malloc(sizeof(t_player));
    if (!player)
        return (NULL);

    player->pos = pos;
    player->dir = dir;
    
    player->rays = malloc(sizeof(t_ray) * NUM_RAYS);
    if (!player->rays) {
        free(player);
        return (NULL);
    }

    // Calculate angle increment including the edges of the FOV
    angle_increment = FOV / (NUM_RAYS - 1);
    for (i = 0; i < NUM_RAYS; i++) {
        ray_angle = atan2f(player->dir.y, player->dir.x) - (FOV / 2.0f) + (i * angle_increment);
        player->rays[i].origin = player->pos;
        player->rays[i].direction = (t_vec2d){cosf(ray_angle), sinf(ray_angle)};
    }
    
    return (player);
}

