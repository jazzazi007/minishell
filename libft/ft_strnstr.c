/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:51:21 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/02/12 18:56:39 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	lit_len;
	char	*b;

	lit_len = ft_strlen(little);
	b = (char *)big;
	if (!lit_len)
		return ((char *)b);
	while (*b && len > 0)
	{
		if (lit_len > len)
			return (NULL);
		if (ft_strncmp(b, little, lit_len) == 0)
			return ((char *)b);
		b++;
		len--;
	}
	return (NULL);
}
