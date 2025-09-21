/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:11 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/19 18:41:00 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int fork_operate(int fd_in, int fd_out, t_cmd *cmd)
{
    t_redir *redir;

    int i = 0;
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
    write(2, "here!\n", 6);
    while (i < cmd ->redir_count)
    {

        if (redir[i].red_type == IN)
        {
            int fd = open(redir[i].filename, O_RDONLY);
            if (fd < 0)
            {
                perror("Error opening input file");
                return(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir[i].red_type == OUT)
        {
            int fd = open(redir[i].filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Error opening output file");
                return(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir[i].red_type == APPEND)
        {
            int fd = open(redir[i].filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("Error opening append file");
                return(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir[i].red_type == HEREDOC)
        {
            if (redir[i].here_fd >= 0)
            {
                dup2(redir[i].here_fd, STDIN_FILENO);
                close(redir[i].here_fd);
                redir[i].here_fd = -1;
            }
        }
        i++;
    }
    return (SUCCESS);
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
