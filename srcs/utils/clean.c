/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:55:39 by codespace         #+#    #+#             */
/*   Updated: 2025/08/19 18:43:14 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	cleanup_resources(int **fds, pid_t *pids, int count, t_shell *shell)
{
    int i;
    int status;

    (void)shell;
    pid_t pid;
    pid = 1;
    i = 0;
    while (i < count - 1)
    {
        ft_close_fdpair(fds[i]);
        i++;
    }

    while (pid > 0)
    {
        pid = waitpid(-1, &status, 0);
        if (count != 0 && pid == pids[count - 1])
        {
            if (WIFEXITED(status))
                g_exit_status = WEXITSTATUS(status);
            if (WIFSIGNALED(status))
                g_exit_status = 128 + WTERMSIG(status);
        }
    }

    i = 0;
    while (i < count - 1)
    {
        free(fds[i]);
        i++;
    }
    free(fds);
    free(pids);
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
		free(cmd);
		cmd = tmp;
	}
}

void clean_env(char **envp)
{
	int i;

	if (!envp)
		return;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void	clean_tokens(t_tokenizer *tokens)
{
	t_tokenizer	*tmp;

	while (tokens)
	{
		tmp = tokens -> next;
		if (tokens -> value)
			free(tokens -> value);
		free(tokens);
		tokens = tmp;
	}
}

void clean_shell(t_shell *shell, int status)
{
	if (shell->cmds)
	{
		clean_cmds(shell->cmds);
		shell->cmds = NULL;
	}
	if (shell->envp)
	{
		clean_env(shell->envp);
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
