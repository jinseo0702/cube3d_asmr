#include "../include/cub3d.h"

int dup_info(char *str, t_map *map_data)
{
    int status;

    status = 0;
    if (ft_strncmp(str, "NO ", 3) == 0)
        map_data->NO = ft_strdup_flag(str, &status);
    else if (ft_strncmp(str, "SO ", 3) == 0)
        map_data->SO = ft_strdup_flag(str, &status);
    else if (ft_strncmp(str, "WE ", 3) == 0)
        map_data->WE = ft_strdup_flag(str, &status);
    else if (ft_strncmp(str, "EA ", 3) == 0)
        map_data->EA = ft_strdup_flag(str, &status);
    else if (ft_strncmp(str, "F ", 2) == 0)
        map_data->F = ft_strdup_flag(str, &status);
    else if (ft_strncmp(str, "C ", 2) == 0)
        map_data->C = ft_strdup_flag(str, &status);
    else
        status = is_right_map(str, map_data);
    if (status == -1)
        return (-1);
    else
        return (1);//enum으로 통제하기
}

int is_right_map(char *str, t_map *map_data)
{
    int status;
    int idx;

    status = 0;
    idx = -1;
    if (*str == '\n')
        return (-1); //error 처리 개항만 있을때
    while (str[++idx] && str[idx] != '\n')
    {
        if (ft_isinstr(str[idx], "01NSWE ") == 0)
            return (-1); // error처리하기 맵에이상한 숫자가 껴있는경우
    }
    map_data->map[map_data->map_height] = ft_strdup_flag(str, &status);
    map_data->map_height++;
    if(status == -1)
        return (-1); // error 처리하기
    return (1);
}

int insert_data(t_map *map_data, char *map)
{
	char *temp;

    map_data->fd = open(map, O_RDONLY);
    if (map_data->fd < 0)
    {
        //error 함수 만들기
        return (0);
    }
    map_data->map = ft_calloc(sizeof(char *), (map_data->high - 5));
    if (map_data->map == NULL)
        return (0);//error enum만들기
	while ((temp = get_next_line(map_data->fd)))
	{
        dup_info(temp, map_data);
        ft_freenull(&temp);
	}
	close(map_data->fd);
    return (1);
}

void print_all(t_map *map)
{
    printf("fd == %d, high == %d, width == %d, m_high == %d \n", map->fd, map->high, map->high, map->map_height);
    printf("NO is %s ", map->NO);
    printf("SO is %s ", map->SO);
    printf("WE is %s ", map->WE);
    printf("EA is %s ", map->EA);
    printf("F is %s ", map->F);
    printf("C is %s ", map->C);
    printf("print map\n");
    int idx = 0;
    while (map->map[idx])
    {
        printf("%s", map->map[idx]);
        idx++;
    }
}