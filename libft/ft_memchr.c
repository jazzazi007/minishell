/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:02:18 by moaljazz          #+#    #+#             */
/*   Updated: 2024/09/14 16:02:19 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char		ch;
	const unsigned char	*ptr;

	ptr = s;
	ch = c;
	while (n-- > 0)
	{
		if (*ptr == ch)
		{
			return ((void *)ptr);
		}
		ptr++;
	}
	return (NULL);
}
