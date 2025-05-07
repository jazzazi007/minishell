/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:02:56 by moaljazz          #+#    #+#             */
/*   Updated: 2024/09/14 16:02:57 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest_str, const void *src_str, size_t n)
{
	char		*dest_str2;
	const char	*src_str2;

	if (n == 0)
		return (dest_str);
	if (dest_str == 0 && src_str == 0)
		return (NULL);
	dest_str2 = (char *)dest_str;
	src_str2 = (char *)src_str;
	while (n > 0)
	{
		dest_str2[n - 1] = src_str2[n - 1];
		n--;
	}
	return (dest_str2);
}
