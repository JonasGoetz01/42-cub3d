#include "../../inc/cub3d.h"

int get_map_height(char **map)
{
    int i = 0;

    while (map[i])
        i++;
    return (i);
}

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

t_vec2d get_player_position(t_global *global)
{
    t_vec2d player_pos = {0, 0};

    for (int y = 0; y < global->map->height; y++)
    {
        for (int x = 0; x < global->map->width; x++)
        {
            if (global->map->map[y][x] == 'N' || global->map->map[y][x] == 'E' || global->map->map[y][x] == 'W' || global->map->map[y][x] == 'S')
            {
                player_pos.x = x;
                player_pos.y = y;
                return (player_pos);
            }
        }
    }
    return (player_pos);
}

void    initMap(t_global *global)
{
    t_line *lines;
    int line_count;

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
        "10000000N00000001101010010001    ",
        "1100000111010101111101111000111  ",
        "11110111 1110101 101111010001    ",
        "11111111 1111111 111111111111    ",
        NULL
    };
    // index y 11 x 26 (player)

    global->map->map = malloc(sizeof(char *) * 15);
    for (int i = 0; i < 14; i++)
    {
        global->map->map[i] = ft_strdup(map[i]);
    }
    global->map->map[14] = NULL;
    global->map->height = get_map_height(global->map->map);
    global->map->width = get_map_width(global->map->map);

    map_to_line_segments(global, &lines, &line_count);
    global->scale_factor = calculate_scale_factor(global->map->width, global->map->height, WIDTH * global->minimap_scale, HEIGHT * global->minimap_scale);
    scale_line_segments(lines, line_count, global->scale_factor);
    global->line_count = line_count;
    global->lines = lines;
}
