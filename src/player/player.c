#include "../../inc/cub3d.h"

t_player *new_player(t_point pos, t_point dir)
{
    t_player *player;
    int i;

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

    for (i = 0; i < NUM_RAYS; i++)
    {
        player->rays[i] = new_ray(player->pos, i * 360 / NUM_RAYS);
    }
    
    return (player);
}
