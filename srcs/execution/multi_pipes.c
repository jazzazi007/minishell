/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:45 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/19 18:37:05 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_shell_pipes(t_shell *sh,
	int ***fds, pid_t **pids)
{
	int	count;

	(void)sh;
	count = count_pipes(sh->cmds);
	*fds = NULL;
	printf("count= %d\n", count);
	if (count > 1)
	{
		*fds = init_pipes(count - 1);
		if (!(*fds))
			return (-1);
	}
	*pids = init_child_pids(count, *fds);
	if (!(*pids))
		return (-1);
	return (count);
}

static int	child_exec(int i, int count,
	t_shell *sh, t_cmd *cmd)
{
	int		fd_in;
	int 	fd_out;
	int		j;

	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	//signal(SIGINT, SIG_DFL); // remove later maybe
//	signal(SIGQUIT, SIG_DFL); // remopve later maybe
	j = -1;
	while (++j < count - 1)
	{
		if (j != i - 1)
			close(sh->fds[j][0]);
		if (j != i)
			close(sh->fds[j][1]);
	}
	if (i > 0)
		fd_in = sh->fds[i - 1][0];
	if (i < count - 1 && count > 1)
		fd_out = sh->fds[i][1];
	printf("%d %d\n", fd_in, fd_out);
	if (fork_operate(fd_in, fd_out, cmd))
		return (1);
	cmd_exec(cmd, sh);
	return (0);
}

t_cmd *get_cmd_node(t_shell *sh, int i)
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

void	ft_free_child(int **fds, pid_t *pids, int count , t_shell *sh)
{
	cleanup_resources(fds,pids,count,sh);
	clean_shell(sh, SUCCESS);
}
bool	is_parent_builtin(const char *cmd)
{
	if (!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"))
		return (true);
	if (!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "cd"))
		return (true);
	return (false);
}

void	check_pipes_forks(t_shell	*sh)
{
	// int			**fds;
	pid_t		*pids;
	int			i;
	int			count;

	count = init_shell_pipes(sh, &sh->fds, &pids);
	if (count < 0)
		return ;
	
	// Handle 'exit' in parent if it's the only command
	if (ft_strcmp(get_cmd_node(sh, 0)->args[0], "exit") == 0 && count == 1)
	{
		cleanup_resources(sh->fds, pids, count , sh);
		exit_command(get_cmd_node(sh, 0), sh);
		return ;
	}
	// cd command handling
	if (ft_strncmp(get_cmd_node(sh, 0)->args[0], "cd", 2) == 0 && count == 1)
	{
		if ((sh->exit_status = cd(get_cmd_node(sh, 0)->args, sh->envp)))
			return ;
	}
	if (count == 1 && is_parent_builtin(sh -> cmds -> args[0]))
	{
		int in = dup(STDIN_FILENO);
		int out = dup(STDOUT_FILENO);
		if (in < 0 || out < 0)
			perror("dup");
		if (fork_operate(STDIN_FILENO, STDOUT_FILENO, sh -> cmds) == SUCCESS)
			built_ins(sh -> cmds, sh);
		dup2(in, STDIN_FILENO);
		dup2(out, STDOUT_FILENO);
		close(in);
		close(out);
		//cleanup_resources(sh->fds, pids, count , sh); // not needed maybe
	}
	else
	{
		i = -1;
		while (++i < count)
		{
			pids[i] = fork();
			if (pids[i] == -1)
			{
				perror("Fork failed");
				cleanup_resources(sh->fds, pids, count , sh);
				return ;
			}
			if (pids[i] == 0)
			{
				signal_excuter();
				// if (built_ins(get_cmd_node(sh, i), sh))

				child_exec(i, count, sh, get_cmd_node(sh, i));
				// else
				ft_free_child(sh->fds,pids,count,sh);
				exit (0);
			}
			else
			{
				signal_exc_parent();
				//waitpid(pids[i], NULL, 0);
			}
		}
	}
	cleanup_resources(sh->	fds, pids, count , sh);
}
