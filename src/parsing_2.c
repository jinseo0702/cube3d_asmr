#include "../include/cub3d.h"

int dup_info(char *str, t_map *map_data)
{
    char *temp;
    if (ft_strncmp(str, "NO ", 3) == 0)
        map_data->NO = ft_strdup(str);
    else if (ft_strncmp(str, "SO ", 3) == 0)
        map_data->SO = ft_strdup(str);
    else if (ft_strncmp(str, "WE ", 3) == 0)
        map_data->WE = ft_strdup(str);
    else if (ft_strncmp(str, "EA ", 3) == 0)
        map_data->EA = ft_strdup(str);
    else if (ft_strncmp(str, "F ", 2) == 0)
        map_data->F = ft_strdup(str);
    else if (ft_strncmp(str, "C ", 2) == 0)
        map_data->C = ft_strdup(str);
}
