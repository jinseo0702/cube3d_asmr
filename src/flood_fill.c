#include "../include/cub3d.h"

// int cnt = 0;

int flud_fill(char **map)
{
    int status;
    int x;
    int y;

    status = 0;
    x = 0;
    while (map[x])
    {
        if (status == -1)
            return (-1);
        y = 0;
        while (map[x][y])
        {
            if (map[x][y] == '0')
                solve_Dfs(map, x, y, &status);
            ++y;
        }
        ++x;
    }
    return (1);
}

void solve_Dfs(char **map, int x, int y, int *status)
{
    if (*status == -1)
        return ;
    if (map[x][y] == 'X')
    {
        *status = -1;
        return ;
    }
    else if(map[x][y] != '0')
        return ;
    map[x][y] += 2;
    solve_Dfs(map, (x - 1), y, status);//상
    solve_Dfs(map, (x + 1), y, status);//하
    solve_Dfs(map, x, (y - 1), status);//좌
    solve_Dfs(map, x, (y + 1), status);//우
}