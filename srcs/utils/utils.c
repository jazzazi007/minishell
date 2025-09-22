/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:48:22 by moaljazz          #+#    #+#             */
/*   Updated: 2025/09/22 05:46:16 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_empty(const char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (true);
	while (line[i])
	{
		if (!((line[i] >= 9 && line[i] <= 13) || line[i] == 32))
			return (false);
		i++;
	}
	return (true);
}

bool	is_parent_builtin(const char *cmd)
{
	if (!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"))
		return (true);
	if (!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "cd"))
		return (true);
	return (false);
}

void	close_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

void	ft_close_fdpair(int fd[2])
{
	if (fd[0] > 0)
		close(fd[0]);
	if (fd[1] > 0)
		close(fd[1]);
}
