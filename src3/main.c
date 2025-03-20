#include "../include/cub3d.h"


int main(int argc, char **argv)
{
    t_data data;


    return (0);
}

int     check_args(int argc, char **argv, t_data *data)
{
    if (argc != 2)
    {
        printf("Error check argument coutn\n");
        exit(1);
    }
    if (map_parsing(argv[1], data))
}
