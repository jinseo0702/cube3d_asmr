/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <jinseo@student.42gyeongsan.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:25:11 by jinseo            #+#    #+#             */
/*   Updated: 2024/03/03 11:33:14 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isinstr(int c, const char *str)
{
	unsigned char	as;

	as = (unsigned char)c;
	while (*str)
    {
        if (c == *str)
            return (1);
        str++;
    }
	return (0);
}