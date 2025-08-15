/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:45 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/13 23:07:11 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void check_pipes_forks(char *ag, char **env)
// {
//     t_minishell shell;
// 	ft_memset(&shell, 0, sizeof(t_minishell));
// 	shell.envp = env;
    
//     if (!is_valid_pipe_syntax(ag))
//     {
//         printf("Error: Invalid pipe or semicolon syntax\n");
//         return;
//     }

//     int pipe_count = count_pipes(ag);
//     int **pipe_fds = init_pipes(pipe_count);
//     if (!pipe_fds)
//         return;

//     pid_t *child_pids = init_child_pids(pipe_count, pipe_fds);
//     if (!child_pids)
//         return;

//     int i = 0;
//     while (i <= pipe_count)
//     {
//         child_pids[i] = fork();
//         if (child_pids[i] == -1)
//         {
//             perror("Fork failed");
//             cleanup_resources(pipe_fds, child_pids, pipe_count);
//             return;
//         }

//         if (child_pids[i] == 0)
//         {
//             int fd_in = STDIN_FILENO;
//             char *command;

//             int j = 0;
//             while (j < pipe_count)
//             {
//                 if (j != i - 1) 
//                     close(pipe_fds[j][0]);
//                 if (j != i)      
//                     close(pipe_fds[j][1]);
//                 j++;
//             }

//             if (i > 0)
//                 fd_in = pipe_fds[i - 1][0];

//             command = get_command(ag, i);
//             if (!command)
//                 exit(1);

//             if (i < pipe_count)
//                 fork_operate(fd_in, command, env, pipe_fds[i]);
//             else
//             {
//                 if (fd_in != STDIN_FILENO)
//                 {
//                     dup2(fd_in, STDIN_FILENO);
//                     close(fd_in);
//                 }
//                 cmd_exec(command, &shell);
//                 exit(1);
//             }
//             free(command);
//             exit(1);
//         }
//         i++;
//     }

//     cleanup_resources(pipe_fds, child_pids, pipe_count);
// }


static int	init_shell_pipes(t_minishell *sh,
	int ***fds, pid_t **pids)
{
	int	count;

	(void)sh;
	count = count_pipes(sh->cmds);
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
	fork_operate(fd_in, fd_out, cmd, fds[i]);
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
		//cd command handling
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
		// If only one command, execute it directly
	i = -1;
	while (++i <= count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("Fork failed");
			cleanup_resources(fds, pids, count);
			return ;
		}
		if (pids[i] == 0)
			child_exec(i, count, fds, sh, get_cmd_node(sh, i));
	}
	cleanup_resources(fds, pids, count);
}
