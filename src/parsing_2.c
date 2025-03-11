#include "../include/cub3d.h"

int parse_color(char *line)
{
    int r, g, b;
    char **parts;
    char **rgb;

    // "F 220,100,0" 형식에서 먼저 공백으로 분리
    parts = ft_split(line, ' ');
    if (!parts || ft_arraylen(parts) < 2) // "F"와 "220,100,0" 부분으로 나뉨
    {
        if (parts)
            ft_free_array(parts);
        return (0x000000); // 오류 시 기본 검은색
    }

    // "220,100,0" 부분을 쉼표로 분리
    rgb = ft_split(parts[1], ',');
    ft_free_array(parts);

    if (!rgb || ft_arraylen(rgb) != 3) // r, g, b 값 확인
    {
        if (rgb)
            ft_free_array(rgb);
        return (0x000000);
    }

    r = ft_atoi(rgb[0]);
    g = ft_atoi(rgb[1]);
    b = ft_atoi(rgb[2]);

    ft_free_array(rgb);

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return (0x000000);

    // RGB 색상값 생성
    return ((r << 16) | (g << 8) | b);
}


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
        map_data->F = parse_color(str); // 수정사항 2025.3.11
    else if (ft_strncmp(str, "C ", 2) == 0)
        map_data->C = parse_color(str); // 수정사항 2025.3.11
    else if (ft_onlyisspace(str) && map_data->exf > 63)
        return (-1);
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
    if (ft_onlyisspace(str))
    {
        printf("error\n");
        return (-1); //error 처리 개항만 있을때
    }
    while (str[++idx] && str[idx] != '\n')
    {
        if (ft_isinstr(str[idx], "01NSWE ") == 0)
            return (-1); // error처리하기 맵에이상한 숫자가 껴있는경우
    }
    // map_data->map[map_data->map_height] = ft_strdup_flag(str, &status);
    map_data->map[map_data->map_height] = ft_calloc(1, map_data->map_width + 2);
    ft_memset(map_data->map[map_data->map_height], 'X', map_data->map_width + 1);
    ft_strlcpy(&map_data->map[map_data->map_height][1], str, ft_strlen(str) + 2);
    idx = -1;
    while (map_data->map[map_data->map_height][++idx])
        if(ft_isspace(map_data->map[map_data->map_height][idx]))
            map_data->map[map_data->map_height][idx] = 'X';
    printf("%s\n", map_data->map[map_data->map_height]);
    if (ft_strlen(str) < map_data->map_width)
        map_data->map[map_data->map_height][ft_strlen(str) + 1] = 'X';
    else
        map_data->map[map_data->map_height][map_data->map_width] = 'X';
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
    map_data->map = (char **)ft_calloc(sizeof(char *), (map_data->high - 4));
    map_data->map[0] = (char *)ft_calloc(sizeof(char), map_data->map_width + 2);
    ft_memset(map_data->map[0], 'X', map_data->map_width + 1);
    if (map_data->map == NULL)
        return (0);//error enum만들기
	while ((temp = get_next_line(map_data->fd)))
	{
        if(dup_info(temp, map_data) == 1 && map_data->exf^63)
            map_data->exf = (map_data->exf >> 1);
        printf("%d %s\n", map_data->exf, temp);
        ft_freenull(&temp);
	}
    map_data->map[map_data->map_height] = ft_calloc(sizeof(char), map_data->map_width + 2);
    ft_memset(map_data->map[map_data->map_height], 'X', map_data->map_width + 1);
    flud_fill(map_data->map);
    // print_all(map_data);
	close(map_data->fd);
    return (1);
}

void print_all(t_map *map)
{
    printf("fd == %d, high == %d, width == %d, m_high == %d \n", map->fd, map->high, map->map_width, map->map_height);
    printf("NO is %s ", map->NO);
    printf("SO is %s ", map->SO);
    printf("WE is %s ", map->WE);
    printf("EA is %s ", map->EA);
    // printf("F is %s ", map->F);
    // printf("C is %s ", map->C);
    printf("print map\n");
    int idx = 0;
    while (map->map[idx])
    {
        printf("%s", map->map[idx]);
        printf("\n");
        idx++;
    }
}