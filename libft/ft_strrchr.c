/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:38:54 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/02/12 18:56:40 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				i;
	unsigned char	ch;

	i = ft_strlen(s);
	ch = c;
	if (ch == '\0')
		return ((char *)s + i);
	while (i > 0)
	{
		if (s[i] == ch)
			return ((char *)s + i);
		i--;
	}
	if (s[i] == ch)
		return ((char *)s + i);
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>

int main ()
{
	char r[] = "rama taleb";
	printf("%s\n", strrchr(r, 'a'));
	printf("%s", (char *)ft_strrchr(r, 'a'));
}*/
