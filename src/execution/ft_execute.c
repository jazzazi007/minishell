/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:25:19 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/19 18:52:07 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	signla_exc_parent();
	close(pd[0]);
	close(pd[1]);
	waitpid(id, NULL, 0);
	waitpid(id2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	setup_signal_handlers();
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

int built_ins(t_cmd *agv, t_minishell *shell)
{
	if (ft_strncmp(agv->args[0], "cd", 3) == 0)
		return(0);
	if (ft_strncmp(agv->args[0], "exit", 5) == 0)
		return (0);
	else if (ft_strncmp(agv->args[0], "echo", 5) == 0)
		return (echo(agv->args));
	else if (ft_strncmp(agv->args[0], "export", 7) == 0)
		return (export_cmd(agv->args, shell));
	else if (ft_strncmp(agv->args[0], "unset", 6) == 0)
		return (unset_cmd(agv->args, shell));
	else if (ft_strncmp(agv->args[0], "env", 4) == 0)
		return (env(shell));
	else if (ft_strncmp(agv->args[0], "pwd", 4) == 0)
		return (pwd(), 0);
	return (1);
}

void	free_fds(t_cmd *cmd)
{
	t_redir *tmp;

	while (cmd)
	{
		tmp = cmd->redir;
		while (tmp)
		{
			if (tmp->red_type == 3 && tmp->here_fd != -1)
				close(tmp->here_fd);
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
}

int	cmd_exec(t_cmd *agv, t_minishell *shell)
{
	free_fds(shell->cmds);
	if (!built_ins(agv, shell))
		return (0);
	if(agv->args[0] == NULL && agv->redir->red_type == 3)
		return(0);
	agv->cmd_path = resolve_cmd_path(agv->args[0], shell);
	if (!agv->cmd_path)
		return (handle_ret(agv->cmd_path, agv->args, 127));
	if (execve(agv->cmd_path, agv->args, shell->envp) == -1)
	{
		shell->exit_status = 126;
		return (exceve_ret(agv->cmd_path, agv->args, 126));
	}
	// shell->exit_status = 0;
	return (handle_ret_num(agv->cmd_path, agv->args, 0));
}
