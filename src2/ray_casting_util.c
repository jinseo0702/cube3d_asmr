#include "../include/cub3d.h"

void calculate_step_side_dist_x(t_ray *ray, t_data *game)
{
    if (ray->dir_x < 0)
    {
        ray->step_x = -1;
        ray->side_dist_x = (game->cor.x - ray->map_x) * ray->delta_dist_x;
    }
    else
    {
        ray->step_x = 1;
        ray->side_dist_x = (ray->map_x + 1.0 - game->cor.x) * ray->delta_dist_x;
    }
}

void calculate_step_side_dist_y(t_ray *ray, t_data *game)
{
    if (ray->dir_y < 0)
    {
        ray->step_y = -1;
        ray->side_dist_y = (game->cor.y - ray->map_y) * ray->delta_dist_y;
    }
    else
    {
        ray->step_y = 1;
        ray->side_dist_y = (ray->map_y + 1.0 - game->cor.y) * ray->delta_dist_y;
    }
}