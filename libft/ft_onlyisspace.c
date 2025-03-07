#include "libft.h"

int ft_onlyisspace(char *str)
{
    if (str == NULL || *str == '\0')
        return (0);
    while (*str)
    {
        if (!ft_isspace(*str))
            return (0);
        str++;
    }
    return (1);
}