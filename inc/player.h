#ifndef PLAYER_H
#define PLAYER_H

#include "point.h"
#include "ray.h"

typedef struct s_player {
    t_point pos;
    t_point dir;
    t_ray **rays;
} t_player;

t_player *new_player(t_point pos, t_point dir);

#endif