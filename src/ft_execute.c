/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:25:19 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/02 19:18:10 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_fork(pid_t id)
{
	if (id < 0)
	{
		perror("creating fork");
		return (-1);
	}
	return (0);
}

int	close_pipes(int outfile, int *pd, pid_t id, pid_t id2)
{
	int	status;

	if (outfile >= 0)
		close(outfile);
	close(pd[0]);
	close(pd[1]);
	waitpid(id, NULL, 0);
	waitpid(id2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

char	*get_cmd_path(char *cmd, char **env)
{
	char	*path_copy;
	char	*dir;
	char	*full_path;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	path_copy = ft_strdup(env[i] + 5);
	if (!path_copy)
		return (NULL);
	dir = ft_strtok(path_copy, ":");
	while (dir)
	{
		full_path = get_cmd_assist(cmd, dir);
		if (!full_path)
			return (null_ret(path_copy));
		if (access(full_path, X_OK) == 0)
			return (path_ret(full_path, path_copy));
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	return (null_ret(path_copy));
}

void	free_split(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

int	cmd_exec(char *agv, t_minishell *shell)
{
	t_tokenizer	*tokens;
	char		**cmd;
	char		*cmd_path;

	tokens = tokenize_input(agv);
	if (!tokens)
		return (1);
	expand_tokens(tokens, shell);
	cmd = build_argv(tokens);
	free_tokens(tokens);
	if (!cmd || !cmd[0])
	{
		free_split(cmd);
		return (0);
	}
	cmd_path = resolve_cmd_path(cmd[0], shell);
	if (!cmd_path)
		return (handle_ret(cmd_path, cmd, 127));
	if (execve(cmd_path, cmd, shell->envp) == -1)
	{
		shell->last_exit = 126;
		return (exceve_ret(cmd_path, cmd, 126));
	}
	shell->last_exit = 0;
	return (handle_ret_num(cmd_path, cmd, 0));
}
