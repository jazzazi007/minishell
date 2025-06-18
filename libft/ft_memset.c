/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:57:41 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/02/12 18:51:02 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int i, size_t n)
{
	unsigned char	*str;
	unsigned char	ch;

	str = (unsigned char *)s;
	ch = i;
	while (n)
	{
		*str++ = ch;
		n--;
	}
	return (s);
}
/*
int main ()
{
	char str[] = "rama taleb";
	printf("%s",(char *)ft_memset( str, 'n', 3));
}*/