/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:11 by moaljazz          #+#    #+#             */
/*   Updated: 2025/06/18 19:23:14 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool is_valid_pipe_syntax(char *ag)
{
    int i = 0;
    bool found_command = false;

    while (ag[i])
    {
        while (ag[i] && ag[i] == ' ')
            i++;
        if ((ag[i] == '|' || ag[i] == ';') && !found_command)
            return false;
        
        if ((ag[i] == '|' || ag[i] == ';'))
        {
            i++;
            while (ag[i] && ag[i] == ' ')
                i++;
            if (ag[i] == '|' || ag[i] == ';' || ag[i] == '\0')
                return false;
            found_command = false;
        }
        else if (ag[i] && ag[i] != ' ')
        {
            found_command = true;
            i++;
        }
        else if (ag[i])
            i++;
    }
    return true;
}

void fork_operate(int fd_in, char *cmd, char **env, int *pipe_fd)
{
    close(pipe_fd[0]);

    if (fd_in != STDIN_FILENO)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);
    
    cmd_exec(cmd, env);
    exit(1);
}

int count_pipes(char *ag)
{
    int i = 0;
    int pipe_count = 0;

    while (ag[i])
    {
        if (ag[i] == '|')
            pipe_count++;
        i++;
    }
    return (pipe_count);
}
