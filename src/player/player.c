#include "../../inc/cub3d.h"

t_player *new_player(t_global *global, t_vec2d pos, t_vec2d dir) {
    t_player *player;
    float angle_increment;
    float ray_angle;
    int i;

    player = malloc(sizeof(t_player));
    if (!player)
        return (NULL);

    player->pos = pos;
    player->dir = dir;
    
    player->rays = malloc(sizeof(t_ray) * global->img->width);
    if (!player->rays) {
        free(player);
        return (NULL);
    }

    angle_increment = FOV / (global->img->width - 1);
    for (i = 0; i < (int)global->img->width; i++) {
        ray_angle = atan2f(player->dir.y, player->dir.x) - (FOV / 2.0f) + (i * angle_increment);
        player->rays[i].origin = player->pos;
        player->rays[i].direction = (t_vec2d){cosf(ray_angle), sinf(ray_angle)};
        player->rays[i].collisions = NULL;
        player->rays[i].collision_count = 0;
    }
    
    return (player);
}

t_vec2d calculate_perpendicular_direction(t_line dir_line, t_line wall) {
    t_vec2d wall_dir;
    t_vec2d perpendicular;
    t_vec2d dir_vector;
    t_vec2d new_dir;
    float dot_product;
    float length;

    wall_dir.x = wall.b.x - wall.a.x;
    wall_dir.y = wall.b.y - wall.a.y;
    perpendicular.x = -wall_dir.y;
    perpendicular.y = wall_dir.x;
    length = sqrt(perpendicular.x * perpendicular.x + perpendicular.y * perpendicular.y);
    perpendicular.x /= length;
    perpendicular.y /= length;
    dir_vector.x = dir_line.b.x - dir_line.a.x;
    dir_vector.y = dir_line.b.y - dir_line.a.y;
    dot_product = dir_vector.x * perpendicular.x + dir_vector.y * perpendicular.y;
    new_dir.x = dot_product * perpendicular.x;
    new_dir.y = dot_product * perpendicular.y;
    return new_dir;
}

bool line_line_collision(t_line *a, t_line *b) {
    float x1 = a->a.x;
    float y1 = a->a.y;
    float x2 = a->b.x;
    float y2 = a->b.y;
    float x3 = b->a.x;
    float y3 = b->a.y;
    float x4 = b->b.x;
    float y4 = b->b.y;

    float denominator = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    if (denominator == 0) {
        return false;  // Lines are parallel
    }

    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;

    return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
}

void update_position(t_global *global, t_vec2d dir) {
    t_vec2d new_pos;
    t_vec2d new_dir = dir;  // Start with the initial direction
    bool collision = false;

    // Calculate the initial new position
    new_pos.x = global->player->pos.x + dir.x * MOVE_SPEED * global->minimap_scale;
    new_pos.y = global->player->pos.y + dir.y * MOVE_SPEED * global->minimap_scale;

    // Check for collisions with each line
    for (int i = 0; i < global->line_count; i++) {
        t_line temp_line = {global->player->pos, new_pos, global->lines[i].alignment};
        if (line_line_collision(&global->lines[i], &temp_line)) {
            new_dir = calculate_perpendicular_direction(temp_line, global->lines[i]);
            new_pos.x = global->player->pos.x + new_dir.x * MOVE_SPEED * global->minimap_scale;
            new_pos.y = global->player->pos.y + new_dir.y * MOVE_SPEED * global->minimap_scale;
            collision = true;
            break;
        }
    }

    // If no collision, use the initially calculated new position
    if (!collision) {
        new_pos.x = global->player->pos.x + dir.x * MOVE_SPEED * global->minimap_scale;
        new_pos.y = global->player->pos.y + dir.y * MOVE_SPEED * global->minimap_scale;
    }

    // Ensure new position is valid (not colliding)
    for (int i = 0; i < global->line_count; i++) {
        t_line temp_line = {global->player->pos, new_pos, global->lines[i].alignment};
        if (line_line_collision(&global->lines[i], &temp_line)) {
            return;  // If still colliding, don't update position
        }
    }

    // Update player position
    global->player->pos = new_pos;

    // Update the origin of each ray
    for (int i = 0; i < (int)global->img->width; i++) {
        global->player->rays[i].origin = new_pos;
    }
}

void rotate_player(t_global *global, float angle) {
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);
    t_vec2d old_dir = global->player->dir;
    t_vec2d new_dir;
    float new_x = old_dir.x * cos_angle - old_dir.y * sin_angle;
    float new_y = old_dir.x * sin_angle + old_dir.y * cos_angle;
    new_dir = (t_vec2d){new_x, new_y};
    global->player->dir = new_dir;

    float angle_increment = FOV / (global->img->width - 1);
    float base_angle = atan2f(new_dir.y, new_dir.x) - (FOV / 2.0f);

    for (int i = 0; i < (int)global->img->width; i++) {
        float ray_angle = base_angle + (i * angle_increment);
        global->player->rays[i].direction = (t_vec2d){cosf(ray_angle), sinf(ray_angle)};
    }
}
