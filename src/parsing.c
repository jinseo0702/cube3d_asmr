#include "../include/cub3d.h"

void init_t_map(t_map *map_data)
{
    map_data->fd = 0;
    map_data->high = 0;
    map_data->map_height = 1;
    map_data->map_width = 0;
    map_data->exf = 0;
    map_data->NO = NULL;
    map_data->SO = NULL;
    map_data->WE = NULL;
    map_data->EA = NULL;
    map_data->F = NULL;
    map_data->C = NULL;
    map_data->map = NULL;
}

int map_parsing(char *map, t_data *data)
{
    init_t_map(&data->map);
    if (ft_strrstr(map, ".cub", 4) == 0)
    {
        //error 함수 만들기
        return (0);
    }
    data->map.fd = open(map, O_RDONLY);
    if (data->map.fd < 0)
    {
        //error 함수 만들기
        return (0);
    }
    if (check_size(&data->map) == 0)
        return (0);
    // printf("hight = %d, width = %d\n", .high, map_data.map_width);
    insert_data(&data->map, map);
    return (1);
}

int check_arg(char *str)
{
    int idx;

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
    {
        idx = -1;
        while (str[++idx])
            if (!ft_isspace(str[idx]))
                return (0);
        return (64);
    }
}

int check_size(t_map *map_data)
{
    // static int check;
    int cnt;
	char *temp;

    cnt = 6;
	while ((temp = get_next_line(map_data->fd)))
	{
		if (map_data->high == 0)
            map_data->map_width = ft_strlen(temp);
        if (ft_strlen(temp) > map_data->map_width)
            map_data->map_width = ft_strlen(temp);
        map_data->exf |= check_arg(temp);
        if (check_arg(temp) && check_arg(temp) != 64)
            map_data->exf |= (1 << (++cnt));
        if (map_data->exf < 8191 && (check_arg(temp) == 0))
            printf("error \n");//status로 바꾸장
		ft_freenull(&temp);
		map_data->high++;
	}
	close(map_data->fd);
    printf("%d\n", map_data->exf);
    return (map_data->exf == 8191);
}
// int main(int argc, char **argv)
// {
//     map_parsing(argv[1]);
//     return (0);
// }