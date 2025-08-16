/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:11 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/16 15:33:53 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void fork_operate(int fd_in, int fd_out, t_cmd *cmd)
{
    t_redir *redir;

    redir = cmd->redir;
    if (fd_in != STDIN_FILENO)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (fd_out != STDOUT_FILENO)
    {
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
    while (redir)
    {
        if (redir->red_type == 2)
        {
            int fd = open(redir->filename, O_RDONLY);
            if (fd < 0)
            {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->red_type == 0)
        {
            int fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->red_type == 1)
        {
            int fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("Error opening append file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->red_type == 3)
        {
            if (redir->here_fd >= 0)
            {
                dup2(redir->here_fd, STDIN_FILENO);
                close(redir->here_fd);
            }
        }
        redir = redir->next;
    }
}

int count_pipes(t_cmd *cmd)
{
    int pipe_count = 0;

    while (cmd)
    {
        cmd = cmd->next;
        pipe_count++;
    }
    return (pipe_count);
}
