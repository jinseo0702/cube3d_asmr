/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <jinseo@student.42gyeongsan.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 17:15:04 by jinseo            #+#    #+#             */
/*   Updated: 2024/03/29 15:01:18 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*reptr;
	size_t	cmp;

	cmp = nmemb * size;
	if (!cmp && cmp / nmemb != size)
		exit(1);
		// return (NULL);
	reptr = (void *)malloc(size * nmemb);
	if (!reptr)
		exit(1);
		// return (NULL);
	ft_memset(reptr, 0, size * nmemb);
	return (reptr);
}
