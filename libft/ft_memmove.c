/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:49:08 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/02/12 18:51:38 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char			*des;
	const char		*s;
	unsigned int	i;

	des = dest;
	s = src;
	i = n;
	if (des == s)
		return (des);
	if (s > des || (s + n) <= des)
	{
		ft_memcpy(des, s, n);
	}
	else
	{
		while (i > 0)
		{
			des[i - 1] = s[i - 1];
			i--;
		}
	}
	return (des);
}
