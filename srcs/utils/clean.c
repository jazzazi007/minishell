/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:20:46 by felayan           #+#    #+#             */
/*   Updated: 2025/09/24 18:47:11 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_resources(t_shell *sh)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		status;

	status = 0;
	cmd = sh -> cmds;
	while (cmd)
	{
		ft_close_fdpair(cmd -> fds);
		cmd = cmd -> next;
	}
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == sh -> last_cmd_pid)
		{
			if (WIFEXITED(status))
				sh -> exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				sh -> exit = 128 + WTERMSIG(status);
		}
		pid = waitpid(-1, &status, 0);
	}
}

void	clean_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		wrds;
	int		rdrs;

	while (cmd)
	{
		tmp = cmd -> next;
		wrds = 0;
		rdrs = cmd -> redir_count;
		while (cmd -> args[wrds])
			free(cmd -> args[wrds++]);
		while (rdrs--)
			free(cmd -> redir[rdrs].filename);
		free(cmd -> args);
		free(cmd -> redir);
		free(cmd -> cmd_path);
		free(cmd);
		cmd = tmp;
	}
}

void	clean_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return ;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	clean_tokens(t_tokens *tokens)
{
	t_tokens	*tmp;

	while (tokens)
	{
		tmp = tokens -> next;
		if (tokens -> value)
			free(tokens -> value);
		free(tokens);
		tokens = tmp;
	}
}

void	clean_shell(t_shell *shell, int status)
{
	if (shell->cmds)
	{
		clean_cmds(shell->cmds);
		shell->cmds = NULL;
	}
	if (shell->envp)
	{
		clean_strs(shell->envp);
		shell->envp = NULL;
	}
	if (shell -> tokens)
	{
		clean_tokens(shell -> tokens);
		shell -> tokens = NULL;
	}
	close_fds();
	exit(status);
}
