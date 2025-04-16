/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 21:18:43 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/02/12 18:36:27 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	num_len(long num)
{
	size_t	length;

	length = 0;
	if (num == 0)
		return (1);
	if (num < 0)
	{
		length += 1;
		num = num * -1;
	}
	while (num != 0)
	{
		length++;
		num /= 10;
	}
	return (length);
}

char	*ft_itoa(int n)
{
	char			*string;
	int				number;
	unsigned int	length;

	length = num_len(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	string = malloc(sizeof(char) * (length + 1));
	number = n;
	if (!string)
		return (NULL);
	if (number < 0)
	{
		string[0] = '-';
		number = -n;
	}
	if (number == 0)
		string[0] = '0';
	while (number != 0)
	{
		string[--length] = (number % 10) + '0';
		number = number / 10;
	}
	string[num_len(n)] = '\0';
	return (string);
}
