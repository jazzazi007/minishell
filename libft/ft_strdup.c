/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:12:10 by moaljazz          #+#    #+#             */
/*   Updated: 2024/09/14 16:12:12 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*result;

	len = ft_strlen(s) + 1;
	result = (char *)malloc(len);
	if (!result)
	{
		return (NULL);
	}
	ft_memcpy(result, s, len);
	return (result);
}
