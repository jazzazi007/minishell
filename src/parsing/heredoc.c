/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:21:12 by codespace         #+#    #+#             */
/*   Updated: 2025/08/08 10:57:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *ft_expand(char *str, t_minishell *shell)
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
			tmp = append_expanded_part(tmp, str, &i, shell, 0);
	}
	return (tmp);
}

static int ft_write_to_heredoc(char *input, int fd[2], int status, 
	t_minishell *shell)
{
	int len;
	char *tmp;
	
	if (status == 1)
	{
		tmp = ft_expand(input, shell);
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

int ft_exp_status(char **src)
{
	int status;

	status = 1;
	if (ft_strchr(*src, '\'') || ft_strchr(*src, '\"'))
	{
		ft_strstrip(src);
		status = 0;
	}
	return (status);
}

int ft_open_heredoc(char *delim, t_minishell *shell)
{
	int fd[2];
	char *input;
	int status;

	status = ft_exp_status(&delim);
	if (pipe(fd) == -1)
		return (-1);
	while(1)
	{
		input = readline("> ");
		if (!input)
		{
			printf("warning: heredoc terminated by EOF\n");
			break;
		}
		if (ft_strcmp(input, delim) == 0)
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
