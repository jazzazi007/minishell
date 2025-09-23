/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:22:51 by moaljazz          #+#    #+#             */
/*   Updated: 2025/09/22 20:51:57 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_newline(bool *newline, int *index, char **args)
{
	int	j;

	while (args[*index] && args[*index][0] == '-' && args[*index][1] == 'n')
	{
		j = 1;
		while (args[*index][j] == 'n')
			j++;
		if (args[*index][j] != '\0')
			break ;
		*newline = false;
		(*index)++;
	}
}

int	echo(char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;
	check_newline(&newline, &i, args);
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
