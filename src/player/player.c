#include "../../inc/cub3d.h"

t_player *new_player(t_vec2d pos, t_vec2d dir)
{
    t_player *player;

    player = malloc(sizeof(t_player));
    if (!player)
        return (NULL);

    player->pos = pos;
    player->dir = dir;
    
    player->rays = malloc(sizeof(t_ray) * NUM_RAYS);
    if (!player->rays)
    {
        free(player);
        return (NULL);
    }

    // for (i = 0; i < NUM_RAYS; i++)
    // {
    //     player->rays[i] = new_ray(i * 360 / NUM_RAYS);
    // }
    
    return (player);
}
