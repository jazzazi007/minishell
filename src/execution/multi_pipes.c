/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:45 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/16 15:32:38 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	init_shell_pipes(t_minishell *sh,
	int ***fds, pid_t **pids)
{
	int	count;

	(void)sh;
	count = count_pipes(sh->cmds);
	// *fds = init_pipes(count);
	*fds = NULL;
	if (count > 0)
		*fds = init_pipes(count);
	if (!(*fds))
		return (-1);
	*pids = init_child_pids(count, *fds);
	if (!(*pids))
		return (-1);
	return (count);
}

static void	child_exec(int i, int count, int **fds,
	t_minishell *sh, t_cmd *cmd)
{
	int		fd_in;
	int 	fd_out;
	int		j;

	if (ft_strcmp(cmd->args[0], "exit") == 0)
		exit_command(cmd, sh);
	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	j = -1;
	while (++j < count)
	{
		if (j != i - 1)
			close(fds[j][0]);
		if (j != i)
			close(fds[j][1]);
	}
	if (i > 0)
		fd_in = fds[i - 1][0];
	if (i < count - 1)
		fd_out = fds[i][1];
	fork_operate(fd_in, fd_out, cmd);
	cmd_exec(cmd, sh);
	exit(1);
}

t_cmd *get_cmd_node(t_minishell *sh, int i)
{
	t_cmd *cmd;
	int j;

	cmd = sh->cmds;
	j = 0;
	while (cmd && j < i)
	{
		cmd = cmd->next;
		j++;
	}
	return cmd;
}

void	check_pipes_forks(t_minishell	*sh)
{
	int			**fds;
	pid_t		*pids;
	int			i;
	int			count;

	count = init_shell_pipes(sh, &fds, &pids);
	if (count < 0)
		return ;

	// Handle 'exit' in parent if it's the only command
	if (ft_strcmp(get_cmd_node(sh, 0)->args[0], "exit") == 0 && count == 1)
	{
		exit_command(get_cmd_node(sh, 0), sh);
		return ;
	}

	// cd command handling
	if (ft_strncmp(get_cmd_node(sh, 0)->args[0], "cd", 2) == 0 && count == 1)
	{
		if (cd(get_cmd_node(sh, 0)->args, sh->envp))
		{
			return ;
		}
		else
		{
			sh->exit_status = 0;
			return ;
		}
	}

	i = -1;
	while (++i < count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("Fork failed");
			cleanup_resources(fds, pids, count , sh);
			return ;
		}
		if (pids[i] == 0)
		{
			if (built_ins(get_cmd_node(sh, i), sh))
				child_exec(i, count, fds, sh, get_cmd_node(sh, i));
			else
				exit (0);
		}
	}
	cleanup_resources(fds, pids, count , sh);
}
