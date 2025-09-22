/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_open.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:11 by moaljazz          #+#    #+#             */
/*   Updated: 2025/09/22 02:42:41 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_file(char *file, t_rdr type)
{
	int	fd;

	fd = -1;
	if (file)
	{
		if (type == IN)
			fd = open(file, O_RDONLY);
		else if (type == OUT)
			fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (type == APPEND)
			fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	}
	else
		return (-2);
	if (fd < 0)
	{
		perror("minishell");
		g_exit_status = errno;
	}
	return (fd);
}

static int	change_fd(int fd, bool in)
{
	if (fd != STDIN_FILENO && in)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("minishell: dup");
			g_exit_status = errno;
			close(fd);
			return (1);
		}
		close(fd);
	}
	else if (fd != STDOUT_FILENO && !in)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("minishell: dup");
			g_exit_status = errno;
			close(fd);
			return (1);
		}
		close(fd);
	}
	return (SUCCESS);
}

int	open_dup_fds(int fd_in, int fd_out, t_cmd *cmd)
{
	t_redir	*rdr;
	int		i;
	int		fd;

	i = 0;
	rdr = cmd -> redir;
	if (change_fd(fd_in, true) || change_fd(fd_out, false))
		return (1);
	while (i < cmd -> redir_count)
	{
		fd = open_file(rdr[i].filename, rdr[i].red_type);
		if (fd == -1)
			return (1);
		if (fd == -2)
		{
			if (rdr[i].here_fd >= 0 && change_fd(rdr[i].here_fd, true))
				return (1);
		}
		else if (rdr[i].red_type == IN && change_fd(fd, true))
			return (1);
		else if (rdr[i].red_type != IN && change_fd(fd, false))
			return (1);
		i++;
	}
	return (SUCCESS);
}
