/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:04:06 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/02/12 18:54:40 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	srclen;
	size_t	deslen;

	srclen = ft_strlen(src);
	deslen = ft_strlen(dst);
	i = 0;
	if (size <= deslen)
		return (srclen + size);
	while (src[i] && (deslen + i) < (size - 1))
	{
		dst[deslen + i] = src[i];
		i++;
	}
	dst[deslen + i] = '\0';
	return (deslen + srclen);
}
