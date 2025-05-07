/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 18:03:41 by moaljazz          #+#    #+#             */
/*   Updated: 2025/01/16 18:03:42 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strspn(const char *s1, const char *s2)
{
	size_t	count;

	count = 0;
	while (*s1)
	{
		if (ft_strchr(s2, *s1))
			count++;
		else
			break ;
		s1++;
	}
	return (count);
}

char	*ft_strpbrk(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (*s1)
	{
		i = 0;
		while (s2[i])
		{
			if (*s1 == s2[i])
				return ((char *)s1);
			i++;
		}
		s1++;
	}
	return (NULL);
}

char	*ft_strtok(char *str, const char *delim)
{
	char		*token;
	static char	*static_str;

	if (str)
		static_str = str;
	if (!static_str)
		return (NULL);
	static_str += ft_strspn(static_str, delim);
	if (*static_str == '\0')
	{
		static_str = NULL;
		return (NULL);
	}
	token = static_str;
	static_str = ft_strpbrk(static_str, delim);
	if (static_str)
	{
		*static_str = '\0';
		static_str++;
	}
	return (token);
}
