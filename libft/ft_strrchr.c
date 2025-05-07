/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:19:39 by moaljazz          #+#    #+#             */
/*   Updated: 2024/09/14 16:19:40 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;

	str = NULL;
	while (*s != '\0')
	{
		if ((char)c == *s)
		{
			str = (char *)s;
		}
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return ((char *)str);
}
