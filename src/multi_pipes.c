/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:45 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/02 19:32:44 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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


static int	init_shell_pipes(char *ag, char **env, t_minishell *sh,
	int ***fds, pid_t **pids)
{
	int	count;

	if (!is_valid_pipe_syntax(ag))
	{
		printf("Error: Invalid pipe or semicolon syntax\n");
		return (-1);
	}
	ft_memset(sh, 0, sizeof(t_minishell));
	sh->envp = env;
	count = count_pipes(ag);
	*fds = init_pipes(count);
	if (!(*fds))
		return (-1);
	*pids = init_child_pids(count, *fds);
	if (!(*pids))
		return (-1);
	return (count);
}

static void	child_exec(int i, int count, int **fds,
	t_minishell *sh, char *ag)
{
	int		fd_in;
	int		j;
	char	*cmd;

	fd_in = STDIN_FILENO;
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
	cmd = get_command(ag, i);
	if (!cmd)
		exit(1);
	if (i < count)
		fork_operate(fd_in, cmd, sh->envp, fds[i]);
	else
	{
		if (fd_in != STDIN_FILENO)
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		cmd_exec(cmd, sh);
	}
	free(cmd);
	exit(1);
}

void	check_pipes_forks(char *ag, char **env)
{
	t_minishell	sh;
	int			**fds;
	pid_t		*pids;
	int			i;
	int			count;

	count = init_shell_pipes(ag, env, &sh, &fds, &pids);
	if (count < 0)
		return ;
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
			child_exec(i, count, fds, &sh, ag);
	}
	cleanup_resources(fds, pids, count);
}
