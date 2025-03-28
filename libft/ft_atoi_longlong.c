/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_longlong.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <jinseo@student.42gyeongsan.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:55:42 by jinseo            #+#    #+#             */
/*   Updated: 2024/12/01 10:00:00 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atoi_longlong(const char *nptr)
{
	long long	nb;
	int			flag;
	long long	mi;

	nb = 0;
	flag = 0;
	mi = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	while (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			mi *= -1;
		nptr++;
		flag++;
	}
	if (flag > 1)
		return (nb);
	while (*nptr >= '0' && *nptr <= '9')
	{
		nb = (*nptr - 48) + (nb * 10);
		nptr++;
	}
	return (nb * mi);
}
