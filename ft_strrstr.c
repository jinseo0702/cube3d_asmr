/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahn <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:42:33 by hyunahn           #+#    #+#             */
/*   Updated: 2024/03/15 14:59:43 by hyunahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

int	ft_strrstr(const char *s1, const char *s2, int len)
{
	int	i;
	int	j;
	int	s1_len;
	int	s2_len;

	if (*s2 == '\0')
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s2_len > s1_len || len < s2_len)
		return (0);
	i = s1_len - s2_len;
	while (i >= 0 && i >= s1_len - len)
	{
		j = 0;
		while (j < s2_len && s1[i + j] == s2[j])
			j++;
		if (j == s2_len)
			return (1);
		i--;
	}
	return (0);
}
