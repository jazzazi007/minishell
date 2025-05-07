/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:19:07 by moaljazz          #+#    #+#             */
/*   Updated: 2024/09/14 16:19:08 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	c;

	if (*little == 0 || big == little)
		return ((char *)big);
	c = ft_strlen(little);
	while (*big && c <= len--)
	{
		if (!(ft_strncmp((char *)big, (char *)little, c)))
			return ((char *)big);
		big++;
	}
	return (NULL);
}
