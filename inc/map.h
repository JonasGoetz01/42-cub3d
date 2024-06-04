#ifndef MAP_H
#define MAP_H

#include "global.h"

typedef struct s_map
{
    char    **map;
    int     width;
    int     height;
}   t_map;

void showMap(t_global *global);
void    initMap(t_global *global);

#endif