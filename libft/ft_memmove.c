/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:04:12 by moaljazz          #+#    #+#             */
/*   Updated: 2024/09/14 16:04:12 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	if (!dest && !src)
		return (NULL);
	if (src < dest && (char *)dest < (char *)src + n)
	{
		d = (char *)dest + n;
		s = (const char *)src + n;
		while (n--)
		{
			*(--d) = *(--s);
		}
	}
	else
	{
		d = (char *)dest;
		s = (const char *)src;
		while (n--)
		{
			*d++ = *s++;
		}
	}
	return (dest);
}
