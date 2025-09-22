/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:55:39 by codespace         #+#    #+#             */
/*   Updated: 2025/09/22 05:44:46 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_resources(t_shell *sh)
{
	t_cmd	*cmd;
	pid_t	last_pid;

	cmd = sh -> cmds;
	last_pid = 0;
	while (cmd)
	{
		if (cmd -> pid > 0)
		{
			if (waitpid(cmd -> pid, &sh -> exit_status, 0) > 0)
				last_pid = cmd -> pid;
		}
		if (cmd -> fds[0] != -1)
			close(cmd -> fds[0]);
		if (cmd -> fds[1] != -1)
			close(cmd -> fds[1]);
		cmd = cmd -> next;
	}
	if (last_pid > 0)
	{
		if (WIFEXITED(sh -> exit_status))
			g_exit_status = WEXITSTATUS(sh -> exit_status);
		else if (WIFSIGNALED(sh -> exit_status))
			g_exit_status = 128 + WTERMSIG(sh -> exit_status);
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
	if (status)
		exit(status);
	if (!status)
		exit(SUCCESS);
}
