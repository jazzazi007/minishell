/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:18:26 by moaljazz          #+#    #+#             */
/*   Updated: 2024/09/14 16:18:27 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;

	i = 0;
	if (!str1 || !str2)
		return (0);
	while ((*str1 || *str2) && i < n)
	{
		if (*str1 != *str2)
		{
			return (*(unsigned char *)str1 - *(unsigned char *)str2);
		}
		i++;
		str1++;
		str2++;
	}
	return (0);
}
