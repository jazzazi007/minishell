/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:10:01 by felayan           #+#    #+#             */
/*   Updated: 2025/09/21 22:02:09 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_here_expand(char *str, t_shell *shell)
{
	char	*tmp;
	int		i;

	tmp = ft_strdup("");
	if (!tmp)
	{
		g_exit_status = MALLOC_FAILURE;
		return (NULL);
	}
	i = 0;
	if (ft_strchr(str, '$'))
	{
		while (str[i])
		{
			tmp = append_expanded_part(tmp, str, &i, shell);
			if (!tmp)
			{
				g_exit_status = MALLOC_FAILURE;
				return (NULL);
			}
		}
	}
	return (tmp);
}

static int	ft_write_to_heredoc(char *input, int fd[2], int st, t_shell *shell)
{
	int		len;
	char	*tmp;

	if (st == 1)
	{
		tmp = ft_here_expand(input, shell);
		if (!tmp)
		{
			free(input);
			ft_close_fdpair(fd);
			return (-1);
		}
		if (ft_strcmp(tmp, "") != 0)
		{
			free(input);
			input = tmp;
		}
		else
			free(tmp);
	}
	len = ft_strlen(input);
	write(fd[1], input, len);
	write(fd[1], "\n", 1);
	free(input);
	return (0);
}

static int	ft_exp_status(t_tokens *delim)
{
	int	status;

	status = 1;
	if (delim -> type == T_DOUBLE || delim -> type == T_SINGLE)
		status = 0;
	return (status);
}

static int	handle_input(char *input, int *fd, t_tokens *delim, t_shell *sh)
{
	if (!input)
	{
		if (g_exit_status == 130)
		{
			ft_close_fdpair(fd);
			return (-1);
		}
		write(2, "minishell: warning: heredoc terminated by EOF\n", 47);
		return (0);
	}
	if (!ft_strcmp(input, delim -> value))
	{
		free(input);
		return (0);
	}
	if (ft_write_to_heredoc(input, fd, ft_exp_status(delim), sh) < 0)
	{
		ft_close_fdpair(fd);
		return (-1);
	}
	return (1);
}

int	ft_open_heredoc(t_tokens *delim, t_shell *shell)
{
	int		fd[2];
	char	*input;
	int		status;

	status = 0;
	if (pipe(fd) == -1)
		return (-1);
	while (1)
	{
		handle_herdoc();
		input = readline("> ");
		status = handle_input(input, fd, delim, shell);
		if (status == 0)
			break ;
		else if (status == -1)
			return (-1);
	}
	close(fd[1]);
	return (fd[0]);
}
