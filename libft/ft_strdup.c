/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <jinseo@student.42gyeongsan.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:13:57 by jinseo            #+#    #+#             */
/*   Updated: 2024/02/29 18:16:05 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		i;

	dest = (char *)malloc((sizeof(char) * ft_strlen(s)) + 1);
	i = 0;
	if (dest == NULL)
		return (NULL);
	while (*(s + i) != '\0')
	{
		*(dest + i) = *(s + i);
		i++;
	}
	*(dest + i) = '\0';
	return (dest);
}
