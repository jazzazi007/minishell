/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 19:58:20 by moaljazz          #+#    #+#             */
/*   Updated: 2025/01/05 19:58:22 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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

int	cmd_exec(char *agv, char **envp)
{
	char	**cmd;
	char	*cmd_path;
	char	*bash_command;

	cmd_path = NULL;
	bash_command = bash_cmd(agv);
	cmd = ft_split(bash_command, ' ');
	free(bash_command);
	if (!cmd || !cmd[0])
	{
		free_split(cmd);
		return (0);
	}
	if (access(cmd[0], X_OK) == 0)
		cmd_path = ft_strdup(cmd[0]);
	else
	{
		cmd_path = get_cmd_path(cmd[0], envp);
		if (!cmd_path)
			return (handle_ret(cmd_path, cmd, 127));
	}
	if (execve(cmd_path, cmd, envp) == -1)
		return (exceve_ret(cmd_path, cmd, 126));
	return (handle_ret_num(cmd_path, cmd, 0));
}
