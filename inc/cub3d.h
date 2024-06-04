#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include "../lib/MLX42/include/MLX42/MLX42.h"
#include "point.h"
#include "line.h"
#include "global.h"
#include "color.h"
#include "rect.h"
#include "map.h"

#define WIDTH 1080
#define HEIGHT 720

void    loop(void *param);

#endif