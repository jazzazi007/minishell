/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:45 by moaljazz          #+#    #+#             */
/*   Updated: 2025/06/18 19:23:48 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void check_pipes_forks(char *ag, char **env)
{
    if (!is_valid_pipe_syntax(ag))
    {
        printf("Error: Invalid pipe or semicolon syntax\n");
        return;
    }

    int pipe_count = count_pipes(ag);
    int **pipe_fds = init_pipes(pipe_count);
    if (!pipe_fds)
        return;

    pid_t *child_pids = init_child_pids(pipe_count, pipe_fds);
    if (!child_pids)
        return;

    int i = 0;
    while (i <= pipe_count)
    {
        child_pids[i] = fork();
        if (child_pids[i] == -1)
        {
            perror("Fork failed");
            cleanup_resources(pipe_fds, child_pids, pipe_count);
            return;
        }

        if (child_pids[i] == 0)
        {
            int fd_in = STDIN_FILENO;
            char *command;

            int j = 0;
            while (j < pipe_count)
            {
                if (j != i - 1) 
                    close(pipe_fds[j][0]);
                if (j != i)      
                    close(pipe_fds[j][1]);
                j++;
            }

            if (i > 0)
                fd_in = pipe_fds[i - 1][0];

            command = get_command(ag, i);
            if (!command)
                exit(1);

            if (i < pipe_count)
                fork_operate(fd_in, command, env, pipe_fds[i]);
            else
            {
                if (fd_in != STDIN_FILENO)
                {
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                }
                cmd_exec(command, env);
                exit(1);
            }
            free(command);
            exit(1);
        }
        i++;
    }

    cleanup_resources(pipe_fds, child_pids, pipe_count);
}
