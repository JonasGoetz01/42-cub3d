#include "../../inc/cub3d.h"

#include <sys/time.h>

unsigned long get_current_millis(void) {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
