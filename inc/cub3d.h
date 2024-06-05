#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft.h"
#include "point.h"
#include "line.h"
#include "global.h"
#include "color.h"
#include "rect.h"
#include "map.h"
#include "hooks.h"
#include "circle.h"
#include "ray.h"
#include "player.h"

#define WIDTH 1080
#define HEIGHT 720
#define NUM_RAYS 30
#define FOV 60

void    loop(void *param);

#endif