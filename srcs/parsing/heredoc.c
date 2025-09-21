/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:21:12 by codespace         #+#    #+#             */
/*   Updated: 2025/08/19 15:01:41 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_here_expand(char *str, t_shell *shell)
{
	char *tmp;
	int i;

	tmp = ft_strdup("");
	if (!tmp)
		return (NULL);
	i = 0;
	if (ft_strchr(str, '$'))
	{	
		while (str[i])
		{
			tmp = append_expanded_part(tmp, str, &i, shell);
			if (!tmp)
			{
				shell->exit_status = 1;
				return (NULL);
			}
		}
	}
	return (tmp);
}

static int ft_write_to_heredoc(char *input, int fd[2], int status, 
	t_shell *shell)
{
	int len;
	char *tmp;
	
	if (status == 1)
	{
		tmp = ft_here_expand(input, shell);
//		expand_var(shell, input, &tmp);
		if (!tmp)
		{
			free(input);
			close(fd[0]);
			close(fd[1]);
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

int ft_exp_status(t_tokenizer *delim)
{
	int status;

	status = 1;
	if (delim -> type == T_DOUBLE || delim -> type == T_SINGLE)
	{
		ft_strstrip(&delim -> value);
		status = 0;
	}
	return (status);
}

int ft_open_heredoc(t_tokenizer *delim, t_shell *shell)
{
	int fd[2];
	char *input;
	int status;

	status = ft_exp_status(delim);
	if (pipe(fd) == -1)
		return (-1);
	while(1)
	{
		handle_herdoc();
		input = readline("> ");
		if (!input)
		{
			if(g_exit_status == 130)
			{
				close(fd[0]);
				close(fd[1]);
				return (-1);
			}
			printf("warning: heredoc terminated by EOF\n");
			break;
		}
		if (ft_strcmp(input, delim -> value) == 0)
		{
			free(input);
			break;
		}
		if (ft_write_to_heredoc(input, fd, status, shell) < 0)
			return (-1);
	}
	close(fd[1]);
	return (fd[0]);
}
