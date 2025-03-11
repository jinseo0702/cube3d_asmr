#include "../include/cub3d.h"


int ft_arraylen(char **array)
{
    int i = 0;
    while (array[i])
        i++;
    return (i);
}

void ft_free_array(char **array)
{
    int i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
