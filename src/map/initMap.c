#include "../../inc/cub3d.h"

int get_map_height(char **map)
{
    int i = 0;

    while (map[i])
        i++;
    return (i);
}

// get the width of the map (not all the rows have the same length)
int get_map_width(char **map)
{
    int i = 0;
    int j = 0;
    int max = 0;

    while (map[i])
    {
        j = 0;
        while (map[i][j])
            j++;
        if (j > max)
            max = j;
        i++;
    }
    return (max);
}

void    initMap(t_global *global)
{
    char *map[] = {
        "        1111111111111111111111111",
        "        1000000000110000000000001",
        "        1011000001110000000000001",
        "        1001000000000000000000001",
        "111111111011000001110000000000001",
        "100000000011000001110111111111111",
        "11110111111111011100000010001    ",
        "11110111111111011101010010001    ",
        "11000000110101011100000010001    ",
        "10000000000000001100000010001    ",
        "10000000000000001101010010001    ",
        "11000001110101011111011110N0111  ",
        "11110111 1110101 101111010001    ",
        "11111111 1111111 111111111111    ",
        NULL
    };

    global->map->map = malloc(sizeof(char *) * 15);
    for (int i = 0; i < 14; i++)
    {
        global->map->map[i] = ft_strdup(map[i]);
    }
    global->map->map[14] = NULL;
    global->map->height = get_map_height(global->map->map);
    global->map->width = get_map_width(global->map->map);
    printf("height: %d\n", global->map->height);
    printf("width: %d\n", global->map->width);
}