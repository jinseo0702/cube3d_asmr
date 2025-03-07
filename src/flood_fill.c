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

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// int main(void)
// {
// char original_map[16][36] = {
//     "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
//     "XXXX0XXXX1111111111111111111111111X",
//     "XXXXXXXXX1000000000110000000000001X",
//     "XXXXXXXXX1011000001110000001000001X",
//     "XXXXXXXXX1001000000000000000000001X",
//     "X111111111011000001110000000000001X",
//     "X100000000011000001110111111111111X",
//     "X11110111111111011100000010001XXXXX",
//     "X11110111111111011101010010001XXXXX",
//     "X11000000110101011100000010001XXXXX",
//     "X10001000011100001100000010001XXXXX",
//     "X10000000000000001101010010001XXXXX",
//     "X11000001110101011111011110N0111XXX",
//     "X11110111X1110101X101111010001XXXXX",
//     "X11111111X1111111X111111111111XXXXX",
//     "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
// };

// // 메모리 할당
// char **map_ptr = (char **)malloc(16 * sizeof(char *));
// for (int i = 0; i < 16; i++) {
//     map_ptr[i] = (char *)malloc(36 * sizeof(char));
//     strcpy(map_ptr[i], original_map[i]);  // 각 행 복사
// }

//     int what = flud_fill(map_ptr);
//     printf ("%d %d\n", what, cnt);
//     for (size_t i = 0; i < 16; i++)
//     {
//         printf("%s\n", map_ptr[i]);
//     }
    
//     return 0;
// }