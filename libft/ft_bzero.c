/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 15:08:25 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/02/12 18:35:49 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*p;
	size_t	i;

	i = 0;
	p = s;
	while (n)
	{
		p[i] = '\0';
		i++;
		n--;
	}
}
/*
int main ()
{
	char str[] = "rama";
	bzero(str,2);
	printf("%s\n",str+2);
	ft_bzero(str,2);
	printf("%s",str+2);
}*/
