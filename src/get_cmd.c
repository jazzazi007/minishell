/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:02 by moaljazz          #+#    #+#             */
/*   Updated: 2025/06/18 19:23:04 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *get_command(char *input, int position)
{
    char *start = input;
    int pipe_count = 0;
    
    while (*start && pipe_count < position)
    {
        if (*start == '|')
            pipe_count++;
        start++;
    }
    
    while (*start && (*start == ' ' || *start == '\t'))
        start++;
    
    if (!*start)
        return NULL;
    
    char *end = start;
    while (*end && *end != '|')
        end++;
    
    int len = end - start;
    char *cmd = malloc(len + 1);
    if (!cmd)
        return NULL;
    
    strncpy(cmd, start, len);
    cmd[len] = '\0';
    
    while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '\t'))
        cmd[--len] = '\0';
    
    return cmd;
}
