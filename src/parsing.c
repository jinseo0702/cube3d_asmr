#include "../include/cub3d.h"

void init_t_map(t_map *map_data)
{
    map_data->fd = 0;
    map_data->high = 0;
    map_data->width = 0;
    map_data->m_high = 0;
    map_data->NO = NULL;
    map_data->SO = NULL;
    map_data->WE = NULL;
    map_data->EA = NULL;
    map_data->F = NULL;
    map_data->C = NULL;
    map_data->map = NULL;
}

int map_parsing(char *map)
{
    t_map map_data;

    if (ft_strrstr(map, ".cub", 4) == 0)
    {
        //error 함수 만들기
        return (0);
    }
    map_data.fd = open(map, O_RDONLY);
    if (map_data.fd < 0)
    {
        //error 함수 만들기
        return (0);
    }
    if (check_size(&map_data) == 0)
        return (0);
    printf("hight = %d, width = %d\n", map_data.high, map_data.width);
    insert_data(&map_data, map);
    print_all(&map_data);
    return (1);
}

int check_arg(char *str)
{
    if (ft_strncmp(str, "NO ", 3) == 0)
        return (1);
    else if (ft_strncmp(str, "SO ", 3) == 0)
        return (2);
    else if (ft_strncmp(str, "WE ", 3) == 0)
        return (4);
    else if (ft_strncmp(str, "EA ", 3) == 0)
        return (8);
    else if (ft_strncmp(str, "F ", 2) == 0)
        return (16);
    else if (ft_strncmp(str, "C ", 2) == 0)
        return (32);
    else
        return (0);
}

int check_size(t_map *map_data)
{
    static int check;
    static int cnt;
	char *temp;

	while ((temp = get_next_line(map_data->fd)))
	{
		if (map_data->high == 0)
            map_data->width = ft_strlen(temp);
        if (ft_strlen(temp) > map_data->width)
            map_data->width = ft_strlen(temp);
        check |= check_arg(temp);
        if (check_arg(temp))
            cnt++;
		ft_freenull(&temp);
		map_data->high++;
	}
	close(map_data->fd);
    return (check == 63 && cnt == 6);
}
int main(int argc, char **argv)
{
    map_parsing(argv[1]);
    return (0);
}