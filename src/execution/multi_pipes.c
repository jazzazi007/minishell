/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:45 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/18 19:09:03 by ralbliwi         ###   ########.fr       */
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

	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
	if (i < count - 1 && count > 1)
		fd_out = fds[i][1];
	fork_operate(fd_in, fd_out, cmd);
	cmd_exec(cmd, sh);
	// exit(1);
}

t_cmd *get_cmd_node(t_minishell *sh, int i)
{
	t_cmd *cmd;
	int j;

	cmd = sh->cmds;
	j = 0;
	while (cmd && j < i )
	{
		cmd = cmd->next;
		j++;
	}
	return cmd;
}

void	ft_free_child(int **fds, pid_t *pids, int count , t_minishell *sh)
{
	t_cmd *tmp;
	ft_free_args(sh->envp);
	// if (sh->cmds->args)
	ft_free_args(sh->cmds->args);
	cleanup_resources(fds,pids,count,sh);
	
	if (!sh->cmds)
		return ;
	while (sh->cmds)
	{
		tmp = sh->cmds->next;
		ft_free_redir(sh->cmds->redir);
		free(sh->cmds);
		sh->cmds = tmp;
	}
	free(tmp);
	free(sh);
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
		cleanup_resources(fds, pids, count , sh);
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
			// sh->exit_status = 0;
			return ;
		}
	}

	i = -1;
	if (count == 1 && !built_ins(get_cmd_node(sh, i), sh))
	{
		//handle free
		return;
	}
	else
	{
		while (++i < count)
		{
			pids[i] = fork();
			if (pids[i] == -1)
			{
				perror("Fork failed");
				cleanup_resources(fds, pids, count , sh);
				return ;
			}
			signal_excuter();
			if (pids[i] == 0)
			{
				// if (built_ins(get_cmd_node(sh, i), sh))
				child_exec(i, count, fds, sh, get_cmd_node(sh, i));
				// else
				ft_free_child(fds,pids,count,sh);
					exit (0);
			}
		signla_exc_parent();
		}
	}
	cleanup_resources(fds, pids, count , sh);
}
