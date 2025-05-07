/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:03:26 by moaljazz          #+#    #+#             */
/*   Updated: 2024/09/14 16:03:27 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	while (n-- > 0)
	{
		if (*(const unsigned char *)str1 < *(const unsigned char *)str2)
		{
			return (-1);
		}
		else if (*(const unsigned char *)str1 > *(const unsigned char *)str2)
		{
			return (1);
		}
		else
		{
			str1++;
			str2++;
		}
	}
	return (0);
}
